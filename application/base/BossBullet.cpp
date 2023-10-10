#include "BossBullet.h"
#include <cassert>

using namespace DirectX;

//������
void BossBullet::Initialize(Model* model, Object3d* obj, const XMFLOAT3& position, const XMFLOAT3& velocity) {

	// NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	obj_ = obj;

	//�����Ŏ󂯎�����������W���Z�b�g
	obj_->SetPosition(position);
	velocity_ = velocity;
}

void BossBullet::Reset() { isDead_ = true; }

//�X�V
void BossBullet::Update() {
	//���W���ړ�������
	XMFLOAT3 pos = obj_->GetPosition();

	pos.x += velocity_.x;
	pos.y += velocity_.y;
	pos.z += velocity_.z;

	obj_->SetPosition(pos);

	//�s��X�V
	XMMATRIX world;

	world = XMMatrixIdentity();
	XMMATRIX matWorld = XMMatrixIdentity();

	XMMATRIX matScale = XMMatrixScaling(obj_->GetScale().x, obj_->GetScale().y, obj_->GetScale().z);

	XMMATRIX matRot = XMMatrixRotationZ(obj_->GetRotation().z)
		* XMMatrixRotationX(obj_->GetRotation().x) * XMMatrixRotationY(obj_->GetRotation().y);

	XMMATRIX matTrans = XMMatrixTranslation(obj_->GetPosition().x,
		obj_->GetPosition().y, obj_->GetPosition().z);

	//����
	matWorld = matScale * matRot * matTrans;

	obj_->SetWorld(matWorld);
	obj_->Update();
	//���Ԍo�߂Ŏ��S
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

//�`��
void BossBullet::Draw() {
	//���f���`��
	obj_->Draw();
}

//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
void BossBullet::OnCollision() { isDead_ = true; }

//���[���h���W���擾
XMFLOAT3 BossBullet::GetWorldPosition() {

	//���[���h���W���擾
	XMFLOAT3 worldPos;

	//���[���h�s��̕��s�ړ��������擾
	worldPos.x = obj_->GetPosition().x;
	worldPos.y = obj_->GetPosition().y;
	worldPos.z = obj_->GetPosition().z;

	return worldPos;
}