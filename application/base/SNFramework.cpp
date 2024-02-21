#include "SNFramework.h"
#include "Object3d.h"
#include "ParticleManager.h"

void SNFramework::Initialize()
{
	//WindowsAPIの初期化

	winApp = WinApp::GetInstance();

	//DirectXの初期化

	dxCommon = DirectXCommon::GetInstance();

	//スプライト共通部の初期化

	spriteCommon = SpriteCommon::GetInstance();

	//入力の初期化
	input = Input::GetInstance();


	audio = Audio::GetInstance();

	sceneManager = SceneManager::GetInstance();

	imguiManager = ImGuiManager::GetInstance();

	winApp->Initialize();

	dxCommon->Initialize(winApp);

	spriteCommon->Initialize(dxCommon);

	input->Initialize(winApp);

	audio->Initialize();


	imguiManager->Initialize(winApp,dxCommon);

	FbxLoader::GetInstance()->Initialize(dxCommon->GetDevice());

	Object3d::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height);

	ParticleManager::StaticInitialize(dxCommon->GetDevice());

	

}

void SNFramework::Update()
{

	input->Update();

	sceneManager->Update();
	imguiManager->Begin();


	imguiManager->End();
}

void SNFramework::Finalize()
{
	

	audio->Finalize();

	sceneManager->Finalize();
	delete sceneFactory;
	
	FbxLoader::GetInstance()->Finalize();

	imguiManager->Finalize();

	winApp->Finalize();
	
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
