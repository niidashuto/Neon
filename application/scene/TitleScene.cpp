#include "TitleScene.h"
#include "GamePlayScene.h"

DirectXCommon* TitleScene::dxCommon_ = DirectXCommon::GetInstance();

SpriteCommon* TitleScene::spriteCommon_ = SpriteCommon::GetInstance();

Input* TitleScene::input_ = Input::GetInstance();

SceneManager* TitleScene::sceneManager_ = SceneManager::GetInstance();

void TitleScene::Initialize()
{
    
    spriteCommon_->LoadTexture(0, "reimu.png");

    ////音声読み込み
    //Audio::GetInstance()->SoundLoadWave("Resources/fanfare.wav");
    ////音声再生
    ////audio->SoundPlayWave("Resources/fanfare.wav");

    //player_ = new Player();

    

    camera_ = new Camera();

    sprite = new Sprite();
    sprite->SetTextureIndex(0),
    sprite->Initialize(spriteCommon_, 0);

    
}

void TitleScene::Finalize()
{
    delete sprite;

}

void TitleScene::Update()
{
   //7キーを押したら
    if (input_->TriggerKey(DIK_7)) {
        //ゲームプレイシーン(次シーン)を生成
        BaseScene* scene = new GamePlayScene();
        //シーン切り替え依頼
        sceneManager_->SetNextScene(scene);
    }
    sprite->Update();
    
}

void TitleScene::Draw()
{


    //postEffect->PreDraw(dxCommon_->GetCommandList());


    Object3d::PreDraw(dxCommon_->GetCommandList());
   


    Object3d::PostDraw();

    //ObjectFBX::PreDraw(dxCommon_->GetCommandList());

    //object1->Draw(dxCommon->GetCommandList());

    //ObjectFBX::PostDraw();

    spriteCommon_->PreDraw();

    //sprite2->Draw();

    sprite->Draw();

    



    ParticleManager::PreDraw(dxCommon_->GetCommandList());
   
    ParticleManager::PostDraw();

    spriteCommon_->PostDraw();


}
