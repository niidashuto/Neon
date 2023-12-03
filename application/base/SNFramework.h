#pragma once
#include "WinApp.h"
#include "DirectXCommon.h"
#include "SpriteCommon.h"
#include "Input.h"
#include "Audio.h"
#include "FbxLoader.h"
#include "PostEffect.h"
#include "ImGuiManager.h"
#include "SceneManager.h"

/**
*SNFramework.h

* ����t���[�����[�N
*/
class SNFramework
{
public://�����o�֐�
	//������
	virtual void Initialize();

	//�I��
	virtual void Finalize();

	//���t���[���X�V
	virtual void Update();

	//�`��
	virtual void Draw() = 0;

	//�I���`�F�b�N
	virtual bool IsEndRequest();

	virtual ~SNFramework() = default;

	//���s
	void Run();

protected:

	WinApp* winApp = nullptr;

	DirectXCommon* dxCommon = nullptr;

	SpriteCommon* spriteCommon = nullptr;

	Input* input = nullptr;

	Audio* audio = nullptr;

	PostEffect* postEffect = nullptr;

	ImGuiManager* imguiManager = nullptr;

	//BaseScene* baseScene

	SceneManager* sceneManager = nullptr;

	
};

