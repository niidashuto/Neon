#include "SNFramework.h"

void SNFramework::Initialize()
{
	//WindowsAPIの初期化
	winApp = new WinApp();
	winApp->Initialize();

	//DirectXの初期化
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	//スプライト共通部の初期化
	spriteCommon = new SpriteCommon();
	spriteCommon->Initialize(dxCommon);

	//入力の初期化
	input = new Input();
	input->Initialize(winApp);

	audio = new Audio();
	audio->Initialize();

	FbxLoader::GetInstance()->Initialize(dxCommon->GetDevice());

}

void SNFramework::Update()
{
	input->Update();
}

void SNFramework::Finalize()
{
	audio->Finalize();
	winApp->Finalize();
	FbxLoader::GetInstance()->Finalize();
	
}

bool SNFramework::IsEndRequest()
{
	//Windowsのメッセージ処理
	if (winApp->ProcessMessage()) {
		return true;
	}
	if (input->TriggerKey(DIK_ESCAPE)) {
		return true;
	}

	return false;
}

void SNFramework::Run()
{
	//ゲームの初期化
	Initialize();

	while (true)//ゲームループ
	{
		//毎フレーム更新
		Update();
		//終了リクエストが来たら抜ける
		if (IsEndRequest())
		{
			break;
		}
		//描画
		Draw();
	}
	//ゲームの終了
	Finalize();
}
