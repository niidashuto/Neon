#include "SNFramework.h"
#include "Object3d.h"
#include "ParticleManager.h"

void SNFramework::Initialize()
{
	//WindowsAPI�̏�����

	winApp = WinApp::GetInstance();

	//DirectX�̏�����

	dxCommon = DirectXCommon::GetInstance();

	//�X�v���C�g���ʕ��̏�����

	spriteCommon = SpriteCommon::GetInstance();

	//���͂̏�����
	input = Input::GetInstance();


	audio = Audio::GetInstance();

	winApp->Initialize();

	dxCommon->Initialize(winApp);

	spriteCommon->Initialize(dxCommon);

	input->Initialize(winApp);

	audio->Initialize();

	FbxLoader::GetInstance()->Initialize(dxCommon->GetDevice());

	Object3d::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height);

	ParticleManager::StaticInitialize(dxCommon->GetDevice());

	

}

void SNFramework::Update()
{

	input->Update();
}

void SNFramework::Finalize()
{
	

	audio->Finalize();
	
	FbxLoader::GetInstance()->Finalize();

	winApp->Finalize();
	
}

bool SNFramework::IsEndRequest()
{
	//Windows�̃��b�Z�[�W����
	if (winApp->ProcessMessage()) {
		return true;
	}
	if (input->TriggerKey(DIK_ESCAPE)) {
		return true;
	}

	return false;
}

void SNFramework::Run()
{
	//�Q�[���̏�����
	Initialize();

	while (true)//�Q�[�����[�v
	{
		//���t���[���X�V
		Update();
		//�I�����N�G�X�g�������甲����
		if (IsEndRequest())
		{
			break;
		}
		//�`��
		Draw();
	}
	//�Q�[���̏I��
	Finalize();
}
