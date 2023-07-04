#include "MyGame.h"
#include "Quaternion.h"
#include "FbxLoader.h"
#include "ObjectFBX.h"


void MyGame::Initialize()
{
#pragma region ��ՃV�X�e���̏�����


    SNFramework::Initialize();

    //spriteCommon->LoadTexture(0, "texture.png");
    //spriteCommon->LoadTexture(1, "reimu.png");

    //scene_ = new GamePlayScene();

    //scene_->Initialize();

    player_ = new Player();

    enemy_ = new Enemy();

    spriteCommon->LoadTexture(1, "white1x1.png");

    spriteCommon->LoadTexture(0, "background.png");

   
    

    postEffect = new PostEffect();
    postEffect->SetTextureIndex(1);
    postEffect->Initialize(spriteCommon,1);
    postEffect->SetSize({ 500.0f,500.0f });
    
    

    Object3d::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height);

    

    ParticleManager::StaticInitialize(dxCommon->GetDevice());

    //�����ǂݍ���
    //audio->SoundLoadWave("Resources/fanfare.wav");
    //�����Đ�
    //audio->SoundPlayWave("Resources/fanfare.wav");


#pragma endregion ��ՃV�X�e���̏�����

#pragma region �ŏ��̃V�[����������

    camera_ = new Camera();

    imGui = new ImGuiManager();
    imGui->Initialize(winApp, dxCommon);

    
    //sprite = new Sprite();
    //sprite->SetTextureIndex(0);
    //sprite->Initialize(spriteCommon, 1);

    sprite = new Sprite();
    sprite->SetTextureIndex(0),
    sprite->Initialize(spriteCommon, 0);

    //sprite2 = new Sprite();
    //sprite2->SetTextureIndex(0);
    //sprite2->Initialize(spriteCommon, 0);

    //sprite2->SetPosition({ 800,0 });
    model_1 = Model::LoadFromOBJ("ground");
    model_2 = Model::LoadFromOBJ("skybox");
    modelPlayer_ = Model::LoadFromOBJ("player");
    modelEnemy_ = Model::LoadFromOBJ("enemy");

    object3d_1 = Object3d::Create();
    object3d_2 = Object3d::Create();
    object3d_3 = Object3d::Create();
    object3DPlayer_ = Object3d::Create();
    object3DEnemy_ = Object3d::Create();
    
    //3D�I�u�W�F�N�g��3D���f�����Ђ��t��
    object3d_1->SetModel(model_1);
    object3d_2->SetModel(model_2);
    object3d_3->SetModel(model_2);
    object3DPlayer_->SetModel(modelPlayer_);
    object3DEnemy_->SetModel(modelEnemy_);
    //3D�I�u�W�F�N�g�̈ʒu���w��
    //object3d_2->SetPosition({ -5,0,-5 });
    object3d_3->SetPosition({ +5,0,+5 });
    //3D�I�u�W�F�N�g�̃X�P�[�����w��
    object3d_1->SetPosition({ 0,-10,0 });
    object3d_1->SetScale({ 10.0f,10.0f,100.0f });
    //object3d_1->SetRotation({ 10.0f,0.0f,0.0f });
    object3d_2->SetScale({ 600.0f,600.0f,600.0f });
    object3d_3->SetScale({ 10.0f,10.0f,10.0f });
    object3DPlayer_->SetScale({ 10.0f,10.0f,10.0f });
    object3DEnemy_->SetScale({ 15.0f,15.0f,15.0f });


    object3d_1->SetCamera(camera_);
    object3d_2->SetCamera(camera_);
    object3d_3->SetCamera(camera_);
    object3DPlayer_->SetCamera(camera_);
    object3DEnemy_->SetCamera(camera_);

    particle1_ = Particle::LoadFromParticleTexture("particle.png");
    pm1_ = ParticleManager::Create();
    pm1_->SetParticleModel(particle1_);
    pm1_->SetCamera(camera_);

    particle2_ = Particle::LoadFromParticleTexture("particle6.png");
    pm2_ = ParticleManager::Create();
    pm2_->SetParticleModel(particle2_);
    pm2_->SetCamera(camera_);

    model1 = FbxLoader::GetInstance()->LoadModelFromFile("cube");

    ObjectFBX::SetDevice(dxCommon->GetDevice());

    ObjectFBX::SetCamera(camera_);

    ObjectFBX::CreateGraphicsPipeline();

    object1 = new ObjectFBX;
    object1->Initialize();
    object1->SetModel(model1);
    object1->SetPosition({ 0,30,-100 });
    object1->SetRotation({ 0,90,0 });
    object1->SetScale({ 0.1f,0.1f,0.1f });
    camera_->SetTarget({ 0,0,0 });
    camera_->SetEye({ 0,0,8.0f });
    camera_->SetUp({ 0,20,0 });
    camera_->CameraMoveVector({ 0,20,0 });
    //camera_->SetEye({ 0,0,0 });
    //object1->PlayAnimation();

    player_->Initialize(modelPlayer_, object3DPlayer_, input, camera_);
    enemy_->Initialize(modelEnemy_, object3DEnemy_, camera_);
    enemy_->SetPlayer(player_);

#pragma endregion �ŏ��̃V�[����������
}

