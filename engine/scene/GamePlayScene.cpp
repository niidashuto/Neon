#include "GamePlayScene.h"
#include "TitleScene.h"

DirectXCommon* GamePlayScene::dxCommon_ = DirectXCommon::GetInstance();

SpriteCommon* GamePlayScene::spriteCommon_ = SpriteCommon::GetInstance();

SceneManager* GamePlayScene::sceneManager_ = SceneManager::GetInstance();

Input* GamePlayScene::input_ = Input::GetInstance();

// �e�N�X�`���ԍ�
enum class TextureIndex {
    WARNING,
    WHITE1X1,
    WHITE1280X720,
    GAMEOVER,
    GAMECLEAR,
    W,
    D,
    S,
    A,
    DAMAGE,
    HP,
    
};

// �X�v���C�g�ԍ�
enum class SpriteIndex {
    SpriteNum,
};

struct SpriteDef {
    TextureIndex textureIndex;
    uint32_t hp;
};

void GamePlayScene::Initialize()
{
   

    spriteCommon_->LoadTexture(0, "warning.png");

    spriteCommon_->LoadTexture(1, "white1x1.png");

    spriteCommon_->LoadTexture(2, "white1280x720.png");

    spriteCommon_->LoadTexture(3, "gameover.png");

    spriteCommon_->LoadTexture(4, "gameclear.png");

    spriteCommon_->LoadTexture(5, "w.png");
    spriteCommon_->LoadTexture(6, "d.png");
    spriteCommon_->LoadTexture(7, "s.png");
    spriteCommon_->LoadTexture(8, "a.png");
    spriteCommon_->LoadTexture(9, "space.png");

    spriteCommon_->LoadTexture(10, "damage.png");

    spriteCommon_->LoadTexture(11, "hp.png");

    

    player_ = new Player();

    enemy_ = new Enemy();

    boss_ = new Boss();

    weakEnemy_ = new WeakEnemy();


    WeakEnemy::SetGamePlayScene(this);
    WeakEnemy::SetPlayer(player_);

    modelWeakEnemy_ = Model::LoadFromOBJ("weakenemy");

    if (!player_->IsBoss())
    {
        //postEffect->Initialize(spriteCommon_, "PostEffectTest");
    }

    camera_ = new Camera();

    sprite = new Sprite();

   /* std::array<Sprite*, (uint32_t)SpriteIndex::SpriteNum> sprites;

    sprites[(uint32_t)SpriteIndex::SpriteNum]->Initialize(spriteCommon_, (uint32_t)TextureIndex::WARNING);

    for (uint32_t i = 0; i < (uint32_t)SpriteIndex::SpriteNum; i++) {
        sprites[i]->Initialize(spriteCommon_, (uint32_t)TextureIndex[i])
    }*/

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

    sprite6 = new Sprite();
    sprite6->SetTextureIndex(5),
        sprite6->Initialize(spriteCommon_, 5);

    sprite6->SetPosition({ 1180.0f,32.0f });
    sprite6->SetSize({ 32.0f,32.0f });

    sprite7 = new Sprite();
    sprite7->SetTextureIndex(6),
        sprite7->Initialize(spriteCommon_, 6);

    sprite7->SetPosition({ 1212.0f,64.0f });
    sprite7->SetSize({ 32.0f,32.0f });

    sprite8 = new Sprite();
    sprite8->SetTextureIndex(7),
        sprite8->Initialize(spriteCommon_, 7);
    sprite8->SetSize({ 32.0f,32.0f });

    sprite8->SetPosition({ 1180.0f,64.0f });

    sprite9 = new Sprite();
    sprite9->SetTextureIndex(8),
        sprite9->Initialize(spriteCommon_, 8);

    sprite9->SetPosition({ 1148.0f,64.0f });
    sprite9->SetSize({ 32.0f,32.0f });

    sprite10 = new Sprite();
    sprite10->SetTextureIndex(9),
        sprite10->Initialize(spriteCommon_, 9);

    sprite10->SetPosition({ 1118.0f,112.0f });
    sprite10->SetSize({ 160.0f,32.0f });

    sprite11 = new Sprite();
    sprite11->SetTextureIndex(10);
    sprite11->Initialize(spriteCommon_, 10);

    spriteHp_ = new Sprite();
    spriteHp_->SetTextureIndex(11);
    spriteHp_->Initialize(spriteCommon_, 11);
    spriteHp_->SetPosition({ 10.0f,10.0f });

    model_1 = Model::LoadFromOBJ("ground");
    model_2 = Model::LoadFromOBJ("skybox");
    modelPlayer_ = Model::LoadFromOBJ("player");
    modelEnemy_ = Model::LoadFromOBJ("enemy");
    modelWeakEnemy_ = Model::LoadFromOBJ("enemy2g");
    modelBoss_ = Model::LoadFromOBJ("boss");
    modelRail_ = Model::LoadFromOBJ("rail");
    modelTitle_ = Model::LoadFromOBJ("neon");

    object3d_1 = Object3d::Create();
    object3d_2 = Object3d::Create();
    object3d_3 = Object3d::Create();
    object3DPlayer_ = Object3d::Create();
    object3DEnemy_ = Object3d::Create();
    object3DBoss_ = Object3d::Create();
    object3DRail_ = Object3d::Create();
    object3DTitle_ = Object3d::Create();

    //3D�I�u�W�F�N�g��3D���f�����Ђ��t��
    object3d_1->SetModel(model_1);
    object3d_2->SetModel(model_2);
    object3d_3->SetModel(model_2);
    object3DPlayer_->SetModel(modelPlayer_);
    object3DEnemy_->SetModel(modelEnemy_);
    object3DBoss_->SetModel(modelBoss_);
    object3DRail_->SetModel(modelRail_);
    object3DTitle_->SetModel(modelTitle_);
    //3D�I�u�W�F�N�g�̈ʒu���w��
    object3d_3->SetPosition({ +5,0,+5 });
    //3D�I�u�W�F�N�g�̃X�P�[�����w��
    object3d_1->SetPosition({ 0,-10,0 });
    object3d_1->SetScale({ 10.0f,10.0f,100.0f });
    object3d_2->SetScale({ 600.0f,600.0f,600.0f });
    object3d_3->SetScale({ 10.0f,10.0f,10.0f });
    object3DPlayer_->SetScale({ 10.0f,10.0f,10.0f });
    object3DEnemy_->SetScale({ 15.0f,15.0f,15.0f });
    object3DBoss_->SetScale({ 40.0f,40.0f,40.0f });
    object3DRail_->SetScale({ 500,100,100 });
    object3DRail_->SetPosition({ 0,20,-2000 });
    object3DRail_->SetRotation({ 0,90,0 });
    object3DTitle_->SetRotation({ 270,0,0 });
    object3DTitle_->SetScale({ 10.0f,10.0f,10.0f });
    object3DTitle_->SetPosition({ 11.0f,30.0f,-60.0f });
    

    object3d_1->SetCamera(camera_);
    object3d_2->SetCamera(camera_);
    object3d_3->SetCamera(camera_);
    object3DPlayer_->SetCamera(camera_);
    object3DEnemy_->SetCamera(camera_);
    object3DBoss_->SetCamera(camera_);
    object3DRail_->SetCamera(camera_);
    object3DTitle_->SetCamera(camera_);
    

    particle1_ = Particle::LoadFromParticleTexture("particle.png");
    pm1_ = ParticleManager::Create();
    pm1_->SetParticleModel(particle1_);
    pm1_->SetCamera(camera_);

    particle2_ = Particle::LoadFromParticleTexture("particle6.png");
    pm2_ = ParticleManager::Create();
    pm2_->SetParticleModel(particle2_);
    pm2_->SetCamera(camera_);

    model1 = FbxLoader::GetInstance()->LoadModelFromFile("cube");

    ObjectFBX::SetDevice(dxCommon_->GetDevice());

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

    player_->Initialize(modelPlayer_, object3DPlayer_, input_, camera_, sprite3,sprite4, sprite5,spriteHp_);
    enemy_->Initialize(modelEnemy_, object3DEnemy_, camera_);
    enemy_->SetPlayer(player_);

    boss_->Initialize(modelBoss_, object3DBoss_, camera_, sprite5);
    boss_->SetPlayer(player_);

    LoadPopEnemyData();
}

