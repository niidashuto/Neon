#pragma once

#include "FbxModel.h"
#include "Camera.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>

class ObjectFBX
{
protected://�G�C���A�X
	//Microsoft::WRL::���ȗ�
	template <class T>using ComPtr =
		Microsoft::WRL::ComPtr<T>;
	//DirectX���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public://�萔
	//�{�[���̍ő吔
	static const int MAX_BONES = 32;

public:
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;
		XMMATRIX world;
		XMFLOAT3 cameraPos;
	};
	//�萔�o�b�t�@�p�f�[�^�\����(�X�L�j���O)
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};
public://�ÓI�����o�֐�

	void Initialize();

	void Update();

	void Draw(ID3D12GraphicsCommandList* cmdList);

	void SetModel(FbxModel* fbxModel) { this->fbxModel = fbxModel; }

	void PlayAnimation();
	//setter
	static void SetDevice(ID3D12Device* device) { ObjectFBX::device = device; }

	static void SetCamera(Camera* camera) { ObjectFBX::camera = camera; }

	void SetPosition(const XMFLOAT3& position) { this->position = position; }

	void SetScale(const XMFLOAT3& scale) { this->scale = scale; }

	void SetRotation(const XMFLOAT3& rotation) { this->rotation = rotation; }

	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	static void PostDraw();

	static void CreateGraphicsPipeline();

protected:
	ComPtr<ID3D12Resource> constBuffTransform;

	ComPtr<ID3D12Resource> constBuffSkin;

	XMFLOAT3 scale = { 1,1,1 };

	XMFLOAT3 rotation = { 0,0,0 };

	XMFLOAT3 position = { 0,0,0 };

	XMMATRIX matWorld;

	FbxModel* fbxModel = nullptr;
	//1�t���[���̎���
	FbxTime frameTime;
	//�A�j���[�V�����J�n����
	FbxTime startTime;
	//�A�j���[�V�����I������
	FbxTime endTime;
	//���ݎ���(�A�j���[�V����)
	FbxTime currentTime;
	//�A�j���[�V�����Đ���
	bool isPlay = false;

private://�ÓI�����o�ϐ�
	//�f�o�C�X
	static ID3D12Device* device;
	//�J����
	static Camera* camera;

	static ID3D12GraphicsCommandList* cmdList_;

	static ComPtr<ID3D12RootSignature> rootsignature;

	static ComPtr<ID3D12PipelineState> pipelinestate;

};

