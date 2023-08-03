#include "MyGame.h"
#include "Quaternion.h"
#include "FbxLoader.h"
#include "ObjectFBX.h"
#include <fstream>




void MyGame::Initialize()
{
#pragma region ��ՃV�X�e���̏�����


    SNFramework::Initialize();

    

    scene_ = new GamePlayScene();

    scene_->Initialize();

    player_ = new Player();

    enemy_ = new Enemy();

    weakEnemy_ = new WeakEnemy();
    WeakEnemy::SetMyGame(this);
    WeakEnemy::SetPlayer(player_);


    postEffect = new PostEffect();
    //postEffect->SetTextureIndex(1);
    postEffect->Initialize(spriteCommon,"PostEffectTest");
    //postEffect->SetSize({ 500.0f,500.0f });

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
    modelWeakEnemy_ = Model::LoadFromOBJ("weakenemy");
    modelRail_ = Model::LoadFromOBJ("rail");

    object3d_1 = Object3d::Create();
    object3d_2 = Object3d::Create();
    object3d_3 = Object3d::Create();
    object3DPlayer_ = Object3d::Create();
    object3DEnemy_ = Object3d::Create();
    //object3DWeakEnemy_ = Object3d::Create();
    object3DRail_ = Object3d::Create();
    
    //3D�I�u�W�F�N�g��3D���f�����Ђ��t��
    object3d_1->SetModel(model_1);
    object3d_2->SetModel(model_2);
    object3d_3->SetModel(model_2);
    object3DPlayer_->SetModel(modelPlayer_);
    object3DEnemy_->SetModel(modelEnemy_);
    //object3DWeakEnemy_->SetModel(modelWeakEnemy_);
    object3DRail_->SetModel(modelRail_);
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
    object3DRail_->SetScale({ 500,100,100 });
    object3DRail_->SetPosition({ 0,20,-2000 });
    object3DRail_->SetRotation({ 0,90,0 });

    object3d_1->SetCamera(camera_);
    object3d_2->SetCamera(camera_);
    object3d_3->SetCamera(camera_);
    object3DPlayer_->SetCamera(camera_);
    object3DEnemy_->SetCamera(camera_);
    //object3DWeakEnemy_->SetCamera(camera_);
    object3DRail_->SetCamera(camera_);

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
   
    LoadPopEnemyData();
    

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
   

    delete postEffect;

    delete object3d_1;
    delete object3d_2;
    delete object3d_3;
    delete object3DRail_;
    delete object1;

    delete model_1;
    delete model_2;
    delete model1;
    
    SNFramework::Finalize();
    
#pragma endregion ��ՃV�X�e���̏I��
}

void MyGame::Update()
{
#pragma region ��ՃV�X�e���̍X�V
    SNFramework::Update();
#pragma endregion ��ՃV�X�e���̍X�V

#pragma region �ŏ��̃V�[���̍X�V

    
    if (player_->IsBoss())
    {
        postEffect->Initialize(spriteCommon, "Warning");
    }

    player_->Update();
    enemy_->Update();

    UpdateEnemyPopCommands();

        
    for (std::unique_ptr<WeakEnemy>& weakEnemy_ : _WeakEnemy) {

        weakEnemy_->Update();

    }

    for (std::unique_ptr<WeakEnemyBullet>& bullet : WeakEnemyBullets_) {
        bullet->Update();
    }

    WeakEnemyBullets_.remove_if(
        [](std::unique_ptr<WeakEnemyBullet>& bullet) { return bullet->IsDead(); });
    

    CheckAllCollisions();

    camera_->Update();
    sprite->Update();

    //sprite2->Update();

    postEffect->Update();

    object3d_1->Update();
    object3d_2->Update();
    object3d_3->Update();
    object3DRail_->Update();

    

    object1->Update();
    

    pm1_->Update();
    pm2_->Update();

    


#pragma endregion �ŏ��̃V�[���̍X�V
}

