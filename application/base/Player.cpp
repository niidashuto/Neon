#include "Player.h"
#include <cassert>
#include <imgui.h>

using namespace DirectX;

Player::~Player() {
	//���f���̉��

	delete modelBullet_;
	delete objBullet_;
}

void Player::Initialize(Model* model, Object3d* obj, Input* input, Camera* camera, Sprite* warning,Sprite* white,Sprite* gameover) {
	// NULL�|�C���^�`�F�b�N
	assert(model);

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	camera_ = camera;
	obj_ = obj;
	warning_ = warning;
	fadeIn_white = white;
	gameover_ = gameover;

	modelBullet_ = Model::LoadFromOBJ("playerbullet");
	objBullet_ = Object3d::Create();

	objBullet_->SetScale({ 5,5,5 });
	objBullet_->SetModel(modelBullet_);
	objBullet_->SetCamera(camera_);
	

	//�V���O���g���C���X�^���X���擾
	this->input_ = input;

	//���[���h�ϊ��̏�����
	pos = { 0.0f,20.0f,-60.0f };
	obj_->SetPosition(pos);

	warning_->SetColor({ 1,1,1,warning_color });

	fadeIn_white->SetColor({ 1,1,1,fadein_color });

	gameover_->SetColor({ 1,1,1,gameover_color });

}

void Player::Reset() {
	//pos = { 0.0f, -5.0f, -60.0f };

	life_ = 5;
	dead_ = false;
	//�e���Z�b�g
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Reset();
	}
}
void Player::Update() {

	if (!dead_) {
		//���S�t���O�̗������e���폜
		bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) { return bullet->IsDead(); });

		//�ړ�����
		Move();
		CameraMove();
		//�U������
		Attack();

		//�e�X�V
		for (std::unique_ptr<PlayerBullet>& bullet : bullets_) { 
			bullet->Update();
		}

		//�ړ�����
		Trans();
	}

	obj_->Update();
}

void Player::Draw() {
	if (!dead_) {
		obj_->Draw();
		

		//�e�`��
		for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
			bullet->Draw();
		}

	}
}

//�ړ�����
void Player::Move() {

	XMFLOAT3 move = obj_->GetPosition();
	XMFLOAT3 rot = obj_->GetRotation();
	XMFLOAT3 scale = obj_->GetScale();
	float moveSpeed = 1.0f;
	float rotSpeed = 1.0f;

	if (start_ == true)
	{
		move.z-=1.0f;
		
		if (move.z <= -3500.0f)
		{
			boss_ = true;
			start_ = false;
			
			
		}
		
	}

	game_start_ = true;
	if (move.z <= -650.0f && !game_start_rot_)
	{
		//move.y = 55.0f;
		//move.x = -35.0f;
		game_start_rot_ = true;

	}
	if (game_start_rot_)
	{
		//move.y -= 0.5f;
		//move.x += 0.5f;
		if (move.x >= 0 && move.y <= 20.0f)
		{
			game_start_ = false;
			//game_start_rot_ = false;

		}
	}
	
	

	//�L�[�{�[�h���͂ɂ��ړ�����
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
		transition_ = true;
		
	}
	if (input_->Pushkey(DIK_2))
	{
		//rot.y -= 1.0f;
		scale.z -= 0.5f;
		scale.x -= 0.5f;
		scale.y += 0.5f;
		if (scale.z <= 0)
		{
			scale.z = max(scale.z, 0);
			scale.z = min(scale.z, 0);

			scale.x = max(scale.x, 0);
			scale.x = min(scale.x, 0);

			scale.y = max(scale.y, 10);
			scale.y = min(scale.y, 10);
		}
		transition_2_ = true;
	}

	if (input_->Pushkey(DIK_3))
	{
		//rot.y += 1.0f;
		scale.z += 0.5f;
		scale.x += 0.5f;
		scale.y -= 0.5f;

		
			scale.z = max(scale.z, 0);
			scale.z = min(scale.z, 10);

			scale.x = max(scale.x, 0);
			scale.x = min(scale.x, 10);

			scale.y = max(scale.y, 20);
			scale.y = min(scale.y, 10);
		
		//transition_2_ = true;
	}
	if (warning_color >= 1.0f)
	{
		fadeIn_ = true;
		
	}

	if (fadein_color >= 1.0f)
	{
		fadeInWhite_ = true;

	}

	if (fadeInWhite_)
	{
		fadein_timer -= 1.0f;
		if (fadein_timer <= 0.0f)
		{
			transition_2_ = false;
			move.z = -680.0f;
			camera_->SetTarget({ 0,0,0.0f });
			camera_->SetEye({ 0,0,8.0f });
			camera_->SetUp({ 0,20,0 });
			camera_->CameraMoveVector({ 0,20,-620 });
			//rot.y = 0;
			

		}
		fadein_color -= 0.02f;
		fadeIn_white->SetColor({ 1,1,1,fadein_color });
		if (fadein_color <= 0)
		{
			fadein_timer = 60.0f * 3;

		}

	}

	if (fadeIn_)
	{
		warning_timer -= 1.0f;
		if (warning_timer <= 0.0f)
		{
			transition_ = false;
			camera_->SetTarget({ 0,0,0 });
			camera_->SetEye({ 0,0,8.0f });
			camera_->SetUp({ 0,20,0 });
			camera_->CameraMoveVector({ 0,20,0 });
			rot.y = 0;
			

		}
		warning_color -= 0.02f;
		warning_->SetColor({ 1,1,1,warning_color });
		if (warning_color <= 0)
		{
			warning_timer = 60.0f * 3;

		}
		
	}
	
	

	if (title_)
	{
		rot.y += rotSpeed;

	}

	if (game_over_)
	{
		game_over_timer -= 1.0f;
		if (game_over_timer <= 0)
		{
			rot.y += 5.0f;
			scale.x -= 0.1f;
			scale.y -= 0.1f;
			scale.z -= 0.1f;
			if (scale.x <= 0)
			{
				extinction_timer -= 1.0f;
				scale.x += 0.1f;
				scale.y += 0.1f;
				scale.z += 0.1f;
				player_extinction_ = true;
				if (extinction_timer <= 0)
				{
					player_extinction_ = false;
					gameover_color += 0.02f;
					gameover_->SetColor({ 1,1,1,gameover_color });
					
				}
				
			}
		}
	}

	if (transition_) {
		warning_color += 0.02f;
		warning_->SetColor({ 1,1,1,warning_color });
	}

	if (transition_2_) {
		fadein_color += 0.02f;
		fadeIn_white->SetColor({ 1,1,1,fadein_color });
	}

	//���@�̉�](Z��)
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

	//���@�̉�](X��)
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

	//��]����
	
	rot.z = max(rot.z, -kRotLimitZ_);
	rot.z = min(rot.z, +kRotLimitZ_);

	rot.x = max(rot.x, -kRotLimitX_);
	rot.x = min(rot.x, +kRotLimitX_);

	//�ړ�����
	move.x = max(move.x, -kMoveLimitX_);
	move.x = min(move.x, +kMoveLimitX_);

	move.y = max(move.y, 0.0f);
	move.y = min(move.y, +kMoveLimitY_);
	

	obj_->SetPosition(move);
	obj_->SetRotation(rot);
	obj_->SetScale(scale);
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
	
	//�L�[�{�[�h���͂ɂ��ړ�����
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
		cmove.z+= moveSpeed;
		tmove.z+= moveSpeed;
	}

	
	
	
	//obj_->SetPosition(move);
	camera_->SetEye(cmove);
	camera_->SetTarget(tmove);
}

