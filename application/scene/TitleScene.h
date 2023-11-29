#pragma once
#include "Object3d.h"
#include "ParticleManager.h"
#include "DirectXCommon.h"
#include "Audio.h"
#include "Sprite.h"
#include "Camera.h"
#include "Player.h"
#include "Enemy.h"
#include "WeakEnemy.h"
#include "Boss.h"
#include "ObjectFBX.h"
#include "ImGuiManager.h"
#include "FbxLoader.h"

class TitleScene {
public://メンバ関数

	//初期化
	void Initialize();

	//終了
	void Finalize();

	//毎フレーム処理
	void Update();

	//描画
	void Draw();

	void CheckAllCollisions();


private:
	static DirectXCommon* dxCommon_;

	static SpriteCommon* spriteCommon_;

	static Input* input_;

	//弾
	std::list<std::unique_ptr<WeakEnemyBullet>> WeakEnemyBullets_;

	std::list<std::unique_ptr<WeakEnemy>> _WeakEnemy;

	std::stringstream enemyPopCommands;

	bool isWait_;
	int waitTimer_;

	//float color_ = 0.5f;

	ImGuiManager* imGui = nullptr;



	Sprite* sprite = nullptr;


	Model* model_1 = nullptr;
	Model* model_2 = nullptr;
	Model* modelRail_ = nullptr;
	Model* modelTitle_ = nullptr;
	//3Dオブジェクト生成
	Object3d* object3DPlayer_ = nullptr;

	Camera* camera_ = nullptr;

	GamePlayScene* scene_ = nullptr;

	//ObjectFBX* 

	
};