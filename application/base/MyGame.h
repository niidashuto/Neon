#pragma once

#include "Sprite.h"
#include "Object3d.h"
#include "Model.h"
#include "ImGuiManager.h"
#include <imgui.h>
#include <wrl.h>
#include "SNFramework.h"
#include "Camera.h"
#include "ParticleManager.h"
#include "ObjectFBX.h"
#include "PostEffect.h"
#include "GamePlayScene.h"
#include "Player.h"
#include "Enemy.h"
#include "WeakEnemy.h"
#include <sstream>

class MyGame : public SNFramework
{
public:

	//�e���ˊԊu
	static const int kFireIntervalStage1 = 50;
	//������
	void Initialize() override;
	//�I��
	void Finalize() override;
	//���t���[������
	void Update() override;
	//�`��
	void Draw() override;

	//�����蔻��
	void CheckAllCollisions();

	void AddEnemyBullet(std::unique_ptr<WeakEnemyBullet> weakEnemyBullet);

	void WeakEnemy_(XMFLOAT3 trans);

	void LoadPopEnemyData();

	void UpdateEnemyPopCommands();

	
	const std::list<std::unique_ptr<WeakEnemyBullet>>& GetWeakEnemyBullets() { return WeakEnemyBullets_; }
	

	

private:

	//�e
	std::list<std::unique_ptr<WeakEnemyBullet>> WeakEnemyBullets_;

	std::list<std::unique_ptr<WeakEnemy>> _WeakEnemy;

	std::stringstream enemyPopCommands;
	
	bool isWait_;
	int waitTimer_;

	ImGuiManager* imGui = nullptr;

	

	Sprite* sprite = nullptr;

	Sprite* sprite2 = nullptr;

	Model* model_1 = nullptr;
	Model* model_2 = nullptr;
	Model* modelRail_ = nullptr;
	//3D�I�u�W�F�N�g����
	Object3d* object3d_1 = nullptr;
	Object3d* object3d_2 = nullptr;
	Object3d* object3d_3 = nullptr;
	Object3d* object3DPlayer_ = nullptr;
	Object3d* object3DEnemy_ = nullptr;
	//Object3d* object3DWeakEnemy_ = nullptr;
	Object3d* object3DRail_ = nullptr;
	//Object3d* obj_ = nullptr;

	Camera* camera_ = nullptr;

	Particle* particle1_ = nullptr;
	Particle* particle2_ = nullptr;

	ParticleManager* pm1_ = nullptr;
	ParticleManager* pm2_ = nullptr;

	FbxModel* model1 = nullptr;

	ObjectFBX* object1 = nullptr;

	PostEffect* postEffect = nullptr;

	GamePlayScene* scene_ = nullptr;

	Player* player_ = nullptr;
	Model* modelPlayer_ = nullptr;

	Enemy* enemy_ = nullptr;
	Model* modelEnemy_ = nullptr;

	WeakEnemy* weakEnemy_ = nullptr;
	Model* modelWeakEnemy_ = nullptr;

};

