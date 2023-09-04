#include "Boss.h"
#include <cassert>

#include "Player.h"

using namespace DirectX;

Boss::~Boss() {
	//モデルの解放

	delete modelBullet_;
	delete objBullet_;
}

// 初期化
void Boss::Initialize(Model* model, Object3d* obj, Camera* camera) {
	// NULLポインタチェック
	assert(model);

	model_ = model;
	camera_ = camera;
	obj_ = obj;

	modelBullet_ = Model::LoadFromOBJ("bossbullet");
	objBullet_ = Object3d::Create();
	objBullet_->SetScale({ 5,5,5 });
	objBullet_->SetModel(modelBullet_);
	objBullet_->SetCamera(camera_);
	Stage1Parameter();

	startCount = std::chrono::steady_clock::now();	//開始時間
	nowCount = std::chrono::steady_clock::now();		//現在時間
	elapsedCount;	//経過時間 経過時間=現在時間-開始時間
	maxTime = 10.0f;					//全体時間
	timeRate;
}

//パラメータ
void Boss::Stage1Parameter() {

	isReverse_ = false;
	//初期ステージ
	scale = { 30.0f,30.0f,30.0f };
	pos = { 0.0f,40.0f,-3650.0f };
	obj_->SetPosition(pos);
	obj_->SetScale(scale);
	//初期フェーズ
	phase_ = Phase::ApproachStage1;

	//発射タイマー初期化
	fireTimer = kFireIntervalStage1;

	life_ = 1;
	isDead_ = false;

	isReverse_ = false;
	//弾リセット
	for (std::unique_ptr<BossBullet>& bullet : bossBullets_) {
		bullet->Reset();
	}
}

//リセット
void Boss::Reset() { Stage1Parameter(); }

//更新
void Boss::Update() {


	//死亡フラグの立った弾を削除
	bossBullets_.remove_if(
		[](std::unique_ptr<BossBullet>& bullet) { return bullet->IsDead(); });

	//座標を移動させる
	switch (phase_) {
	case Boss::Phase::ApproachStage1:

		UpdateApproachStage1();
		break;

	case Boss::Phase::AttackStage1:

		UpdateAttackStage1();

		break;
	}
	//弾更新
	for (std::unique_ptr<BossBullet>& bullet : bossBullets_) {
		bullet->Update();
	}

	//座標を移動させる
	switch (phase_) {
	case Boss::Phase::Leave:
		UpdateLeave();
		break;

	}

	//行列更新
	Trans();

	obj_->Update();
}