void GamePlayScene::Finalize()
{
    delete sprite;

    delete postEffect;

    delete object3d_1;
    delete object3d_2;
    delete object3d_3;
    delete object3DRail_;
    delete object1;

    delete model_1;
    delete model_2;
    delete model1;

}

void GamePlayScene::Update()
{
    if (isFadeOutStart_)
    {
        FadeOut({ 1.0f,1.0f,1.0f });//�Q�[���v���C�J�ڎ��͍�������
    }

    //�{�X�����񂾂�
    if (boss_->IsDead())
    {
        FadeIn({ 1.0f,1.0f,1.0f });
    }
    //�v���C���[�����񂾂�
    if (player_->IsDead())
    {
        FadeIn({ 1.0f,1.0f,1.0f });
        FadeInGameOver({ 1.0f,1.0f,1.0f });
        isLimit_ = true;
    }

    if (player_->IsDead()||boss_->IsGameClear()) {
        if (input_->TriggerKey(DIK_SPACE))
        {
            SceneManager::GetInstance()->ChangeScene("TITLE");
        }
    }
    
    if (!player_->IsDead() || !boss_->IsDead())
    {
        player_->Update();
    }
    enemy_->Update();
    boss_->Update();

    XMFLOAT3 move = object3DPlayer_->GetPosition();
    XMFLOAT3 rot = object3DPlayer_->GetRotation();
    XMFLOAT3 scale = object3DPlayer_->GetScale();

    if (enemy_->IsDead())
    {
        UpdateEnemyPopCommands();

        FadeInWhite({ 1.0f,1.0f,1.0f });



        if (fadeInWhite_)
        {
            fadein_timer -= 1.0f;
            scaleSmaller_ = true;
            if (scaleSmaller_ && !isEndScale_)
            {
                scale.z -= 0.5f;
                scale.x -= 0.5f;
                scale.y += 0.5f;
                if (scale.z <= 0)
                {
                    scale.z = max(scale.z, 10);
                    scale.z = min(scale.z, 0);

                    scale.x = max(scale.x, 10);
                    scale.x = min(scale.x, 0);

                    scale.y = max(scale.y, 10);
                    scale.y = min(scale.y, 0);
                    isEndScale_ = true;
                }

            }
            if (fadein_timer <= 0.0f)
            {
                FadeOutWhite({ 1.0f,1.0f,1.0f });
                if (!changeStage_)
                {
                    transition_ = false;
                    move.x = 0.0f;
                    move.y = 20.0f;
                    move.z = -680.0f;
                    camera_->SetTarget({ 0,0,0.0f });
                    camera_->SetEye({ 0,0,8.0f });
                    camera_->SetUp({ 0,20,0 });
                    camera_->CameraMoveVector({ 0,20,-620 });
                    //rot.y = 0;
                }

                scaleSmaller_ = false;


            }

        }
        if (!scaleSmaller_)
        {
            default_timer -= 1.0f;
            if (default_timer <= 0.0f)
            {
                changeStage_ = true;
                if (!player_->IsGameOver())
                {
                scale.z += 0.5f;
                scale.x += 0.5f;
                scale.y -= 0.5f;
                

                scale.z = max(scale.z, 10);
                scale.z = min(scale.z, 10);

                scale.x = max(scale.x, 0);
                scale.x = min(scale.x, 10);

                scale.y = max(scale.y, 10);
                scale.y = min(scale.y, 10);

                }

                //isStart_ = true;
            }
        }

        object3DPlayer_->SetPosition(move);
        object3DPlayer_->SetRotation(rot);
        object3DPlayer_->SetScale(scale);
    }

    for (std::unique_ptr<WeakEnemy>& weakEnemy_ : _WeakEnemy) {

        weakEnemy_->Update();

    }

    for (std::unique_ptr<WeakEnemyBullet>& bullet : WeakEnemyBullets_) {
        bullet->Update();
    }

    WeakEnemyBullets_.remove_if(
        [](std::unique_ptr<WeakEnemyBullet>& bullet) { return bullet->IsDead(); });

    if (player_->IsPlayerExtinction()&&!isEnd_)
    {
        pm1_->ActiveZ(particle1_, { object3DPlayer_->GetPosition() }, { 0.0f ,0.0f,25.0f }, { 4.2f,4.2f,0.0f }, { 0.0f,0.001f,0.0f }, 40, { 4.0f, 0.0f });
        isEnd_ = true;
    }


    CheckAllCollisions();

    camera_->Update();
    sprite->Update();
    sprite3->Update();
    sprite4->Update();
    sprite5->Update();
    sprite6->Update();
    sprite7->Update();
    sprite8->Update();
    sprite9->Update();
    sprite10->Update();
    sprite11->Update();
    spriteHp_->Update();

    postEffect->Update();

    object3d_1->Update();
    object3d_2->Update();
    object3d_3->Update();
    object3DRail_->Update();
    object3DTitle_->Update();
    //object3DW_->Update();


    object1->Update();


    pm1_->Update();
    pm2_->Update();
}

