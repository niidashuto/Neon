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
#include "Boss.h"

/**
*MyGame.h

* マイゲーム
*/
class MyGame : public SNFramework
{
public:

	//弾発射間隔
	static const int kFireIntervalStage1 = 50;
	//初期化
	void Initialize() override;
	//終了
	void Finalize() override;
	//毎フレーム処理
	void Update() override;
	//描画
	void Draw() override;

	//当たり判定
	void CheckAllCollisions();
	//エネミーの弾を追加
	void AddEnemyBullet(std::unique_ptr<WeakEnemyBullet> weakEnemyBullet);
	//雑魚敵の位置
	void WeakEnemy_(XMFLOAT3 trans);
	//雑魚敵生成データ
	void LoadPopEnemyData();
	//雑魚敵生成
	void UpdateEnemyPopCommands();

	
	const std::list<std::unique_ptr<WeakEnemyBullet>>& GetWeakEnemyBullets() { return WeakEnemyBullets_; }
	

	
	

private:

	//弾
	std::list<std::unique_ptr<WeakEnemyBullet>> WeakEnemyBullets_;

	std::list<std::unique_ptr<WeakEnemy>> _WeakEnemy;

	std::stringstream enemyPopCommands;
	
	bool isWait_;
	int waitTimer_;

	//float color_ = 0.5f;

	ImGuiManager* imGui = nullptr;

	

	Sprite* sprite = nullptr;

	Sprite* sprite2 = nullptr;

	Model* model_1 = nullptr;
	Model* model_2 = nullptr;
	Model* modelRail_ = nullptr;
	Model* modelTitle_ = nullptr;
	//3Dオブジェクト生成
	Object3d* object3d_1 = nullptr;
	Object3d* object3d_2 = nullptr;
	Object3d* object3d_3 = nullptr;
	Object3d* object3DPlayer_ = nullptr;
	Object3d* object3DEnemy_ = nullptr;
	Object3d* object3DBoss_ = nullptr;
	Object3d* object3DTitle_ = nullptr;

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

	Boss* boss_ = nullptr;
	Model* modelBoss_ = nullptr;

};

