#include "GamePlayScene.h"


DirectXCommon* GamePlayScene::dxCommon_ = DirectXCommon::GetInstance();

SpriteCommon* GamePlayScene::spriteCommon_ = SpriteCommon::GetInstance();

Input* GamePlayScene::input_ = Input::GetInstance();

void GamePlayScene::Initialize()
{
    ////spriteCommon->LoadTexture(0, "texture.png");
    ////spriteCommon->LoadTexture(1, "reimu.png");

    spriteCommon_->LoadTexture(1, "white1x1.png");

    spriteCommon_->LoadTexture(0, "warning.png");

    spriteCommon_->LoadTexture(2, "white1280x720.png");

    spriteCommon_->LoadTexture(3, "gameover.png");

    spriteCommon_->LoadTexture(4, "gameclear.png");

    //spriteCommon_->LoadTexture(5, "RankS.png");

    //ParticleManager::StaticInitialize(DirectXCommon::GetInstance()->GetDevice());

    ////音声読み込み
    //Audio::GetInstance()->SoundLoadWave("Resources/fanfare.wav");
    ////音声再生
    ////audio->SoundPlayWave("Resources/fanfare.wav");

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

    //imGui = new ImGuiManager();
    //imGui->Initialize(winApp_, dxCommon_);


    //sprite = new Sprite();
    //sprite->SetTextureIndex(0);
    //sprite->Initialize(spriteCommon, 1);

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

    model_1 = Model::LoadFromOBJ("ground");
    model_2 = Model::LoadFromOBJ("skybox");
    modelPlayer_ = Model::LoadFromOBJ("player");
    modelEnemy_ = Model::LoadFromOBJ("enemy");
    modelWeakEnemy_ = Model::LoadFromOBJ("weakenemy");
    modelBoss_ = Model::LoadFromOBJ("boss");
    modelRail_ = Model::LoadFromOBJ("rail");
    modelTitle_ = Model::LoadFromOBJ("neon");

    object3d_1 = Object3d::Create();
    object3d_2 = Object3d::Create();
    object3d_3 = Object3d::Create();
    object3DPlayer_ = Object3d::Create();
    object3DEnemy_ = Object3d::Create();
    object3DBoss_ = Object3d::Create();
    //object3DWeakEnemy_ = Object3d::Create();
    object3DRail_ = Object3d::Create();
    object3DTitle_ = Object3d::Create();

    //3Dオブジェクトと3Dモデルをひも付け
    object3d_1->SetModel(model_1);
    object3d_2->SetModel(model_2);
    object3d_3->SetModel(model_2);
    object3DPlayer_->SetModel(modelPlayer_);
    object3DEnemy_->SetModel(modelEnemy_);
    object3DBoss_->SetModel(modelBoss_);
    //object3DWeakEnemy_->SetModel(modelWeakEnemy_);
    object3DRail_->SetModel(modelRail_);
    object3DTitle_->SetModel(modelTitle_);
    //3Dオブジェクトの位置を指定
    //object3d_2->SetPosition({ -5,0,-5 });
    object3d_3->SetPosition({ +5,0,+5 });
    //3Dオブジェクトのスケールを指定
    object3d_1->SetPosition({ 0,-10,0 });
    object3d_1->SetScale({ 10.0f,10.0f,100.0f });
    //object3d_1->SetRotation({ 10.0f,0.0f,0.0f });
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
    //object3DWeakEnemy_->SetCamera(camera_);
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
    camera_->SetTarget({ 0,17.0f,-20.0f });
    camera_->SetEye({ 0,20.0f,-12.0f });
    camera_->SetUp({ 0,20,0 });
    camera_->CameraMoveVector({ 0,20,0 });
    //camera_->SetEye({ 0,0,0 });
    //object1->PlayAnimation();

    player_->Initialize(modelPlayer_, object3DPlayer_, input_, camera_, sprite, sprite3, sprite4, sprite5);
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
    player_->Update();
    enemy_->Update();
    boss_->Update();

    if (enemy_->IsDead())
    {
        UpdateEnemyPopCommands();
    }

    for (std::unique_ptr<WeakEnemy>& weakEnemy_ : _WeakEnemy) {

        weakEnemy_->Update();

    }

    for (std::unique_ptr<WeakEnemyBullet>& bullet : WeakEnemyBullets_) {
        bullet->Update();
    }

    WeakEnemyBullets_.remove_if(
        [](std::unique_ptr<WeakEnemyBullet>& bullet) { return bullet->IsDead(); });

    if (player_->IsPlayerExtinction())
    {
        pm1_->ActiveZ(particle1_, { object3DPlayer_->GetPosition() }, { 0.0f ,0.0f,25.0f }, { 4.2f,4.2f,0.0f }, { 0.0f,0.001f,0.0f }, 10, { 3.0f, 0.0f });

    }


    CheckAllCollisions();

    camera_->Update();
    sprite->Update();
    sprite3->Update();
    sprite4->Update();
    sprite5->Update();

    postEffect->Update();

    object3d_1->Update();
    object3d_2->Update();
    object3d_3->Update();
    object3DRail_->Update();
    object3DTitle_->Update();



    object1->Update();


    pm1_->Update();
    pm2_->Update();
}