void GamePlayScene::Draw()
{

    Object3d::PreDraw(dxCommon_->GetCommandList());
    if (!scaleSmaller_&&player_->GetWorldPosition().z>=-670.0f)
    {
        object3d_1->Draw();
    }
    object3d_2->Draw();
    object3DRail_->Draw();
    
    player_->Draw();
   
    enemy_->Draw();
    

    if (player_->IsBoss())
    {
        boss_->Draw();
    }


    for (std::unique_ptr<WeakEnemy>& weakEnemy_ : _WeakEnemy) {
        weakEnemy_->Draw();
    }

    for (std::unique_ptr<WeakEnemyBullet>& bullet : WeakEnemyBullets_) {

        bullet->Draw();

    }


    Object3d::PostDraw();

    ObjectFBX::PreDraw(dxCommon_->GetCommandList());

    ObjectFBX::PostDraw();

    spriteCommon_->PreDraw();

    if (player_->IsFadeIn() == false)
    {
        if (player_->IsFadeInWhite() == false)
        {
            sprite6->Draw();
            sprite7->Draw();
            sprite8->Draw();
            sprite9->Draw();
            sprite10->Draw();
        }
    }

    spriteHp_->Draw();

    sprite->Draw();

    sprite3->Draw();

    sprite4->Draw();

    sprite5->Draw();


    if (isDamage_)
    {
        sprite11->Draw();
    }
    

    ParticleManager::PreDraw(dxCommon_->GetCommandList());
    pm1_->Draw();
    player_->DrawParticle();
    
    ParticleManager::PostDraw();

    spriteCommon_->PostDraw();

    
}

