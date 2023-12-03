#include "MyGame.h"
#include "Quaternion.h"
#include "FbxLoader.h"
#include "ObjectFBX.h"
#include <fstream>
#include "MyMath.h"


void MyGame::Initialize()
{
#pragma region ��ՃV�X�e���̏�����


    SNFramework::Initialize();

    BaseScene* scene = new TitleScene();

    sceneManager->SetNextScene(scene);

    //scene_ = new GamePlayScene();
    //titleScene_ = new TitleScene();

    //scene_->Initialize();
    //titleScene_->Initialize();

    postEffect = new PostEffect();

    postEffect->Initialize(spriteCommon, "PostEffectTest");


#pragma endregion ��ՃV�X�e���̏�����

}

void MyGame::Finalize()
{
#pragma region �ŏ��̃V�[���̏I��

    //scene_->Finalize();
    //titleScene_->Finalize();

#pragma endregion �ŏ��̃V�[���̏I��

#pragma region ��ՃV�X�e���̏I��
   
    
    SNFramework::Finalize();
    
#pragma endregion ��ՃV�X�e���̏I��
}

void MyGame::Update()
{
#pragma region ��ՃV�X�e���̍X�V
    SNFramework::Update();

    //scene_->Update();
    //titleScene_->Update();
#pragma endregion ��ՃV�X�e���̍X�V


}

void MyGame::Draw()
{
    postEffect->PreDraw(dxCommon->GetCommandList());
    //spriteCommon->PreDraw();
    
    //scene_->Draw();
    sceneManager->Draw();
    
    
    postEffect->PostDraw(dxCommon->GetCommandList());
   
    //�`��O����
    dxCommon->PreDraw();

#pragma region �ŏ��̃V�[���̕`��

    postEffect->Draw(dxCommon->GetCommandList());
    imguiManager->Draw();
   
    
#pragma endregion �ŏ��̃V�[���̕`��

    //�`��㏈��
    dxCommon->PostDraw();
}









