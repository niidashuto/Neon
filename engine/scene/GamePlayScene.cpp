#include "GamePlayScene.h"


DirectXCommon* GamePlayScene::dxCommon_ = DirectXCommon::GetInstance();

SpriteCommon* GamePlayScene::spriteCommon_ = SpriteCommon::GetInstance();

void GamePlayScene::Initialize()
{
    ////spriteCommon->LoadTexture(0, "texture.png");
    ////spriteCommon->LoadTexture(1, "reimu.png");

    spriteCommon_->LoadTexture(1, "white1x1.png");

    spriteCommon_->LoadTexture(0, "background.png");

    //ParticleManager::StaticInitialize(DirectXCommon::GetInstance()->GetDevice());

    ////‰¹º“Ç‚Ýž‚Ý
    //Audio::GetInstance()->SoundLoadWave("Resources/fanfare.wav");
    ////‰¹ºÄ¶
    ////audio->SoundPlayWave("Resources/fanfare.wav");
}

void GamePlayScene::Finalize()
{
}

void GamePlayScene::Update()
{
}

void GamePlayScene::Draw()
{
}