void GamePlayScene::CheckAllCollisions()
{
    //����Ώ�A,B�̍��W
    XMFLOAT3 posA, posB, posC, posD{};
    // A,B�̍��W�̋����p
    XMFLOAT3 posAB, posAC, posAD;
    //����Ώ�A,B�̔��a
    float radiusA;
    float radiusB;
    float radiusC;
    float radiusD;
    float radiusAB;
    float radiusAC;
    float radiusAD;

    //���@�e���X�g���擾
    const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();
    //�G�e���X�g���擾
    const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetEnemyBullets();

    const std::list<std::unique_ptr<BossBullet>>& bossBullets = boss_->GetEnemyBullets();

    const std::list<std::unique_ptr<WeakEnemyBullet>>& weakEnemyBullets = this->GetWeakEnemyBullets();

#pragma region ���@�ƓG�e�̓����蔻��
    //���ꂼ��̔��a
    radiusA = 1.0f;
    radiusB = 1.0f;
    radiusC = 2.0f;
    radiusD = 4.0f;

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

            isDamage_ = true;
        }

        if (isDamage_)
        {
            damage_timer -= 1.0f;
            if (damage_timer <= 0.0f)
            {
                isDamage_ = false;
                damage_timer = 10.0f;
            }

        }
       
    }

    //���@�ƑS�Ă̓G�e�̓����蔻��
    for (const std::unique_ptr<BossBullet>& bossbullet : bossBullets) {
        //�G�e�̍��W
        posD = bossbullet->GetWorldPosition();
        //���WA,B�̋��������߂�
        posAD.x = (posD.x - posA.x) * (posD.x - posA.x);
        posAD.y = (posD.y - posA.y) * (posD.y - posA.y);
        posAD.z = (posD.z - posA.z) * (posD.z - posA.z);
        radiusAD = (radiusA + radiusD) * (radiusA + radiusD);

        //���Ƌ��̌�������
        if (radiusAD >= (posAD.x + posAD.y + posAD.z)) {
            //���L�����̏Փˎ��R�[���o�b�N�֐����Ăяo��
            player_->OnCollision();
            //�G�e�̏Փˎ��R�[���o�b�N�֐����Ăяo��gg
            bossbullet->OnCollision();


        }
    }

#pragma endregion

