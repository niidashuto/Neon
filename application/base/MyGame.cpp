#include "MyGame.h"
#include "Quaternion.h"
#include "FbxLoader.h"
#include "ObjectFBX.h"
#include <fstream>
#include "MyMath.h"


void MyGame::Initialize()
{
#pragma region 基盤システムの初期化


    SNFramework::Initialize();

    BaseScene* scene = new TitleScene();

    sceneManager->SetNextScene(scene);

    //scene_ = new GamePlayScene();
    //titleScene_ = new TitleScene();

    //scene_->Initialize();
    //titleScene_->Initialize();

    postEffect = new PostEffect();

    postEffect->Initialize(spriteCommon, "PostEffectTest");


#pragma endregion 基盤システムの初期化

}

void MyGame::Finalize()
{
#pragma region 最初のシーンの終了

    //scene_->Finalize();
    //titleScene_->Finalize();

#pragma endregion 最初のシーンの終了

#pragma region 基盤システムの終了
   
    
    SNFramework::Finalize();
    
#pragma endregion 基盤システムの終了
}

void MyGame::Update()
{
#pragma region 基盤システムの更新
    SNFramework::Update();

    //scene_->Update();
    //titleScene_->Update();
#pragma endregion 基盤システムの更新


}

void MyGame::Draw()
{
    postEffect->PreDraw(dxCommon->GetCommandList());
    //spriteCommon->PreDraw();
    
    //scene_->Draw();
    sceneManager->Draw();
    
    
    postEffect->PostDraw(dxCommon->GetCommandList());
   
    //描画前処理
    dxCommon->PreDraw();

#pragma region 最初のシーンの描画

    postEffect->Draw(dxCommon->GetCommandList());
    imguiManager->Draw();
   
    
#pragma endregion 最初のシーンの描画

    //描画後処理
    dxCommon->PostDraw();
}









