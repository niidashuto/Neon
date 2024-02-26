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

    sceneFactory = new SceneFactory();
    SceneManager::GetInstance()->SetSceneFactory(sceneFactory);

    SceneManager::GetInstance()->ChangeScene("TITLE");

    postEffect = new PostEffect();

    postEffect->Initialize(spriteCommon, "PostEffectTest");


#pragma endregion ��ՃV�X�e���̏�����

}

void MyGame::Finalize()
{
#pragma region �ŏ��̃V�[���̏I��

#pragma endregion �ŏ��̃V�[���̏I��

#pragma region ��ՃV�X�e���̏I��
   
    
    SNFramework::Finalize();
    
#pragma endregion ��ՃV�X�e���̏I��
}

void MyGame::Update()
{
#pragma region ��ՃV�X�e���̍X�V
    SNFramework::Update();

#pragma endregion ��ՃV�X�e���̍X�V


}

void MyGame::Draw()
{
    postEffect->PreDraw(dxCommon->GetCommandList());
   
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









