#pragma once
#include "WinApp.h"
#include "DirectXCommon.h"
#include "SpriteCommon.h"
#include "Input.h"
#include "Audio.h"
#include "FbxLoader.h"


class SNFramework
{
public://メンバ関数
	//初期化
	virtual void Initialize();

	//終了
	virtual void Finalize();

	//毎フレーム更新
	virtual void Update();

	//描画
	virtual void Draw() = 0;

	//終了チェック
	virtual bool IsEndRequest();

	virtual ~SNFramework() = default;

	//実行
	void Run();

protected:

	WinApp* winApp = nullptr;

	DirectXCommon* dxCommon = nullptr;

	SpriteCommon* spriteCommon = nullptr;

	Input* input = nullptr;

	Audio* audio = nullptr;
};

