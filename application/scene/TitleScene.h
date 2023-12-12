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
#include "SceneManager.h"

class TitleScene :public BaseScene {

private:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public://メンバ関数

	//初期化
	void Initialize()override;

	//終了
	void Finalize()override;

	//毎フレーム処理
	void Update()override;

	//描画
	void Draw()override;

	void FadeIn(XMFLOAT3 rgb);

	void UpdateChangeColor();
private:
	static DirectXCommon* dxCommon_;

	static SpriteCommon* spriteCommon_;

	static Input* input_;

	//弾
	std::list<std::unique_ptr<WeakEnemyBullet>> WeakEnemyBullets_;

	std::list<std::unique_ptr<WeakEnemy>> _WeakEnemy;

	std::stringstream enemyPopCommands;

	bool isFadeIn_ = false;

	bool isFadeInFin_ = false;

	bool isFadeInStart_ = false;

	bool isWait_;
	int waitTimer_;

	XMFLOAT3 selectColor_ = { 0.0f,0.0f,0.0f };//xyz=rgb

	bool isColorReverse_ = false;

	//float color_ = 0.5f;

	ImGuiManager* imGui = nullptr;

	Sprite* sprite = nullptr;

	Sprite* sprite3 = nullptr;

	Sprite* sprite4 = nullptr;

	Sprite* sprite5 = nullptr;

	Sprite* spriteFadeInOut_ = new Sprite();


	Model* model_1 = nullptr;
	Model* model_2 = nullptr;
	Model* modelRail_ = nullptr;
	Model* modelTitle_ = nullptr;
	//3Dオブジェクト生成
	Object3d* object3DPlayer_ = nullptr;
	Object3d* object3DTitle_ = nullptr;
	Object3d* object3d_1 = nullptr;
	Object3d* object3d_2 = nullptr;

	Player* player_ = nullptr;
	Model* modelPlayer_ = nullptr;

	Camera* camera_ = nullptr;

	//GamePlayScene* scene_ = nullptr;

	//ObjectFBX* 

	Easing easeFadeInOut_ = Easing(0.0f, 1.0f, 1.0f);

	static SceneManager* sceneManager_;
};