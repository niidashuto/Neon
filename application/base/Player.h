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
#include "Boss.h"
#include "ParticleManager.h"

/**
*Player.h

* プレイヤー
*/
class Player
{
private:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	~Player();

	//初期化
	void Initialize(Model* model, Object3d* obj, Input* input, Camera* camera,Sprite* white, Sprite* gameover, Sprite* gameclear);
	//リセット処理
	void Reset();

	//更新
	void Update();

	//プレイヤーの移動処理
	void Move();
	void CameraMove();

	//プレイヤーの攻撃処理
	void Attack();

	//転送
	void Trans();

	void Warning();

	//ワールド座標を取得
	XMFLOAT3 GetWorldPosition();

	//描画
	void Draw();

	void DrawParticle();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	//弾リストを取得
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	float easeInSine(float number) { return 1 - cosf((0.8f * 3.141592f) / 2); }

	

private:
	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	//モデル
	Model* model_ = nullptr;
	Model* modelBullet_ = nullptr;

	Object3d* obj_ = nullptr;
	Object3d* objBullet_ = nullptr;
	//カメラ
	Camera* camera_ = nullptr;

	Sprite* warning_ = nullptr;

	Sprite* fadeIn_white = nullptr;

	Sprite* gameover_ = nullptr;

	Sprite* gameclear_ = nullptr;

	//インプット
	Input* input_ = nullptr;

	Enemy* enemy_ = nullptr;

	PostEffect* postEffect_ = nullptr;

	SpriteCommon* spriteCommon_ = nullptr;
	
	Easing easing_= { 80.0f,20.0f,1.0f };

	Particle* playerParticle_ = nullptr;

	ParticleManager* pPm_ = nullptr;

	//ポジション
	XMFLOAT3 pos;
	//アングル
	XMFLOAT3 angle;

	//死亡フラグとライフ
	bool dead_ = false;
	int life_ = 20;

	const float kRotLimitZ_ = 30.0f;
	const float kRotLimitX_ = 30.0f;

	const float kMoveLimitX_ = 40.0f;
	const float kMoveLimitY_ = 40.0f;

	bool start_ = false;

	bool game_start_ = false;

	bool bossStart_ = false;

	bool hit_ = false;

	bool title_ = true;

	bool transition_ = false;

	bool transition_2_ = false;

	bool fadeIn_ = false;

	bool fadeInWhite_ = false;

	bool game_start_rot_ = false;

	bool game_over_ = false;

	bool game_clear_ = false;

	bool clear_change_ = false;

	bool player_extinction_ = false;

	float warning_timer = 60.0f * 2;

	float fadein_timer = 60.0f * 2;

	float game_over_timer = 60.0f * 2;

	float game_clear_timer = 60.0f * 2;

	float extinction_timer = 10.0f;

	float start_wait_timer = 60.0f * 2;

	float warning_color;

	float fadein_color;

	float gameover_color;

	float gamecler_color;

public: //アクセッサ、インライン関数
	bool IsDead() const { return dead_; }
	bool IsBoss() const { return bossStart_; }
	bool IsHit()const { return hit_; }
	bool IsFadeIn()const { return fadeIn_; }
	bool IsPlayerExtinction()const { return player_extinction_; }
	bool IsGameClear()const { return game_clear_; }
	bool IsGameOver()const { return game_over_; }
	bool IsGameClearChange()const { return clear_change_; }

	bool IsFadeInWhite()const { return fadeInWhite_; }
};
