#pragma once
#include "Camera.h"
#include "Input.h"
#include "Model.h"
#include "Object3d.h"
#include "PlayerBullet.h"
#include <DirectXMath.h>
#include <list>
#include <memory>
#include "Enemy.h"
#include "PostEffect.h"
#include "SpriteCommon.h"
#include "Easing.h"

/**
*Player.h

* �v���C���[
*/
class Player
{
private:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	~Player();

	//������
	void Initialize(Model* model, Object3d* obj, Input* input, Camera* camera,Sprite* warning);
	//���Z�b�g����
	void Reset();

	//�X�V
	void Update();

	//�v���C���[�̈ړ�����
	void Move();
	void CameraMove();

	//�v���C���[�̍U������
	void Attack();

	//�]��
	void Trans();

	void Warning();

	//���[���h���W���擾
	XMFLOAT3 GetWorldPosition();

	//�`��
	void Draw();

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	//�e���X�g���擾
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	float easeInSine(float number) { return 1 - cosf((0.8f * 3.141592f) / 2); }

	

private:
	//�e
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	//���f��
	Model* model_ = nullptr;
	Model* modelBullet_ = nullptr;

	Object3d* obj_ = nullptr;
	Object3d* objBullet_ = nullptr;
	//�J����
	Camera* camera_ = nullptr;

	Sprite* warning_ = nullptr;

	//�C���v�b�g
	Input* input_ = nullptr;

	Enemy* enemy_ = nullptr;

	PostEffect* postEffect_ = nullptr;

	SpriteCommon* spriteCommon_ = nullptr;
	
	Easing easing_= { 80.0f,20.0f,1.0f };

	//�|�W�V����
	XMFLOAT3 pos;
	//�A���O��
	XMFLOAT3 angle;

	//���S�t���O�ƃ��C�t
	bool isDead_ = false;
	int life_ = 20;

	const float rotLimitZ_ = 30.0f;
	const float rotLimitX_ = 30.0f;

	const float moveLimitX_ = 40.0f;
	const float moveLimitY_ = 40.0f;

	bool start_ = false;

	bool boss_ = false;

	bool hit_ = false;

	bool title_ = true;

	bool transition_ = false;

	bool fadeIn_ = false;

	float warningTimer_ = 60.0f * 2;

	float warningColor;

public: //�A�N�Z�b�T�A�C�����C���֐�
	bool IsDead() const { return isDead_; }
	bool IsBoss() const { return boss_; }
	bool IsHit()const { return hit_; }
	bool IsFadeIn()const { return fadeIn_; }
};