void GamePlayScene::Draw()
{

    
    //postEffect->PreDraw(dxCommon_->GetCommandList());


    Object3d::PreDraw(dxCommon_->GetCommandList());
    if (!player_->IsFadeInWhite())
    {
        object3d_1->Draw();
    }
    object3d_2->Draw();
    object3DRail_->Draw();
    if (player_->IsFadeIn() == false)
    {
        object3DTitle_->Draw();
    }
    //object3d_3->Draw();
    player_->Draw();
    if (player_->IsFadeIn() == true)
    {
        if (player_->IsFadeInWhite() == false)
        {

            enemy_->Draw();
        }
    }

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

    //object1->Draw(dxCommon->GetCommandList());

    ObjectFBX::PostDraw();

    spriteCommon_->PreDraw();

    //sprite2->Draw();

    sprite->Draw();

    sprite3->Draw();

    sprite4->Draw();

    sprite5->Draw();

    

    ParticleManager::PreDraw(dxCommon_->GetCommandList());
    pm1_->Draw();
    //pm2_->Draw();
    ParticleManager::PostDraw();

    //spriteCommon->PostDraw();

    //postEffect->PostDraw(dxCommon_->GetCommandList());

    spriteCommon_->PostDraw();

    
}

void GamePlayScene::CheckAllCollisions()
{
    //判定対象A,Bの座標
    XMFLOAT3 posA, posB, posC, posD{};
    // A,Bの座標の距離用
    XMFLOAT3 posAB, posAC, posAD;
    //判定対象A,Bの半径
    float radiusA;
    float radiusB;
    float radiusC;
    float radiusD;
    float radiusAB;
    float radiusAC;
    float radiusAD;

    //自機弾リストを取得
    const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();
    //敵弾リストを取得
    const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetEnemyBullets();

    const std::list<std::unique_ptr<BossBullet>>& bossBullets = boss_->GetEnemyBullets();

    const std::list<std::unique_ptr<WeakEnemyBullet>>& weakEnemyBullets = this->GetWeakEnemyBullets();

#pragma region 自機と敵弾の当たり判定
    //それぞれの半径
    radiusA = 1.0f;
    radiusB = 1.0f;
    radiusC = 2.0f;
    radiusD = 4.0f;

    //自機の座標
    posA = player_->GetWorldPosition();

    //自機と全ての敵弾の当たり判定
    for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
        //敵弾の座標
        posB = bullet->GetWorldPosition();
        //座標A,Bの距離を求める
        posAB.x = (posB.x - posA.x) * (posB.x - posA.x);
        posAB.y = (posB.y - posA.y) * (posB.y - posA.y);
        posAB.z = (posB.z - posA.z) * (posB.z - posA.z);
        radiusAB = (radiusA + radiusB) * (radiusA + radiusB);

        //球と球の交差判定
        if (radiusAB >= (posAB.x + posAB.y + posAB.z)) {
            //自キャラの衝突時コールバック関数を呼び出す
            player_->OnCollision();
            //敵弾の衝突時コールバック関数を呼び出すgg
            bullet->OnCollision();


        }
    }

    //自機と全ての敵弾の当たり判定
    for (const std::unique_ptr<WeakEnemyBullet>& weakbullet : weakEnemyBullets) {
        //敵弾の座標
        posC = weakbullet->GetWorldPosition();
        //座標A,Bの距離を求める
        posAC.x = (posC.x - posA.x) * (posC.x - posA.x);
        posAC.y = (posC.y - posA.y) * (posC.y - posA.y);
        posAC.z = (posC.z - posA.z) * (posC.z - posA.z);
        radiusAC = (radiusA + radiusC) * (radiusA + radiusC);


        //球と球の交差判定
        if (radiusAC >= (posAC.x + posAC.y + posAC.z)) {
            //自キャラの衝突時コールバック関数を呼び出す
            player_->OnCollision();
            //敵弾の衝突時コールバック関数を呼び出す
            weakbullet->OnCollision();


        }
    }

    //自機と全ての敵弾の当たり判定
    for (const std::unique_ptr<BossBullet>& bossbullet : bossBullets) {
        //敵弾の座標
        posD = bossbullet->GetWorldPosition();
        //座標A,Bの距離を求める
        posAD.x = (posD.x - posA.x) * (posD.x - posA.x);
        posAD.y = (posD.y - posA.y) * (posD.y - posA.y);
        posAD.z = (posD.z - posA.z) * (posD.z - posA.z);
        radiusAD = (radiusA + radiusD) * (radiusA + radiusD);

        //球と球の交差判定
        if (radiusAD >= (posAD.x + posAD.y + posAD.z)) {
            //自キャラの衝突時コールバック関数を呼び出す
            player_->OnCollision();
            //敵弾の衝突時コールバック関数を呼び出すgg
            bossbullet->OnCollision();


        }
    }

