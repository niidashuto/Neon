#include "WeakEnemy.h"
#include <cassert>
#include "MyGame.h"
#include "GamePlayScene.h"
#include "Player.h"

using namespace DirectX;

GamePlayScene* WeakEnemy::gamePlayScene_ = nullptr;
Player* WeakEnemy::player_ = nullptr;

WeakEnemy::~WeakEnemy() {
	//���f���̉��

	delete modelBullet_;
	delete objBullet_;
}

// ������
void WeakEnemy::Initialize(Model* model, const XMFLOAT3& pos, Camera* camera) {
	// NULL�|�C���^�`�F�b�N
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

	startCount = std::chrono::steady_clock::now();	//�J�n����
	nowCount = std::chrono::steady_clock::now();		//���ݎ���
	elapsedCount;	//�o�ߎ��� �o�ߎ���=���ݎ���-�J�n����
	maxTime = 10.0f;					//�S�̎���
	timeRate;

	//LoadPopEnemyData();

	easing_.Standby(false);
}

//�p�����[�^
void WeakEnemy::Parameter() {

	isReverse_ = false;
	//�����X�e�[�W
	scale = { 5.0f,5.0f,5.0f };
	
	obj_->SetScale(scale);
	//�����t�F�[�Y
	phase_ = Phase::ApproachStage1;

	//���˃^�C�}�[������
	fireTimer = kFireIntervalStage1;

	life_ = 1;
	isDead_ = false;

	isReverse_ = false;
	
	
}

//���Z�b�g
void WeakEnemy::Reset() { Parameter(); }

//�X�V
void WeakEnemy::Update() {


	XMFLOAT3 rot = obj_->GetRotation();
	rot.z += 1.0f;


	obj_->SetRotation(rot);

	//���W���ړ�������
	switch (phase_) {
	case WeakEnemy::Phase::ApproachStage1:

		UpdateApproach();
		break;

	case WeakEnemy::Phase::AttackStage1:

		UpdateAttack();

		break;
	}
	

	//���W���ړ�������
	switch (phase_) {
	case WeakEnemy::Phase::Leave:
		UpdateLeave();
		break;

	}

	//�s��X�V
	Trans();

	obj_->Update();
}

//�]��
void WeakEnemy::Trans() {

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
//�e����
void WeakEnemy::Fire() {

	assert(player_);

	//�e�̑��x
	const float kBulletSpeed = 0.5f;
	XMFLOAT3 velocity = {};

	//���@�̃��[���h���W���擾
	player_->GetWorldPosition();
	//�G�̃��[���h���W���擾
	GetWorldPosition();
	//�G�����@�̍����x�N�g�������߂�
	velocity.x = player_->GetWorldPosition().x - GetWorldPosition().x;
	velocity.y = player_->GetWorldPosition().y - GetWorldPosition().y;
	velocity.z = player_->GetWorldPosition().z - GetWorldPosition().z;
	// �x�N�g���̐��K��
	float len;
	len = sqrtf(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
	if (len != 0)
	{
		velocity.x /= len;
		velocity.y /= len;
		velocity.z /= len;
	}

	// �x�N�g���̒����𑬂��ɍ��킹��

	velocity.z -= kBulletSpeed;

	//���W���R�s�[
	XMFLOAT3 position = obj_->GetPosition();

	//�e�𐶐���������
	std::unique_ptr<WeakEnemyBullet> newBullet = std::make_unique<WeakEnemyBullet>();
	newBullet->Initialize(modelBullet_, objBullet_, position, velocity);

	//�e��o�^
	gamePlayScene_->AddEnemyBullet(std::move(newBullet));
	
	
}

//�`��
void WeakEnemy::Draw() {
	if (!isDead_) {
		//���f���̕`��
		obj_->Draw();
	}

}


//��ԕω��p�̍X�V�֐�
//�ڋ�
void WeakEnemy::UpdateApproach() {
	//���x
	XMFLOAT3 velocity;
	

	//�ړ�
	velocity = { 0.0f, 0.0f, 0.0f };
	pos.x += velocity.x;
	pos.y += velocity.y;
	pos.z += velocity.z;

	
	obj_->SetPosition(pos);
	//���˃^�C�}�[�J�E���g�_�E��
	fireTimer--;
	//�w�莞�ԂɒB����
	if (fireTimer <= 0) {
		//�e����
		Fire();
		//���˃^�C�}�[������
		fireTimer = kFireIntervalStage1;
	}

	//�w��̈ʒu�ɓ��B������U��
	if (pos.z < 30.0f) {
		phase_ = Phase::AttackStage1;
	}

}
//�U��
void WeakEnemy::UpdateAttack() {

	


	easing_.easeIn(easing_.t, easing_.b, easing_.c, easing_.d);
	
	obj_->SetPosition({ obj_->GetPosition().x,easing_.num_Y,obj_->GetPosition().z});
	//���˃^�C�}�[�J�E���g�_�E��
	fireTimer--;
	//�w�莞�ԂɒB����
	if (fireTimer <= 0) {
		//�e����
		Fire();
		//���˃^�C�}�[������
		fireTimer = kFireIntervalStage1;
	}
	//���񂾂�
	if (life_ <= 0 || obj_->GetPosition().z > player_->GetWorldPosition().z) {
		isDead_ = true;
		life_ = 0;

	}
	if (isDead_)
	{
		phase_ = Phase::Leave;
	}
}

//���E
void WeakEnemy::UpdateLeave() {
	//���x
	XMFLOAT3 velocity;

	deathTimer_--;

	//�ړ�
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


//���[���h���W���擾
XMFLOAT3 WeakEnemy::GetWorldPosition() {

	//���[���h���W���擾
	XMFLOAT3 worldPos;

	//���[���h�s��̕��s�ړ��������擾
	worldPos.x = obj_->GetPosition().x;
	worldPos.y = obj_->GetPosition().y;
	worldPos.z = obj_->GetPosition().z;

	return worldPos;
}
//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
void WeakEnemy::OnCollisionPlayer()
{
	life_--;


}
