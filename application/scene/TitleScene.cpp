#include "TitleScene.h"
//#include "GamePlayScene.h"

DirectXCommon* TitleScene::dxCommon_ = DirectXCommon::GetInstance();

SpriteCommon* TitleScene::spriteCommon_ = SpriteCommon::GetInstance();

Input* TitleScene::input_ = Input::GetInstance();

SceneManager* TitleScene::sceneManager_ = SceneManager::GetInstance();

void TitleScene::Initialize()
{
    
    spriteCommon_->LoadTexture(1, "white1x1.png");

    spriteCommon_->LoadTexture(0, "warning.png");

    spriteCommon_->LoadTexture(2, "white1280x720.png");

    spriteCommon_->LoadTexture(3, "gameover.png");

    spriteCommon_->LoadTexture(4, "gameclear.png");

    //spriteCommon_->LoadTexture(5, "warning.png");

    ////音声読み込み
    //Audio::GetInstance()->SoundLoadWave("Resources/fanfare.wav");
    ////音声再生
    ////audio->SoundPlayWave("Resources/fanfare.wav");

    //player_ = new Player();

    camera_ = new Camera();

    player_ = new Player();

    sprite = new Sprite();
    sprite->SetTextureIndex(0),
    sprite->Initialize(spriteCommon_, 0);

    sprite3 = new Sprite();
    sprite3->SetTextureIndex(2),
        sprite3->Initialize(spriteCommon_, 2);

    sprite4 = new Sprite();
    sprite4->SetTextureIndex(3),
        sprite4->Initialize(spriteCommon_, 3);

    sprite5 = new Sprite();
    sprite5->SetTextureIndex(4),
    sprite5->Initialize(spriteCommon_, 4);

    spriteFadeInOut_->SetTextureIndex(0);
    spriteFadeInOut_->Initialize(spriteCommon_, 0);
    spriteFadeInOut_->SetColor({ 0.0f,0.0f, 0.0f, easeFadeInOut_.start });

    modelPlayer_ = Model::LoadFromOBJ("player");

    object3DPlayer_ = Object3d::Create();
    object3DPlayer_->SetModel(modelPlayer_);
    object3DPlayer_->SetPosition({ 0.0f,20.0f,-60.0f });
    object3DPlayer_->SetScale({ 10.0f,10.0f,10.0f });
    object3DPlayer_->SetCamera(camera_);

    modelTitle_ = Model::LoadFromOBJ("neon");
    object3DTitle_ = Object3d::Create();
    object3DTitle_->SetModel(modelTitle_);
    object3DTitle_->SetRotation({ 270,0,0 });
    object3DTitle_->SetScale({ 10.0f,10.0f,10.0f });
    object3DTitle_->SetPosition({ 11.0f,30.0f,-60.0f });
    object3DTitle_->SetCamera(camera_);

    model_1 = Model::LoadFromOBJ("ground");
    model_2 = Model::LoadFromOBJ("skybox");
    object3d_1 = Object3d::Create();
    object3d_2 = Object3d::Create();
    object3d_1->SetModel(model_1);
    object3d_2->SetModel(model_2);
    object3d_1->SetPosition({ 0,-10,0 });
    object3d_1->SetScale({ 10.0f,10.0f,100.0f });
    object3d_2->SetScale({ 600.0f,600.0f,600.0f });
    object3d_1->SetCamera(camera_);
    object3d_2->SetCamera(camera_);

    camera_->SetTarget({ 0,17.0f,-20.0f });
    camera_->SetEye({ 0,20.0f,-12.0f });
    camera_->SetUp({ 0,20,0 });
    camera_->CameraMoveVector({ 0,20,0 });

    //player_->Initialize(modelPlayer_, object3DPlayer_, input_, camera_, sprite3, sprite4, sprite5);

    

    
}

void TitleScene::Finalize()
{
    delete sprite;

}

void TitleScene::Update()
{
    
   //7キーを押したら
    if (input_->TriggerKey(DIK_RETURN)) {
        isFadeInStart_ = true;
    }

    if (isFadeInStart_)
    {
        FadeIn({ 1.0f,1.0f,1.0f });//ゲームプレイ遷移時は黒くする
        if (spriteFadeInOut_->GetColor().w == easeFadeInOut_.end)
        {
            //シーン切り替え
            SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
        }
    }

    
    sprite->Update();
    camera_->Update();
    //player_->Update();
    object3DTitle_->Update();
    object3DPlayer_->Update();
    object3d_1->Update();
    object3d_2->Update();
    spriteFadeInOut_->Update();
    
}

void TitleScene::Draw()
{


    //postEffect->PreDraw(dxCommon_->GetCommandList());


    Object3d::PreDraw(dxCommon_->GetCommandList());
   
    //player_->Draw();
    object3DPlayer_->Draw();
    if (!player_->IsFadeInWhite())
    {
        object3d_1->Draw();
    }
    object3d_2->Draw();

    if (player_->IsFadeIn() == false)
    {
        object3DTitle_->Draw();
    }

    Object3d::PostDraw();

    //ObjectFBX::PreDraw(dxCommon_->GetCommandList());

    //object1->Draw(dxCommon->GetCommandList());

    //ObjectFBX::PostDraw();

    spriteCommon_->PreDraw();

    //sprite2->Draw();

    //sprite->Draw();
    spriteFadeInOut_->Draw();

    ParticleManager::PreDraw(dxCommon_->GetCommandList());
   
    ParticleManager::PostDraw();

    spriteCommon_->PostDraw();


}

void TitleScene::FadeIn(DirectX::XMFLOAT3 rgb)
{
    if (!isFadeIn_)
    {
        easeFadeInOut_.Standby(true);
        isFadeIn_ = true;
    }
    else
    {
        easeFadeInOut_.ease_out_quint();
        spriteFadeInOut_->SetColor({ rgb.x,rgb.y,rgb.z, easeFadeInOut_.num_X });//透明度だけ変える

    }
}

void TitleScene::UpdateChangeColor()
{
    //色を変えるスピード
    const float speedColor = 0.02f;

    if (isColorReverse_)
    {
        selectColor_.x -= speedColor;
        selectColor_.y -= speedColor;
        selectColor_.z -= speedColor;
    }

    else
    {
        selectColor_.x += speedColor;
        selectColor_.y += speedColor;
        selectColor_.z += speedColor;

    }

    const DirectX::XMFLOAT2 maxAndMinSpeedColor = { 0.9f,0.0f };//{max,min}

    if (selectColor_.x >= maxAndMinSpeedColor.x)
    {
        isColorReverse_ = true;
    }
    if (selectColor_.x <= maxAndMinSpeedColor.y)
    {
        isColorReverse_ = false;
    }
}
