#include "TitleScene.h"


DirectXCommon* TitleScene::dxCommon_ = DirectXCommon::GetInstance();

SpriteCommon* TitleScene::spriteCommon_ = SpriteCommon::GetInstance();

Input* TitleScene::input_ = Input::GetInstance();

void TitleScene::Initialize()
{
    
    spriteCommon_->LoadTexture(0, "reimu.png");

    ////‰¹º“Ç‚Ýž‚Ý
    //Audio::GetInstance()->SoundLoadWave("Resources/fanfare.wav");
    ////‰¹ºÄ¶
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
