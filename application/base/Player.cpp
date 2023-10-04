#include "Player.h"
#include <cassert>
#include <imgui.h>

using namespace DirectX;

Player::~Player() {
	//モデルの解放

	delete modelBullet_;
	delete objBullet_;
}

void Player::Initialize(Model* model, Object3d* obj, Input* input, Camera* camera, Sprite* warning) {
	// NULLポインタチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	camera_ = camera;
	obj_ = obj;
	warning_ = warning;

	modelBullet_ = Model::LoadFromOBJ("playerbullet");
	objBullet_ = Object3d::Create();

	objBullet_->SetScale({ 5,5,5 });
	objBullet_->SetModel(modelBullet_);
	objBullet_->SetCamera(camera_);
	

	//シングルトンインスタンスを取得
	this->input_ = input;

	//ワールド変換の初期化
	pos = { 0.0f,20.0f,-60.0f };
	obj_->SetPosition(pos);

	warning_->SetColor({ 1,1,1,warningColor });

}

void Player::Reset() {
	pos = { 0.0f, -5.0f, -60.0f };

	life_ = 5;
	isDead_ = false;
	//弾リセット
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Reset();
	}
}
void Player::Update() {

	if (!isDead_) {
		//死亡フラグの立った弾を削除
		bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) { return bullet->IsDead(); });

		//移動処理
		Move();
		CameraMove();
		//攻撃処理
		Attack();

		//弾更新
		for (std::unique_ptr<PlayerBullet>& bullet : bullets_) { 
			bullet->Update();
		}

		//移動制限
		Trans();
	}

	obj_->Update();
}

void Player::Draw() {
	if (!isDead_) {
		obj_->Draw();
		ImGui::InputInt("HP", &life_);

		//弾描画
		for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
			bullet->Draw();
		}

	}
}

//移動処理
void Player::Move() {

	XMFLOAT3 move = obj_->GetPosition();
	XMFLOAT3 rot = obj_->GetRotation();
	float moveSpeed = 1.0f;
	float rotSpeed = 1.0f;

	if (start_ == true)
	{
		move.z-=1.0f;
		if (move.z <= -3500.0f)
		{
			warningTimer_--;
			for (int i = 0; i <= 100; i++)
			{
				
				warningColor += 0.1f;
				warning_->SetColor({ 1,1,1,warningColor });
					
				
				
			}
			boss_ = true;
			start_ = false;
			
			
		}
		
	}

	
	

	//キーボード入力による移動処理
	XMMATRIX matTrans = XMMatrixIdentity();
	if (input_->Pushkey(DIK_A)) {
		move.x += moveSpeed;
		rot.z -= rotSpeed;
	}
	if (input_->Pushkey(DIK_D)) {
		move.x -= moveSpeed;
		rot.z += rotSpeed;
	}
	if (input_->Pushkey(DIK_W)) {
		move.y += moveSpeed;
		rot.x += rotSpeed;
	}
	if (input_->Pushkey(DIK_S)) {
		move.y -= moveSpeed;
		rot.x -= rotSpeed;
	}

	if (input_->Pushkey(DIK_0)) {
		start_ = true;
	}

	if (input_->Pushkey(DIK_RETURN))
	{
		title_ = false;
	}

	if (title_)
	{
		rot.y += rotSpeed;

	}

	//自機の回転(Z軸)
	if (!input_->Pushkey(DIK_A)&& !input_->Pushkey(DIK_D))
	{
		if (rot.z < 0)
		{
			rot.z += rotSpeed;
		}
		else if (rot.z > 0)
		{
			rot.z -= rotSpeed;
		}
	}

	//自機の回転(X軸)
	if (!input_->Pushkey(DIK_W) && !input_->Pushkey(DIK_S))
	{
		if (rot.x < 0)
		{
			rot.x += rotSpeed;
		}
		else if (rot.x > 0)
		{
			rot.x -= rotSpeed;
		}
	}

	//回転制限
	rot.z = max(rot.z, -rotLimitZ_);
	rot.z = min(rot.z, +rotLimitZ_);

	rot.x = max(rot.x, -rotLimitX_);
	rot.x = min(rot.x, +rotLimitX_);

	//移動制限
	move.x = max(move.x, -moveLimitX_);
	move.x = min(move.x, +moveLimitX_);

	move.y = max(move.y, 0.0f);
	move.y = min(move.y, +moveLimitY_);

	obj_->SetPosition(move);
	obj_->SetRotation(rot);

}

