#include "WeakEnemy.h"
#include <cassert>
#include "MyGame.h"
#include "GamePlayScene.h"
#include "Player.h"

using namespace DirectX;

GamePlayScene* WeakEnemy::gamePlayScene_ = nullptr;
Player* WeakEnemy::player_ = nullptr;

WeakEnemy::~WeakEnemy() {
	//モデルの解放

	delete modelBullet_;
	delete objBullet_;
}

// 初期化
void WeakEnemy::Initialize(Model* model, const XMFLOAT3& pos, Camera* camera) {
	// NULLポインタチェック
	assert(model);

	obj_ = Object3d::Create();

	this->pos = pos;
	obj_->SetPosition(pos);
	obj_->SetCamera(camera);
	obj_->SetModel(model);

	modelBullet_ = Model::LoadFromOBJ("weakenemybullet");
	objBullet_ = Object3d::Create();
	objBullet_->SetScale({ 2,2,2 });
	objBullet_->SetModel(modelBullet_);
	objBullet_->SetCamera(camera);
	Parameter();

	startCount = std::chrono::steady_clock::now();	//開始時間
	nowCount = std::chrono::steady_clock::now();		//現在時間
	elapsedCount;	//経過時間 経過時間=現在時間-開始時間
	maxTime = 10.0f;					//全体時間
	timeRate;

	//LoadPopEnemyData();

	easing_.Standby(false);
}

//パラメータ
void WeakEnemy::Parameter() {

	isReverse_ = false;
	//初期ステージ
	scale = { 5.0f,5.0f,5.0f };
	
	obj_->SetScale(scale);
	//初期フェーズ
	phase_ = Phase::ApproachStage1;

	//発射タイマー初期化
	fireTimer = kFireIntervalStage1;

	life_ = 1;
	isDead_ = false;

	isReverse_ = false;
	
	
}

//リセット
void WeakEnemy::Reset() { Parameter(); }

//更新
void WeakEnemy::Update() {


	XMFLOAT3 rot = obj_->GetRotation();
	rot.z += 1.0f;


	obj_->SetRotation(rot);

	//座標を移動させる
	switch (phase_) {
	case WeakEnemy::Phase::ApproachStage1:

		UpdateApproach();
		break;

	case WeakEnemy::Phase::AttackStage1:

		UpdateAttack();

		break;
	}
	

	//座標を移動させる
	switch (phase_) {
	case WeakEnemy::Phase::Leave:
		UpdateLeave();
		break;

	}

	//行列更新
	Trans();

	obj_->Update();
}

//転送
void WeakEnemy::Trans() {

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
void WeakEnemy::Fire() {

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
	std::unique_ptr<WeakEnemyBullet> newBullet = std::make_unique<WeakEnemyBullet>();
	newBullet->Initialize(modelBullet_, objBullet_, position, velocity);

	//弾を登録
	gamePlayScene_->AddEnemyBullet(std::move(newBullet));
	
	
}

//描画
void WeakEnemy::Draw() {
	if (!isDead_) {
		//モデルの描画
		obj_->Draw();
	}

}


//状態変化用の更新関数
//接近
void WeakEnemy::UpdateApproach() {
	//速度
	XMFLOAT3 velocity;
	

	//移動
	velocity = { 0.0f, 0.0f, 0.0f };
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
void WeakEnemy::UpdateAttack() {

	


	easing_.easeIn(easing_.t, easing_.b, easing_.c, easing_.d);
	
	obj_->SetPosition({ obj_->GetPosition().x,easing_.num_Y,obj_->GetPosition().z});
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
void WeakEnemy::UpdateLeave() {
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



const XMFLOAT3 WeakEnemy::Bezier(const XMFLOAT3& p0, const XMFLOAT3& p1, const XMFLOAT3& p2, const XMFLOAT3& p3, const float t)
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
XMFLOAT3 WeakEnemy::GetWorldPosition() {

	//ワールド座標を取得
	XMFLOAT3 worldPos;

	//ワールド行列の平行移動成分を取得
	worldPos.x = obj_->GetPosition().x;
	worldPos.y = obj_->GetPosition().y;
	worldPos.z = obj_->GetPosition().z;

	return worldPos;
}
//衝突を検出したら呼び出されるコールバック関数
void WeakEnemy::OnCollisionPlayer()
{
	life_--;


}