#pragma endregion

#pragma region 自弾と敵の当たり判定
    //それぞれの半径
    radiusA = 5.0f;
    radiusB = 1.0f;
    radiusC = 5.0f;
    radiusD = 20.0f;

    //敵の座標
    posA = enemy_->GetWorldPosition();

    //敵と全ての弾の当たり判定
    for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
        //弾の座標
        posB = bullet->GetWorldPosition();
        //座標A,Bの距離を求める
        posAB.x = (posB.x - posA.x) * (posB.x - posA.x);
        posAB.y = (posB.y - posA.y) * (posB.y - posA.y);
        posAB.z = (posB.z - posA.z) * (posB.z - posA.z);
        radiusAB = (radiusA + radiusB) * (radiusA + radiusB);

        //球と球の交差判定
        if (radiusAB >= (posAB.x + posAB.y + posAB.z)) {
            //敵キャラの衝突時コールバック関数を呼び出す
            enemy_->OnCollisionPlayer();
            //自機弾の衝突時コールバック関数を呼び出す
            bullet->OnCollision();

            pm1_->ActiveZ(particle1_, { object3DEnemy_->GetPosition() }, { 0.0f ,0.0f,25.0f }, { 4.2f,4.2f,0.0f }, { 0.0f,0.001f,0.0f }, 10, { 3.0f, 0.0f });

        }
    }

    for (std::unique_ptr<WeakEnemy>& weakEnemy_ : _WeakEnemy) {
        posC = weakEnemy_->GetWorldPosition();


        //雑魚敵と全ての弾の当たり判定
        for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
            //弾の座標
            posB = bullet->GetWorldPosition();
            //座標A,Bの距離を求める
            posAC.x = (posB.x - posC.x) * (posB.x - posC.x);
            posAC.y = (posB.y - posC.y) * (posB.y - posC.y);
            posAC.z = (posB.z - posC.z) * (posB.z - posC.z);
            radiusAC = (radiusA + radiusC) * (radiusA + radiusC);

            //球と球の交差判定
            if (radiusAC >= (posAC.x + posAC.y + posAC.z)) {
                //敵キャラの衝突時コールバック関数を呼び出す
                 //弾更新


                weakEnemy_->OnCollisionPlayer();


                //自機弾の衝突時コールバック関数を呼び出す
                bullet->OnCollision();

                pm1_->ActiveZ(particle1_, { weakEnemy_->GetWorldPosition() }, { 0.0f ,0.0f,25.0f }, { 4.2f,4.2f,0.0f }, { 0.0f,0.001f,0.0f }, 10, { 10.0f, 0.0f });
            }
        }
    }
    //敵の座標
    posA = boss_->GetWorldPosition();

    //敵と全ての弾の当たり判定
    for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
        //弾の座標
        posD = bullet->GetWorldPosition();
        //座標A,Bの距離を求める
        posAD.x = (posD.x - posA.x) * (posD.x - posA.x);
        posAD.y = (posD.y - posA.y) * (posD.y - posA.y);
        posAD.z = (posD.z - posA.z) * (posD.z - posA.z);
        radiusAD = (radiusA + radiusD) * (radiusA + radiusD);

        //球と球の交差判定
        if (radiusAD >= (posAD.x + posAD.y + posAD.z)) {
            //敵キャラの衝突時コールバック関数を呼び出す
            boss_->OnCollisionPlayer();
            //自機弾の衝突時コールバック関数を呼び出す
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
    //object3DWeakEnemy_->SetPosition(trans);
    std::unique_ptr<WeakEnemy> newWeakEnemy = std::make_unique<WeakEnemy>();
    newWeakEnemy->Initialize(modelWeakEnemy_, trans, camera_);
    //newWeakEnemy.reset(weakEnemy_);
    _WeakEnemy.push_back(std::move(newWeakEnemy));
}

void GamePlayScene::LoadPopEnemyData()
{

    //ファイルを開く
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
