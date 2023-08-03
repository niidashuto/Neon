#include "MyGame.h"
#include "Quaternion.h"
#include "FbxLoader.h"
#include "ObjectFBX.h"
#include <fstream>




void MyGame::Initialize()
{
#pragma region 基盤システムの初期化


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

    //音声読み込み
    //audio->SoundLoadWave("Resources/fanfare.wav");
    //音声再生
    //audio->SoundPlayWave("Resources/fanfare.wav");


#pragma endregion 基盤システムの初期化

#pragma region 最初のシーンを初期化

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
    
    //3Dオブジェクトと3Dモデルをひも付け
    object3d_1->SetModel(model_1);
    object3d_2->SetModel(model_2);
    object3d_3->SetModel(model_2);
    object3DPlayer_->SetModel(modelPlayer_);
    object3DEnemy_->SetModel(modelEnemy_);
    //object3DWeakEnemy_->SetModel(modelWeakEnemy_);
    object3DRail_->SetModel(modelRail_);
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
    

#pragma endregion 最初のシーンを初期化
}

void MyGame::Finalize()
{
#pragma region 最初のシーンの終了
    delete sprite;
    imGui->Finalize();
    delete imGui;

#pragma endregion 最初のシーンの終了

#pragma region 基盤システムの終了
   

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
    
#pragma endregion 基盤システムの終了
}

void MyGame::Update()
{
#pragma region 基盤システムの更新
    SNFramework::Update();
#pragma endregion 基盤システムの更新

#pragma region 最初のシーンの更新

    
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

    


#pragma endregion 最初のシーンの更新
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
   
    //描画前処理
    dxCommon->PreDraw();

#pragma region 最初のシーンの描画
   
    postEffect->Draw(dxCommon->GetCommandList());
    imGui->Draw();
#pragma endregion 最初のシーンの描画

    //描画後処理
    dxCommon->PostDraw();
}

void MyGame::CheckAllCollisions()
{
    //判定対象A,Bの座標
    XMFLOAT3 posA, posB,posC{};
    // A,Bの座標の距離用
    XMFLOAT3 posAB,posAC;
    //判定対象A,Bの半径
    float radiusA;
    float radiusB;
    float radiusC;
    float radiusAB;
    float radiusAC;

    //自機弾リストを取得
    const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();
    //敵弾リストを取得
    const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetEnemyBullets();

    const std::list<std::unique_ptr<WeakEnemyBullet>>& weakEnemyBullets = this->GetWeakEnemyBullets();

#pragma region 自機と敵弾の当たり判定
    //それぞれの半径
    radiusA = 1.0f;
    radiusB = 1.0f;
    radiusC = 2.0f;

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

#pragma endregion

#pragma region 自弾と敵の当たり判定
    //それぞれの半径
    radiusA = 5.0f;
    radiusB = 1.0f;
    radiusC = 5.0f;

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
    //ファイルを開く
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







