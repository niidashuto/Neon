#pragma once
#include "BaseScene.h"
#include "AbstractSceneFactory.h"

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
	//void SetNextScene(BaseScene* nextScene) { nextScene_ = nextScene; }

	void ChangeScene(const std::string& sceneName);

	//シーンファクトリーのセッター
	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { this->sceneFactory_ = sceneFactory; }

private:
	//今のシーン(実行中のシーン)
	BaseScene* scene_ = nullptr;

	//次のシーン
	BaseScene* nextScene_ = nullptr;

	//シーンファクトリー(借りてくる)
	AbstractSceneFactory* sceneFactory_ = nullptr;
};