void MyGame::Draw()
{
    postEffect->PreDraw(dxCommon->GetCommandList());
    //spriteCommon->PreDraw();
    
    spriteCommon->PreDraw();
    //sprite->Draw();
    //sprite2->Draw();

    //sprite->Draw();

    imGui->Begin();


    

    spriteCommon->PostDraw();

    Object3d::PreDraw(dxCommon->GetCommandList());
    object3d_1->Draw();
    object3d_2->Draw();
    object3DRail_->Draw();
    //object3d_3->Draw();
    player_->Draw();
    enemy_->Draw();

    imGui->End();
    
    for (std::unique_ptr<WeakEnemy>& weakEnemy_ : _WeakEnemy) {
        weakEnemy_->Draw();
    }

    for (std::unique_ptr<WeakEnemyBullet>& bullet : WeakEnemyBullets_) {
        bullet->Draw();
    }
    

    Object3d::PostDraw();

    ObjectFBX::PreDraw(dxCommon->GetCommandList());

    //object1->Draw(dxCommon->GetCommandList());

    ObjectFBX::PostDraw();

    ParticleManager::PreDraw(dxCommon->GetCommandList());
    pm1_->Draw();
    //pm2_->Draw();
    ParticleManager::PostDraw();
    
    //spriteCommon->PostDraw();
    
    postEffect->PostDraw(dxCommon->GetCommandList());
   
    //�`��O����
    dxCommon->PreDraw();

#pragma region �ŏ��̃V�[���̕`��
   
    postEffect->Draw(dxCommon->GetCommandList());
    imGui->Draw();
#pragma endregion �ŏ��̃V�[���̕`��

    //�`��㏈��
    dxCommon->PostDraw();
}

void MyGame::CheckAllCollisions()
{
    //����Ώ�A,B�̍��W
    XMFLOAT3 posA, posB,posC{};
    // A,B�̍��W�̋����p
    XMFLOAT3 posAB,posAC;
    //����Ώ�A,B�̔��a
    float radiusA;
    float radiusB;
    float radiusC;
    float radiusAB;
    float radiusAC;

    //���@�e���X�g���擾
    const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();
    //�G�e���X�g���擾
    const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetEnemyBullets();

    const std::list<std::unique_ptr<WeakEnemyBullet>>& weakEnemyBullets = this->GetWeakEnemyBullets();

#pragma region ���@�ƓG�e�̓����蔻��
    //���ꂼ��̔��a
    radiusA = 1.0f;
    radiusB = 1.0f;
    radiusC = 2.0f;

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
        radiusAB = (radiusA + radiusB) * (radiusA + radiusB);

        //���Ƌ��̌�������
        if (radiusAB >= (posAB.x + posAB.y + posAB.z)) {
            //���L�����̏Փˎ��R�[���o�b�N�֐����Ăяo��
            player_->OnCollision();
            //�G�e�̏Փˎ��R�[���o�b�N�֐����Ăяo��gg
            bullet->OnCollision();
            

        }
    }

    //���@�ƑS�Ă̓G�e�̓����蔻��
    for (const std::unique_ptr<WeakEnemyBullet>& weakbullet : weakEnemyBullets) {
        //�G�e�̍��W
        posC = weakbullet->GetWorldPosition();
        //���WA,B�̋��������߂�
        posAC.x = (posC.x - posA.x) * (posC.x - posA.x);
        posAC.y = (posC.y - posA.y) * (posC.y - posA.y);
        posAC.z = (posC.z - posA.z) * (posC.z - posA.z);
        radiusAC = (radiusA + radiusC) * (radiusA + radiusC);

        //���Ƌ��̌�������
        if (radiusAC >= (posAC.x + posAC.y + posAC.z)) {
            //���L�����̏Փˎ��R�[���o�b�N�֐����Ăяo��
            player_->OnCollision();
            //�G�e�̏Փˎ��R�[���o�b�N�֐����Ăяo��
            weakbullet->OnCollision();
            
        }
    }

#pragma endregion

