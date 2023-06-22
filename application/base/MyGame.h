#pragma once

#include "Sprite.h"
#include "Object3d.h"
#include "Model.h"
#include "ImGuiManager.h"
#include <imgui.h>
#include <fstream>
#include <wrl.h>
#include "SNFramework.h"
#include "Camera.h"
#include "ParticleManager.h"
#include "ObjectFBX.h"
#include "PostEffect.h"
#include "GamePlayScene.h"

class MyGame : public SNFramework
{
public:
	//初期化
	void Initialize() override;
	//終了
	void Finalize() override;
	//毎フレーム処理
	void Update() override;
	//描画
	void Draw() override;

private:

	ImGuiManager* imGui = nullptr;

	

	Sprite* sprite = nullptr;

	Sprite* sprite2 = nullptr;

	Model* model_1 = nullptr;
	Model* model_2 = nullptr;
	//3Dオブジェクト生成
	Object3d* object3d_1 = nullptr;
	Object3d* object3d_2 = nullptr;
	Object3d* object3d_3 = nullptr;

	Camera* camera_ = nullptr;

	Particle* particle1_ = nullptr;
	Particle* particle2_ = nullptr;

	ParticleManager* pm1_ = nullptr;
	ParticleManager* pm2_ = nullptr;

	FbxModel* model1 = nullptr;

	ObjectFBX* object1 = nullptr;

	PostEffect* postEffect = nullptr;

	GamePlayScene* scene_ = nullptr;

};

