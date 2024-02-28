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
//#include "BaseScene.h"
#include "SceneManager.h"

/**
*GamePlayScene.h

* ゲームプレイシーン
*/
class GamePlayScene : public BaseScene
{
private:
	// テクスチャ番号
	enum class TextureIndex {
		WARNING,
		WHITE1X1,
		WHITE1280X720,
		GAMEOVER,
		GAMECLEAR,
		W,
		D,
		S,
		A,
		SPACE,
		DAMAGE,
		HP,
		NUM

	};
public://メンバ関数

	//初期化
	void Initialize() override;

	//終了
	void Finalize() override;

	//毎フレーム処理
	void Update() override;

	//描画
	void Draw() override;

	void CheckAllCollisions();

	//エネミーの弾を追加
	void AddEnemyBullet(std::unique_ptr<WeakEnemyBullet> weakEnemyBullet);
	//雑魚敵の位置
	void WeakEnemy_(XMFLOAT3 trans);
	//雑魚敵生成データ
	void LoadPopEnemyData();
	//雑魚敵生成
	void UpdateEnemyPopCommands();

	void FadeOut(XMFLOAT3 rgb);

	void FadeIn(XMFLOAT3 rgb);

	void FadeInGameOver(XMFLOAT3 rgb);

	void FadeInWhite(XMFLOAT3 rgb);

	void FadeOutWhite(XMFLOAT3 rgb);

	void UpdateChangeColor();

	void ObjectDead();

	//bool IsStart()const { return isStart_; }

	const std::list<std::unique_ptr<WeakEnemyBullet>>& GetWeakEnemyBullets() { return WeakEnemyBullets_; }

private:
	static DirectXCommon* dxCommon_;

	static SpriteCommon* spriteCommon_;

	static Input* input_;

	static SceneManager* sceneManager_;

	//弾
	std::list<std::unique_ptr<WeakEnemyBullet>> WeakEnemyBullets_;

	std::list<std::unique_ptr<WeakEnemy>> _WeakEnemy;

	std::stringstream enemyPopCommands;

	std::array<Sprite*, (uint32_t)TextureIndex::NUM> sprites;

	XMFLOAT3 selectColor_ = { 0.0f,0.0f,0.0f };//xyz=rgb

	bool isFadeOut_ = false;
	bool isFadeIn_ = false;
	bool isFadeInGameOver_ = false;
	bool isColorReverse_ = true;
	bool isFadeOutStart_ = true;
	bool isWait_;
	bool isEnd_ = false;
	bool isEndScale_ = false;
	bool transition_ = false;
	bool fadeInWhite_ = false;
	bool fadeInGameOver_ = false;
	bool fadeOutWhite_ = false;
	bool scaleSmaller_ = false;
	bool changeStage_ = false;
	bool isStart_ = false;
	bool isLimit_ = false;
	bool isDamage_ = false;
	int waitTimer_;

	float fadein_timer = 60.0f * 2;

	float default_timer = 60.0f * 1;

	float damage_timer = 10.0f;

	int phase;

	ImGuiManager* imGui = nullptr;

	Model* model_1 = nullptr;
	Model* model_2 = nullptr;
	Model* modelRail_ = nullptr;
	Model* modelTitle_ = nullptr;
	Model* modelW_ = nullptr;
	//3Dオブジェクト生成
	Object3d* object3d_1 = nullptr;
	Object3d* object3d_2 = nullptr;
	Object3d* object3d_3 = nullptr;
	Object3d* object3DPlayer_ = nullptr;
	Object3d* object3DEnemy_ = nullptr;
	Object3d* object3DBoss_ = nullptr;
	Object3d* object3DTitle_ = nullptr;
	Object3d* object3DW_ = nullptr;

	Object3d* object3DRail_ = nullptr;

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

	Easing easeFadeOut_ = Easing(1.0f, 0.0f, 3.0f);

	Easing easeFadeIn_ = Easing(0.0f, 1.0f, 3.0f);

	Easing easeFadeInGameOver_ = Easing(0.0f, 1.0f, 3.0f);

	Easing easeFadeInWhite_ = Easing(0.0f, 1.0f, 3.0f);

	Easing easeFadeOutWhite_ = Easing(1.0f, 0.0f, 3.0f);

};