//転送
void Boss::Trans() {

	XMMATRIX world;
	//行列更新
	world = XMMatrixIdentity();
	XMMATRIX matWorld = XMMatrixIdentity();

	XMMATRIX matScale = XMMatrixScaling(obj_->GetScale().x, obj_->GetScale().y, obj_->GetScale().z);

	XMMATRIX matRot = XMMatrixRotationZ(obj_->GetRotation().z)
		* XMMatrixRotationX(obj_->GetRotation().x) * XMMatrixRotationY(obj_->GetRotation().y);

	XMMATRIX matTrans = XMMatrixTranslation(obj_->GetPosition().x,
		obj_->GetPosition().y, obj_->GetPosition().z);

	//合成
	matWorld = matScale * matRot * matTrans;

	world = matWorld;
	obj_->SetWorld(world);

}
//弾発射
void Boss::Fire() {

	assert(player_);

	//弾の速度
	const float kBulletSpeed = 0.5f;
	XMFLOAT3 velocity = {};

	//自機のワールド座標を取得
	player_->GetWorldPosition();
	//敵のワールド座標を取得
	GetWorldPosition();
	//敵→自機の差分ベクトルを求める
	velocity.x = player_->GetWorldPosition().x - GetWorldPosition().x;
	velocity.y = player_->GetWorldPosition().y - GetWorldPosition().y;
	velocity.z = player_->GetWorldPosition().z - GetWorldPosition().z;
	// ベクトルの正規化
	float len;
	len = sqrtf(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
	if (len != 0)
	{
		velocity.x /= len;
		velocity.y /= len;
		velocity.z /= len;
	}

	// ベクトルの長さを速さに合わせる

	velocity.z -= kBulletSpeed;

	//座標をコピー
	XMFLOAT3 position = obj_->GetPosition();

	//弾を生成し初期化
	std::unique_ptr<BossBullet> newBullet = std::make_unique<BossBullet>();
	newBullet->Initialize(modelBullet_, objBullet_, position, velocity);

	//弾を登録
	bossBullets_.push_back(std::move(newBullet));
}

//描画
void Boss::Draw() {
	if (!isDead_) {
		//モデルの描画
		obj_->Draw();

		//弾描画
		for (std::unique_ptr<BossBullet>& bullet : bossBullets_) {
			bullet->Draw();
		}
	}

}


//状態変化用の更新関数
//接近
void Boss::UpdateApproachStage1() {
	//速度
	XMFLOAT3 velocity;
	//移動
	velocity = { 0.0f, 0.0f, -0.3f };
	pos.x += velocity.x;
	pos.y += velocity.y;
	pos.z += velocity.z;

	obj_->SetPosition(pos);
	//発射タイマーカウントダウン
	fireTimer--;
	//指定時間に達した
	if (fireTimer <= 0) {
		//弾発射
		Fire();
		//発射タイマー初期化
		fireTimer = kFireIntervalStage1;
	}

	//指定の位置に到達したら攻撃
	if (pos.z < 30.0f) {
		phase_ = Phase::AttackStage1;
	}
}
//攻撃
void Boss::UpdateAttackStage1() {

	

	//発射タイマーカウントダウン
	fireTimer--;
	//指定時間に達した
	if (fireTimer <= 0) {
		//弾発射
		Fire();
		//発射タイマー初期化
		fireTimer = kFireIntervalStage1;
	}
	//死んだら
	if (life_ <= 0 || obj_->GetPosition().z > player_->GetWorldPosition().z) {
		isDead_ = true;
		life_ = 0;

	}
	if (isDead_)
	{
		phase_ = Phase::Leave;
	}
}

//離脱
void Boss::UpdateLeave() {
	//速度
	XMFLOAT3 velocity;

	deathTimer_--;

	//移動
	velocity = { 1.0f, 1.0f, 0.03f };
	pos.x += velocity.x;
	pos.y += velocity.y;
	pos.z += velocity.z;
	obj_->SetPosition(pos);
}

const XMFLOAT3 Boss::Bezier(const XMFLOAT3& p0, const XMFLOAT3& p1, const XMFLOAT3& p2, const XMFLOAT3& p3, const float t)
{
	XMFLOAT3 ans;
	ans.x = (1.0f - t) * (1.0f - t) * (1.0f - t) * p0.x + 3.0f * (1.0f - t) * (1.0f - t) * t *
		p1.x + 3 * (1.0f - t) * t * t * p2.x + t * t * t * p3.x;
	ans.y = (1.0f - t) * (1.0f - t) * (1.0f - t) * p0.y + 3.0f * (1.0f - t) * (1.0f - t) * t *
		p1.y + 3 * (1.0f - t) * t * t * p2.y + t * t * t * p3.y;
	ans.z = (1.0f - t) * (1.0f - t) * (1.0f - t) * p0.z + 3.0f * (1.0f - t) * (1.0f - t) * t *
		p1.z + 3 * (1.0f - t) * t * t * p2.z + t * t * t * p3.z;

	return ans;
}


//ワールド座標を取得
XMFLOAT3 Boss::GetWorldPosition() {

	//ワールド座標を取得
	XMFLOAT3 worldPos;

	//ワールド行列の平行移動成分を取得
	worldPos.x = obj_->GetPosition().x;
	worldPos.y = obj_->GetPosition().y;
	worldPos.z = obj_->GetPosition().z;

	return worldPos;
}
//衝突を検出したら呼び出されるコールバック関数
void Boss::OnCollisionPlayer()
{
	life_--;


}

