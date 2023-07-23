#include "WeakEnemy.h"
#include <cassert>
#include "MyGame.h"

#include "Player.h"

using namespace DirectX;

WeakEnemy::~WeakEnemy() {
	//���f���̉��

	delete modelBullet_;
	delete objBullet_;
}

// ������
void WeakEnemy::Initialize(Model* model, Object3d* obj, Camera* camera) {
	// NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	camera_ = camera;
	obj_ = obj;

	modelBullet_ = Model::LoadFromOBJ("weakenemybullet");
	objBullet_ = Object3d::Create();
	objBullet_->SetScale({ 5,5,5 });
	objBullet_->SetModel(modelBullet_);
	objBullet_->SetCamera(camera_);
	Stage1Parameter();

	startCount = std::chrono::steady_clock::now();	//�J�n����
	nowCount = std::chrono::steady_clock::now();		//���ݎ���
	elapsedCount;	//�o�ߎ��� �o�ߎ���=���ݎ���-�J�n����
	maxTime = 10.0f;					//�S�̎���
	timeRate;

	//LoadPopEnemyData();
}

//�p�����[�^
void WeakEnemy::Stage1Parameter() {

	isReverse_ = false;
	//�����X�e�[�W
	scale = { 10.0f,10.0f,10.0f };
	pos = { 0.0f,10.0f,-200.0f };
	obj_->SetPosition(pos);
	obj_->SetScale(scale);
	//�����t�F�[�Y
	phase_ = Phase::ApproachStage1;

	//���˃^�C�}�[������
	fireTimer = kFireIntervalStage1;

	life_ = 1;
	isDead_ = false;

	isReverse_ = false;
	//�e���Z�b�g
	for (std::unique_ptr<WeakEnemyBullet>& bullets : WeakEnemyBullets_) {
		bullets->Reset();
	}
	
}

//���Z�b�g
void WeakEnemy::Reset() { Stage1Parameter(); }

//�X�V
void WeakEnemy::Update() {


	//���S�t���O�̗������e���폜
	WeakEnemyBullets_.remove_if(
		[](std::unique_ptr<WeakEnemyBullet>& bullet) { return bullet->IsDead(); });

	//UpdateEnemyPopCommands();

	//���W���ړ�������
	switch (phase_) {
	case WeakEnemy::Phase::ApproachStage1:

		UpdateApproachStage1();
		break;

	case WeakEnemy::Phase::AttackStage1:

		UpdateAttackStage1();

		break;
	}
	//�e�X�V
	for (std::unique_ptr<WeakEnemyBullet>& bullet : WeakEnemyBullets_) {
		bullet->Update();
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
	myGame_->AddEnemyBullet(std::move(newBullet));
	
	
}

//�`��
void WeakEnemy::Draw() {
	if (!isDead_) {
		//���f���̕`��
		obj_->Draw();

		//�e�`��
		for (std::unique_ptr<WeakEnemyBullet>& bullet : WeakEnemyBullets_) {
			bullet->Draw();
		}
	}

}


//��ԕω��p�̍X�V�֐�
//�ڋ�
void WeakEnemy::UpdateApproachStage1() {
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
void WeakEnemy::UpdateAttackStage1() {

	
	//�w��̈ʒu�ɓ��B�����甽�]
	if (pos.x >= 30.0f) {
		isReverse_ = true;
		startCount = std::chrono::steady_clock::now();
	}
	if (pos.x <= -30.0f) {
		isReverse_ = false;
		startCount = std::chrono::steady_clock::now();
	}

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
	if (life_ <= 0) {
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
