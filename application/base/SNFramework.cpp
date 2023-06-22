#include "SNFramework.h"

void SNFramework::Initialize()
{
	//WindowsAPI�̏�����
	winApp = new WinApp();
	winApp->Initialize();

	//DirectX�̏�����
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	//�X�v���C�g���ʕ��̏�����
	spriteCommon = new SpriteCommon();
	spriteCommon->Initialize(dxCommon);

	//���͂̏�����
	input = new Input();
	input->Initialize(winApp);

	audio = new Audio();
	audio->Initialize();

	FbxLoader::GetInstance()->Initialize(dxCommon->GetDevice());

}

void SNFramework::Update()
{
	input->Update();
}

void SNFramework::Finalize()
{
	audio->Finalize();
	winApp->Finalize();
	FbxLoader::GetInstance()->Finalize();
	
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
