#include "SceneManager.h"

SceneManager* SceneManager::GetInstance()
{
	static SceneManager instance;

	return &instance;
}

SceneManager::~SceneManager()
{
}

void SceneManager::Update()
{
	//シーン切り替え機構
	if (nextScene_) {
		//旧シーンの終了
		if (scene_) {
			scene_->Finalize();
			delete scene_;
		}

		//シーン切り替え
		scene_ = nextScene_;
		nextScene_ = nullptr;

		//シーンマネージャーをセット
		scene_->SetSceneManager(this);

		//次シーンを初期化する
		scene_->Initialize();
	}

	//実行中シーンを更新する
	scene_->Update();
}

void SceneManager::Draw()
{
	scene_->Draw();
}

void SceneManager::Finalize()
{
	scene_->Finalize();
	delete scene_;
}
