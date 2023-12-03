#pragma once

//前方宣言
class SceneManager;

//シーン基底クラス
class BaseScene {
public://メンバ関数


	virtual ~BaseScene() = default;

	//初期化
	virtual void Initialize() = 0;
	//更新
	virtual void Update() = 0;
	//描画
	virtual void Draw() = 0;
	//終了処理
	virtual void Finalize() = 0;

	virtual void SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }

private:
	//シーンマネージャー(借りてくる)
	SceneManager* sceneManager_ = nullptr;
};