#pragma region ���e�ƓG�̓����蔻��
    //���ꂼ��̔��a
    radiusA = 5.0f;
    radiusB = 1.0f;
    radiusC = 5.0f;

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
        radiusAB = (radiusA + radiusB) * (radiusA + radiusB);

        //���Ƌ��̌�������
        if (radiusAB >= (posAB.x + posAB.y + posAB.z)) {
            //�G�L�����̏Փˎ��R�[���o�b�N�֐����Ăяo��
            enemy_->OnCollisionPlayer();
            //���@�e�̏Փˎ��R�[���o�b�N�֐����Ăяo��
            bullet->OnCollision();

            pm1_->ActiveZ(particle1_, { object3DEnemy_->GetPosition() }, { 0.0f ,0.0f,25.0f }, { 4.2f,4.2f,0.0f }, { 0.0f,0.001f,0.0f }, 10, { 3.0f, 0.0f });
            
        }
    }

    for (std::unique_ptr<WeakEnemy>& weakEnemy_ : _WeakEnemy) {
        posC = weakEnemy_->GetWorldPosition();


        //�G���G�ƑS�Ă̒e�̓����蔻��
        for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
            //�e�̍��W
            posB = bullet->GetWorldPosition();
            //���WA,B�̋��������߂�
            posAC.x = (posB.x - posC.x) * (posB.x - posC.x);
            posAC.y = (posB.y - posC.y) * (posB.y - posC.y);
            posAC.z = (posB.z - posC.z) * (posB.z - posC.z);
            radiusAC = (radiusA + radiusC) * (radiusA + radiusC);

            //���Ƌ��̌�������
            if (radiusAC >= (posAC.x + posAC.y + posAC.z)) {
                //�G�L�����̏Փˎ��R�[���o�b�N�֐����Ăяo��
                 //�e�X�V


                weakEnemy_->OnCollisionPlayer();


                //���@�e�̏Փˎ��R�[���o�b�N�֐����Ăяo��
                bullet->OnCollision();

                pm1_->ActiveZ(particle1_, { weakEnemy_->GetWorldPosition()}, {0.0f ,0.0f,25.0f}, {4.2f,4.2f,0.0f}, {0.0f,0.001f,0.0f}, 10, {10.0f, 0.0f});
            }
        }
    }

#pragma endregion
}

void MyGame::AddEnemyBullet(std::unique_ptr<WeakEnemyBullet> weakEnemyBullet)
{
    WeakEnemyBullets_.push_back(std::move(weakEnemyBullet));
}

void MyGame::WeakEnemy_(XMFLOAT3 trans)
{
    //object3DWeakEnemy_->SetPosition(trans);
    std::unique_ptr<WeakEnemy> newWeakEnemy = std::make_unique<WeakEnemy>();
    newWeakEnemy->Initialize(modelWeakEnemy_, trans, camera_);
    //newWeakEnemy.reset(weakEnemy_);
    _WeakEnemy.push_back(std::move(newWeakEnemy));
}

void MyGame::LoadPopEnemyData()
{
    //�t�@�C�����J��
    std::ifstream file;
    file.open("Resources/enemyPop.csv");
    assert(file.is_open());

    enemyPopCommands << file.rdbuf();

    file.close();
}

void MyGame::UpdateEnemyPopCommands()
{
    if (isWait_) {
        waitTimer_--;
        if (waitTimer_ <= 0) {
            isWait_ = false;
        }
        return;
    }

    std::string line;

    while (getline(enemyPopCommands, line)) {
        std::istringstream line_stream(line);

        std::string word;
        getline(line_stream, word, ',');

        if (word.find("//") == 0) {
            continue;
        }

        if (word.find("POP") == 0) {
            getline(line_stream, word, ',');
            float x = (float)std::atof(word.c_str());

            getline(line_stream, word, ',');
            float y = (float)std::atof(word.c_str());

            getline(line_stream, word, ',');
            float z = (float)std::atof(word.c_str());

            WeakEnemy_(XMFLOAT3(x,y,z));
        }

        else if (word.find("WAIT") == 0) {
            getline(line_stream, word, ',');

            int32_t waitTime = atoi(word.c_str());

            isWait_ = true;
            waitTimer_ = waitTime;
            

            break;
        }
    }
}







