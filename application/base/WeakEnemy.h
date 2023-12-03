#pragma once
#include "Camera.h"
#include "WeakEnemyBullet.h"
#include "Model.h"
#include "Object3d.h"
#include <chrono>
#include <DirectXMath.h>
#include <list>
#include <memory>
#include <sstream>
#include <Vector3.h>
#include "Easing.h"
//#include "GamePlayScene.h"
//#include "SceneManager.h"

//���@�N���X�̑O���錾
class Player;

class MyGame;

class GamePlayScene;

/**
*WeakEnemy.h

* �G���G
*/
class WeakEnemy {
private:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	~WeakEnemy();
	//�e���ˊԊu
	static const int kFireIntervalStage1 = 50;
	//������
	void Initialize(Model* model, const XMFLOAT3& pos, Camera* camera);

	//���Z�b�g����
	void Reset();

	//�p�����[�^
	void Parameter();
	//�X�V
	void Update();
	//�]���@
	void Trans();
	//�e����
	void Fire();
	//���[���h���W���擾
	XMFLOAT3 GetWorldPosition();
	//�`��
	void Draw();

	//��ԕω��p�̍X�V�֐�
	//�ڋ�
	void UpdateApproach();
	//�U��
	void UpdateAttack();
	//���E
	void UpdateLeave();

	
	//�x�W�G
	const XMFLOAT3 Bezier(const XMFLOAT3& p0, const XMFLOAT3& p1, const XMFLOAT3& p2, const XMFLOAT3& p3, const float t);

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollisionPlayer();

	//�e���X�g���擾
	//const std::list<std::unique_ptr<WeakEnemyBullet>>& GetWeakEnemyBullets() { return WeakEnemyBullets_; }

	

private:

	//�e
	//std::list<std::unique_ptr<WeakEnemyBullet>> WeakEnemyBullets_;

	

	
	Model* modelBullet_ = nullptr;

	Object3d* obj_ = nullptr;
	Object3d* objBullet_ = nullptr;

	Easing easing_ = { 80.0f,20.0f,1.0f};
	

	static GamePlayScene *gamePlayScene_;

	//�s���t�F�[�Y
	enum class Phase {
		//��������X�e�[�W1
		ApproachStage1, //�ڋ�
		AttackStage1,   //�U��

		//�S��
		Leave, //���E

	};

	//�t�F�[�Y
	Phase phase_;
	//�e���˃^�C�}�[
	int32_t fireTimer = 0;
	//�X�P�[��
	XMFLOAT3 scale;
	//�|�W�V����
	XMFLOAT3 pos;
	//�A���O��
	XMFLOAT3 angle;

	//���@
	static Player* player_;
	//���Ԍv��
	std::chrono::steady_clock::time_point startCount;	//�J�n����
	std::chrono::steady_clock::time_point nowCount;		//���ݎ���
	std::chrono::microseconds elapsedCount;	//�o�ߎ��� �o�ߎ���=���ݎ���-�J�n����
	float	maxTime = 10.0f;					//�S�̎���
	float	timeRate;
	//����_
	XMFLOAT3 start;
	XMFLOAT3 p1;
	XMFLOAT3 p2;
	XMFLOAT3 end;

	//���S�t���O�ƃ��C�t
	bool isDead_;
	int life_;

	

	int deathTimer_ = 60 * 5;

	

	//���]�t���O
	bool isReverse_ = false;

public:
	bool IsDead() const { return isDead_; }
	static void SetPlayer(Player* player) { player_ = player; }

	static void SetGamePlayScene(GamePlayScene* gamePlayScene) { gamePlayScene_ = gamePlayScene; }
};

