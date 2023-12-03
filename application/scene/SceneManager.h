#pragma once
#include "BaseScene.h"

//シーン管理
class SceneManager
{
public://シングルトンインスタンス
	static SceneManager* GetInstance();

public://メンバ関数
	~SceneManager();

	//更新
	void Update();
	//描画
	void Draw();
	//終了
	void Finalize();



public:
	//次シーン予約
	void SetNextScene(BaseScene* nextScene) { nextScene_ = nextScene; }

private:
	//今のシーン(実行中のシーン)
	BaseScene* scene_ = nullptr;

	//次のシーン
	BaseScene* nextScene_ = nullptr;
};