//�U������
void Player::Attack() {
	
		if (input_->TriggerKey(DIK_SPACE)&&!title_) {
			//�e�̑��x
			const float kBulletSpeed = 2.0f;
			XMFLOAT3 velocity(0.0f, 0.0f, -kBulletSpeed);



			XMMATRIX matVec = XMMatrixIdentity();
			matVec.r[0].m128_f32[0] = velocity.x;
			matVec.r[0].m128_f32[1] = velocity.y;
			matVec.r[0].m128_f32[2] = velocity.z;
			matVec.r[0].m128_f32[3] = 0.0f;

			matVec *= obj_->GetWorld();

			//���L�����̍��W���R�s�[
			XMFLOAT3 position = obj_->GetPosition();

			//�e�𐶐���������
			std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();

			//PlayerBullet* newBullet=new PlayerBullet();

			newBullet->Initialize(modelBullet_, objBullet_, position, velocity);

			//�e��o�^
			bullets_.push_back(std::move(newBullet));
			//���郁�����̏��L��������unique_ptr�͂�����������݂ł��Ȃ�
			//���̏��L�������n���邽�߂̋@�\�� std::move()
		}
	
}

void Player::Trans() {

	XMMATRIX world;
	//�s��X�V
	world = XMMatrixIdentity();
	XMMATRIX matWorld = XMMatrixIdentity();

	XMMATRIX matScale = XMMatrixScaling(obj_->GetScale().x, obj_->GetScale().y, obj_->GetScale().z);

	XMMATRIX matRot = XMMatrixRotationZ(obj_->GetRotation().z)
		* XMMatrixRotationX(obj_->GetRotation().x) * XMMatrixRotationY(obj_->GetRotation().y);

	XMMATRIX matTrans = XMMatrixTranslation(obj_->GetPosition().x,
		obj_->GetPosition().y, obj_->GetPosition().z);

	//����
	matWorld = matScale * matRot * matTrans;

	world = matWorld;
	obj_->SetWorld(world);

}

void Player::Warning()
{
	
}

//���[���h���W���擾
XMFLOAT3 Player::GetWorldPosition() {

	//���[���h���W���擾
	XMFLOAT3 worldPos;

	//���[���h�s��̕��s�ړ��������擾
	worldPos.x = obj_->GetPosition().x;
	worldPos.y = obj_->GetPosition().y;
	worldPos.z = obj_->GetPosition().z;

	return worldPos;
}

//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
void Player::OnCollision() {
	life_--;

	//camera_->ShakeEye({ 0.0f, 6.0f, -115.0f }, 10, { -5.0f,1.0f,-130.0f }, { 5.0f,11.0f,-100.0f });
	//camera_->ShakeTarget({ 0.0f,5.0f,0.0f }, 10, { -5.0f,0.0f,-5.0f }, { 5.0f,10.0f,5.0f });
	//camera_->Update();
	//easing_.easeIn();
	
	if (life_ <= 0) {
		game_over_ = true;
		start_ = false;
		
		//dead_ = true;
	}
}
