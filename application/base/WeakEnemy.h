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

//自機クラスの前方宣言
class Player;

class MyGame;

class GamePlayScene;

/**
*WeakEnemy.h

* 雑魚敵
*/
class WeakEnemy {
private:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	~WeakEnemy();
	//弾発射間隔
	static const int kFireIntervalStage1 = 50;
	//初期化
	void Initialize(Model* model, const XMFLOAT3& pos, Camera* camera);

	//リセット処理
	void Reset();

	//パラメータ
	void Parameter();
	//更新
	void Update();
	//転送　
	void Trans();
	//弾発射
	void Fire();
	//ワールド座標を取得
	XMFLOAT3 GetWorldPosition();
	//描画
	void Draw();

	//状態変化用の更新関数
	//接近
	void UpdateApproach();
	//攻撃
	void UpdateAttack();
	//離脱
	void UpdateLeave();

	
	//ベジエ
	const XMFLOAT3 Bezier(const XMFLOAT3& p0, const XMFLOAT3& p1, const XMFLOAT3& p2, const XMFLOAT3& p3, const float t);

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollisionPlayer();

	//弾リストを取得
	//const std::list<std::unique_ptr<WeakEnemyBullet>>& GetWeakEnemyBullets() { return WeakEnemyBullets_; }

	

private:

	//弾
	//std::list<std::unique_ptr<WeakEnemyBullet>> WeakEnemyBullets_;

	

	
	Model* modelBullet_ = nullptr;

	Object3d* obj_ = nullptr;
	Object3d* objBullet_ = nullptr;

	Easing easing_ = { 80.0f,20.0f,1.0f};
	

	static GamePlayScene *gamePlayScene_;

	//行動フェーズ
	enum class Phase {
		//ここからステージ1
		ApproachStage1, //接近
		AttackStage1,   //攻撃

		//全体
		Leave, //離脱

	};

	//フェーズ
	Phase phase_;
	//弾発射タイマー
	int32_t fireTimer = 0;
	//スケール
	XMFLOAT3 scale;
	//ポジション
	XMFLOAT3 pos;
	//アングル
	XMFLOAT3 angle;

	//自機
	static Player* player_;
	//時間計測
	std::chrono::steady_clock::time_point startCount;	//開始時間
	std::chrono::steady_clock::time_point nowCount;		//現在時間
	std::chrono::microseconds elapsedCount;	//経過時間 経過時間=現在時間-開始時間
	float	maxTime = 10.0f;					//全体時間
	float	timeRate;
	//制御点
	XMFLOAT3 start;
	XMFLOAT3 p1;
	XMFLOAT3 p2;
	XMFLOAT3 end;

	//死亡フラグとライフ
	bool isDead_;
	int life_;

	

	int deathTimer_ = 60 * 5;

	

	//反転フラグ
	bool isReverse_ = false;

public:
	bool IsDead() const { return isDead_; }
	static void SetPlayer(Player* player) { player_ = player; }

	static void SetGamePlayScene(GamePlayScene* gamePlayScene) { gamePlayScene_ = gamePlayScene; }
};

