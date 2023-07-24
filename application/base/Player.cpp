#include "Player.h"
#include <cassert>

using namespace DirectX;

Player::~Player() {
	//���f���̉��

	delete modelBullet_;
	delete objBullet_;
}

void Player::Initialize(Model* model, Object3d* obj, Input* input, Camera* camera) {
	// NULL�|�C���^�`�F�b�N
	assert(model);

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	camera_ = camera;
	obj_ = obj;

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

}

void Player::Reset() {
	pos = { 0.0f, -5.0f, -60.0f };

	life_ = 5;
	isDead_ = false;
	//�e���Z�b�g
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Reset();
	}
}
void Player::Update() {

	if (!isDead_) {
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
	if (!isDead_) {
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
	float moveSpeed = 1.0f;
	float rotSpeed = 1.0f;

	if (start_ == true)
	{
		move.z--;
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
	rot.z = max(rot.z, -rotLimitZ_);
	rot.z = min(rot.z, +rotLimitZ_);

	rot.x = max(rot.x, -rotLimitX_);
	rot.x = min(rot.x, +rotLimitX_);

	//�ړ�����
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
		cmove.z--;
		tmove.z--;
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
		cmove.y -= moveSpeed;
		tmove.y -= moveSpeed;
	}
	
	
	//obj_->SetPosition(move);
	camera_->SetEye(cmove);
	camera_->SetTarget(tmove);
}

//�U������
void Player::Attack() {

	if (input_->TriggerKey(DIK_SPACE)) {
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
	if (life_ <= 0) {
		isDead_ = true;
	}
}