#pragma region ���e�ƓG�̓����蔻��
    //���ꂼ��̔��a
    radiusA = 5.0f;
    radiusB = 1.0f;
    radiusC = 5.0f;
    radiusD = 20.0f;

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

                pm1_->ActiveZ(particle1_, { weakEnemy_->GetWorldPosition() }, { 0.0f ,0.0f,25.0f }, { 4.2f,4.2f,0.0f }, { 0.0f,0.001f,0.0f }, 10, { 10.0f, 0.0f });
            }
        }
    }
    //�G�̍��W
    posA = boss_->GetWorldPosition();

    //�G�ƑS�Ă̒e�̓����蔻��
    for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
        //�e�̍��W
        posD = bullet->GetWorldPosition();
        //���WA,B�̋��������߂�
        posAD.x = (posD.x - posA.x) * (posD.x - posA.x);
        posAD.y = (posD.y - posA.y) * (posD.y - posA.y);
        posAD.z = (posD.z - posA.z) * (posD.z - posA.z);
        radiusAD = (radiusA + radiusD) * (radiusA + radiusD);

        //���Ƌ��̌�������
        if (radiusAD >= (posAD.x + posAD.y + posAD.z)) {
            //�G�L�����̏Փˎ��R�[���o�b�N�֐����Ăяo��
            boss_->OnCollisionPlayer();
            //���@�e�̏Փˎ��R�[���o�b�N�֐����Ăяo��
            bullet->OnCollision();

            pm1_->ActiveZ(particle1_, { object3DBoss_->GetPosition() }, { 0.0f ,0.0f,100.0f }, { 4.2f,4.2f,0.0f }, { 0.0f,0.001f,0.0f }, 20, { 50.0f, 0.0f });

        }
    }

#pragma endregion
}

void GamePlayScene::AddEnemyBullet(std::unique_ptr<WeakEnemyBullet> weakEnemyBullet)
{
    WeakEnemyBullets_.push_back(std::move(weakEnemyBullet));
}

void GamePlayScene::WeakEnemy_(XMFLOAT3 trans)
{
    std::unique_ptr<WeakEnemy> newWeakEnemy = std::make_unique<WeakEnemy>();
    newWeakEnemy->Initialize(modelWeakEnemy_, trans, camera_);
    _WeakEnemy.push_back(std::move(newWeakEnemy));
}

void GamePlayScene::LoadPopEnemyData()
{

    //�t�@�C�����J��
    std::ifstream file;
    file.open("Resources/enemyPop.csv");
    assert(file.is_open());

    enemyPopCommands << file.rdbuf();

    file.close();
}

void GamePlayScene::UpdateEnemyPopCommands()
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

            WeakEnemy_(XMFLOAT3(x, y, z));
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

void GamePlayScene::FadeOut(XMFLOAT3 rgb)
{
    if (!isFadeOut_)
    {
        easeFadeOut_.Standby(true);
        isFadeOut_ = true;
    }
    else
    {
        easeFadeOut_.ease_out_quint();
        sprite->SetColor({ rgb.x,rgb.y,rgb.z, easeFadeOut_.num_X });//�����x�����ς���

    }
}

void GamePlayScene::FadeIn(XMFLOAT3 rgb)
{
    if (!isFadeIn_)
    {
        easeFadeIn_.Standby(true);
        isFadeIn_ = true;
    }
    else
    {
        easeFadeIn_.ease_out_quint();
        sprite->SetColor({ rgb.x,rgb.y,rgb.z, easeFadeIn_.num_X });//�����x�����ς���

    }
    
}

void GamePlayScene::FadeInGameOver(XMFLOAT3 rgb)
{
    if (!isFadeInGameOver_)
    {
        easeFadeInGameOver_.Standby(true);
        isFadeInGameOver_ = true;
    }
    else
    {
        easeFadeInGameOver_.ease_out_quint();
        sprite4->SetColor({ rgb.x,rgb.y,rgb.z, easeFadeInGameOver_.num_X });//�����x�����ς���

    }
}

void GamePlayScene::FadeInWhite(XMFLOAT3 rgb)
{
    if (!fadeInWhite_)
    {
        easeFadeInWhite_.Standby(true);
        fadeInWhite_ = true;
    }
    else
    {
        easeFadeInWhite_.ease_out_quint();
        sprite3->SetColor({ rgb.x,rgb.y,rgb.z, easeFadeInWhite_.num_X });
    }
}

void GamePlayScene::FadeOutWhite(XMFLOAT3 rgb)
{
    if (!fadeOutWhite_)
    {
        easeFadeOutWhite_.Standby(true);
        fadeOutWhite_ = true;
    }
    else
    {
        easeFadeOutWhite_.ease_out_quint();
        sprite3->SetColor({ rgb.x,rgb.y,rgb.z, easeFadeOutWhite_.num_X });
    }
}

void GamePlayScene::UpdateChangeColor()
{
    //�F��ς���X�s�[�h
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

    const XMFLOAT2 maxAndMinSpeedColor = { 0.9f,0.0f };//{max,min}

    if (selectColor_.x >= maxAndMinSpeedColor.x)
    {
        isColorReverse_ = true;
    }
    if (selectColor_.x <= maxAndMinSpeedColor.y)
    {
        isColorReverse_ = false;
    }
}