void MyGame::Finalize()
{
#pragma region �ŏ��̃V�[���̏I��
    delete sprite;
    imGui->Finalize();
    delete imGui;

#pragma endregion �ŏ��̃V�[���̏I��

#pragma region ��ՃV�X�e���̏I��
    //�X�v���C�g���ʕ����
    delete spriteCommon;
    //���͉��
    delete input;
    //DirectX���
    delete dxCommon;

    delete postEffect;

    delete object3d_1;
    delete object3d_2;
    delete object3d_3;
    delete object1;

    delete model_1;
    delete model_2;
    delete model1;
    
    SNFramework::Finalize();
    delete audio;
    //WindowsAPI���
    delete winApp;
#pragma endregion ��ՃV�X�e���̏I��
}

void MyGame::Update()
{
#pragma region ��ՃV�X�e���̍X�V
    SNFramework::Update();
#pragma endregion ��ՃV�X�e���̍X�V

#pragma region �ŏ��̃V�[���̍X�V

    pm1_->Active(particle1_, 100.0f, 0.2f, 0.001f, 5, { 6.0f, 0.0f });
    pm2_->Active(particle2_, 100.0f, 0.2f, 0.001f, 5, { 6.0f, 0.0f });

    player_->Update();
    enemy_->Update();

    CheckAllCollisions();

    camera_->Update();
    sprite->Update();

    //sprite2->Update();

    postEffect->Update();

    object3d_1->Update();
    object3d_2->Update();
    object3d_3->Update();
    

    

    object1->Update();
    

    pm1_->Update();
    pm2_->Update();

    imGui->Begin();  


    imGui->End();


#pragma endregion �ŏ��̃V�[���̍X�V
}

void MyGame::Draw()
{
    postEffect->PreDrawScene(dxCommon->GetCommandList());
    //spriteCommon->PreDraw();
    
    //postEffect->Draw(dxCommon->GetCommandList());
    //spriteCommon->PostDraw();
    postEffect->PostDrawScene(dxCommon->GetCommandList());
   
    //�`��O����
    dxCommon->PreDraw();

#pragma region �ŏ��̃V�[���̕`��
    spriteCommon->PreDraw();
    //sprite->Draw();
    //sprite2->Draw();
    
    //sprite->Draw();
    

    
    spriteCommon->PostDraw();

    Object3d::PreDraw(dxCommon->GetCommandList());
    object3d_1->Draw();
    object3d_2->Draw();
    //object3d_3->Draw();
    object3DPlayer_->Draw();
    object3DEnemy_->Draw();

    Object3d::PostDraw();

    ObjectFBX::PreDraw(dxCommon->GetCommandList());

    //object1->Draw(dxCommon->GetCommandList());

    ObjectFBX::PostDraw();

    ParticleManager::PreDraw(dxCommon->GetCommandList());
    //pm1_->Draw();
    //pm2_->Draw();
    ParticleManager::PostDraw();

    imGui->Draw();
#pragma endregion �ŏ��̃V�[���̕`��

    //�`��㏈��
    dxCommon->PostDraw();
}

void MyGame::CheckAllCollisions()
{
    //����Ώ�A,B�̍��W
    XMFLOAT3 posA, posB;
    // A,B�̍��W�̋����p
    XMFLOAT3 posAB;
    //����Ώ�A,B�̔��a
    float radiusA;
    float radiusB;
    float radiiusAB;

    //���@�e���X�g���擾
    const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();
    //�G�e���X�g���擾
    const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetEnemyBullets();

#pragma region ���@�ƓG�e�̓����蔻��
    //���ꂼ��̔��a
    radiusA = 1.0f;
    radiusB = 1.0f;

    //���@�̍��W
    posA = player_->GetWorldPosition();

    //���@�ƑS�Ă̓G�e�̓����蔻��
    for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
        //�G�e�̍��W
        posB = bullet->GetWorldPosition();
        //���WA,B�̋��������߂�
        posAB.x = (posB.x - posA.x) * (posB.x - posA.x);
        posAB.y = (posB.y - posA.y) * (posB.y - posA.y);
        posAB.z = (posB.z - posA.z) * (posB.z - posA.z);
        radiiusAB = (radiusA + radiusB) * (radiusA + radiusB);

        //���Ƌ��̌�������
        if (radiiusAB >= (posAB.x + posAB.y + posAB.z)) {
            //���L�����̏Փˎ��R�[���o�b�N�֐����Ăяo��
            player_->OnCollision();
            //�G�e�̏Փˎ��R�[���o�b�N�֐����Ăяo��
            bullet->OnCollision();
        }
    }

#pragma endregion

#pragma region ���e�ƓG�̓����蔻��
    //���ꂼ��̔��a
    radiusA = 5.0f;
    radiusB = 1.0f;

    //�G�̍��W
    posA = enemy_->GetWorldPosition();

    //�G�ƑS�Ă̒e�̓����蔻��
    for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
        //�e�̍��W
        posB = bullet->GetWorldPosition();
        //���WA,B�̋��������߂�
        posAB.x = (posB.x - posA.x) * (posB.x - posA.x);
        posAB.y = (posB.y - posA.y) * (posB.y - posA.y);
        posAB.z = (posB.z - posA.z) * (posB.z - posA.z);
        radiiusAB = (radiusA + radiusB) * (radiusA + radiusB);

        //���Ƌ��̌�������
        if (radiiusAB >= (posAB.x + posAB.y + posAB.z)) {
            //�G�L�����̏Փˎ��R�[���o�b�N�֐����Ăяo��
            enemy_->OnCollisionPlayer();
            //���@�e�̏Փˎ��R�[���o�b�N�֐����Ăяo��
            bullet->OnCollision();
        }
    }
#pragma endregion
}
