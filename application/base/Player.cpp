#include "Player.h"
#include <cassert>
#include <imgui.h>


using namespace DirectX;

Player::~Player() {
	//���f���̉��

	delete modelBullet_;
	delete objBullet_;
}

void Player::Initialize(Model* model, Object3d* obj, Input* input, Camera* camera,Sprite* white, Sprite* gameover,Sprite* gameclear,Sprite* hp) {
	// NULL�|�C���^�`�F�b�N
	assert(model);

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	camera_ = camera;
	obj_ = obj;
	fadeIn_white = white;
	gameover_ = gameover;
	gameclear_ = gameclear;

	hp_ = hp;

	modelBullet_ = Model::LoadFromOBJ("playerbullet");
	objBullet_ = Object3d::Create();

	objBullet_->SetScale({ 5,5,5 });
	objBullet_->SetModel(modelBullet_);
	objBullet_->SetCamera(camera_);
	
	playerParticle_ = Particle::LoadFromParticleTexture("lightblue1x1.png");
	pPm_ = ParticleManager::Create();
	pPm_->SetParticleModel(playerParticle_);
	pPm_->SetCamera(camera_);

	//�V���O���g���C���X�^���X���擾
	this->input_ = input;

	//���[���h�ϊ��̏�����
	pos = { 0.0f,20.0f,-60.0f };
	obj_->SetPosition(pos);

	fadeIn_white->SetColor({ 1,1,1,fadein_color });

	gameover_->SetColor({ 1,1,1,gameover_color });

	gameclear_->SetColor({ 1,1,1,gamecler_color });

	Reset();

}

void Player::Reset() {
	
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

		Hp();

		//�e�X�V
		for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
			pPm_->ActiveZ(playerParticle_, { bullet->GetWorldPosition()}, {0.0f ,0.0f,0.0f}, {0.0f,0.0f,0.0f-2.0f}, {0.0f,0.0f,0.0f}, 1, {5.0f, 0.0f});
			bullet->Update();
		}

		//�ړ�����
		Trans();
	}

	obj_->Update();

	pPm_->Update();
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

void Player::DrawParticle()
{
	pPm_->Draw();
}

//�ړ�����
void Player::Move() {

	XMFLOAT3 move = obj_->GetPosition();
	float moveY = obj_->GetPosition().y;
	XMFLOAT3 rot = obj_->GetRotation();
	XMFLOAT3 scale = obj_->GetScale();
	float moveSpeed = 1.0f;
	float rotSpeed = 1.0f;

	if (start_ == true&&!bossStart_&&!game_over_)
	{
		move.z-=2.0f;
		
		if (move.z <= -3500.0f)
		{
			bossStart_ = true;
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
	//XMMATRIX matTrans = XMMatrixIdentity();
	if (input_->Pushkey(DIK_A)) {
		move.x += moveSpeed;
		rot.z -= rotSpeed;
	}
	if (input_->Pushkey(DIK_D)) {
		move.x -= moveSpeed;
		rot.z += rotSpeed;
	}
	if (input_->Pushkey(DIK_W)) {
		moveY += moveSpeed;
		rot.x += rotSpeed;
	}
	if (input_->Pushkey(DIK_S)) {
		moveY -= moveSpeed;
		rot.x -= rotSpeed;
	}

	if (move.z<=-680.0f) {
		start_wait_timer -= 1.0f;
		if (start_wait_timer <= 0.0f)
		{
			start_ = true;
		}
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
				dead_ = true;
				if (extinction_timer <= 0)
				{
					player_extinction_ = false;
					//gameover_color += 0.02f;
					//gameover_->SetColor({ 1,1,1,gameover_color });
					
					
				}
				
			}
		}
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

	moveY = max(moveY, 0.0f);
	moveY = min(moveY, +kMoveLimitY_);
	

	obj_->SetPosition({ move.x,moveY,move.z });
	obj_->SetRotation(rot);
	obj_->SetScale(scale);
}

void Player::CameraMove()
{
	//XMFLOAT3 move = obj_->GetPosition();
	XMFLOAT3 cmove = camera_->GetEye();
	XMFLOAT3 tmove = camera_->GetTarget();
	float moveSpeed = 1.0f;
	
	if (start_ == true&&!bossStart_&&!game_over_)
	{
		cmove.z-=2.0f;
		tmove.z-=2.0f;
	}
	
	//�L�[�{�[�h���͂ɂ��ړ�����
	XMMATRIX matTrans = XMMatrixIdentity();
	if (input_->Pushkey(DIK_RIGHT)) {
		//camera_->Flerp(cmove.x - moveSpeed, (cmove.x - moveSpeed) - cmove.x, 0.1f);
		cmove.x -= moveSpeed;
		//camera_->Flerp(tmove.x - moveSpeed, (tmove.x - moveSpeed) - tmove.x, 0.1f);
		tmove.x -= moveSpeed;
	}
	if (input_->Pushkey(DIK_LEFT)) {
		
		cmove.x += moveSpeed;
		tmove.x += moveSpeed;
	}
	if (input_->Pushkey(DIK_UP)) {
		
		cmove.y += moveSpeed;
		tmove.y += moveSpeed;
	}
	if (input_->Pushkey(DIK_DOWN)) {
		
		cmove.y-= moveSpeed;
		tmove.y-= moveSpeed;
	}

	
	
	
	//obj_->SetPosition(move);
	camera_->SetEye(cmove);
	camera_->SetTarget(tmove);
}

void Player::Hp()
{
	//HP�𐔒l�������p��
	hp_->SetSize({ 20.0f*life_,20.0f });
	hp_->SetTextureSize({ 20.0f * life_,20.0f });
}

//�U������
void Player::Attack() {
	
		if (input_->TriggerKey(DIK_SPACE)) {
			//�e�̑��x
			const float kBulletSpeed = 4.0f;
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

	//HP�����炷
	life_--;

	//HP��0�ɂȂ�����
	if (life_ <= 0) {
		game_over_ = true;
		start_ = false;
		
		//dead_ = true;
	}
}