void Player::CameraMove()
{
	//XMFLOAT3 move = obj_->GetPosition();
	XMFLOAT3 cmove = camera_->GetEye();
	XMFLOAT3 tmove = camera_->GetTarget();
	float moveSpeed = 1.0f;
	
	if (start_ == true)
	{
		cmove.z-=1.0f;
		tmove.z-=1.0f;
	}
	
	//キーボード入力による移動処理
	XMMATRIX matTrans = XMMatrixIdentity();
	if (input_->Pushkey(DIK_LEFT)) {
		//move.x -= moveSpeed;
		cmove.x -= moveSpeed;
		tmove.x -= moveSpeed;
	}
	if (input_->Pushkey(DIK_RIGHT)) {
		//move.x += moveSpeed;
		cmove.x += moveSpeed;
		tmove.x += moveSpeed;
	}
	if (input_->Pushkey(DIK_UP)) {
		//move.y += moveSpeed;
		cmove.y += moveSpeed;
		tmove.y += moveSpeed;
	}
	if (input_->Pushkey(DIK_DOWN)) {
		//move.y -= moveSpeed;
		cmove.y -= moveSpeed;
		tmove.y -= moveSpeed;
	}
	
	
	//obj_->SetPosition(move);
	camera_->SetEye(cmove);
	camera_->SetTarget(tmove);
}

//攻撃処理
void Player::Attack() {
	
		if (input_->TriggerKey(DIK_SPACE)&&!title_) {
			//弾の速度
			const float kBulletSpeed = 2.0f;
			XMFLOAT3 velocity(0.0f, 0.0f, -kBulletSpeed);



			XMMATRIX matVec = XMMatrixIdentity();
			matVec.r[0].m128_f32[0] = velocity.x;
			matVec.r[0].m128_f32[1] = velocity.y;
			matVec.r[0].m128_f32[2] = velocity.z;
			matVec.r[0].m128_f32[3] = 0.0f;

			matVec *= obj_->GetWorld();

			//自キャラの座標をコピー
			XMFLOAT3 position = obj_->GetPosition();

			//弾を生成し初期化
			std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();

			//PlayerBullet* newBullet=new PlayerBullet();

			newBullet->Initialize(modelBullet_, objBullet_, position, velocity);

			//弾を登録
			bullets_.push_back(std::move(newBullet));
			//あるメモリの所有権を持つunique_ptrはただ一つしか存在できない
			//その所有権を謙渡するための機能が std::move()
		}
	
}

void Player::Trans() {

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

void Player::Warning()
{
	
}

//ワールド座標を取得
XMFLOAT3 Player::GetWorldPosition() {

	//ワールド座標を取得
	XMFLOAT3 worldPos;

	//ワールド行列の平行移動成分を取得
	worldPos.x = obj_->GetPosition().x;
	worldPos.y = obj_->GetPosition().y;
	worldPos.z = obj_->GetPosition().z;

	return worldPos;
}

//衝突を検出したら呼び出されるコールバック関数
void Player::OnCollision() {
	life_--;

	//camera_->ShakeEye({ 0.0f, 6.0f, -115.0f }, 10, { -5.0f,1.0f,-130.0f }, { 5.0f,11.0f,-100.0f });
	//camera_->ShakeTarget({ 0.0f,5.0f,0.0f }, 10, { -5.0f,0.0f,-5.0f }, { 5.0f,10.0f,5.0f });
	//camera_->Update();
	//easing_.easeIn();
	
	if (life_ <= 0) {
		isDead_ = true;
	}
}
