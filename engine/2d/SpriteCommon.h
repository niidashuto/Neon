#pragma once
#include <DirectXMath.h>
#include <d3d12.h>
#include <wrl.h>
#include<array>
#include<string>

#include "DirectXCommon.h"


/**
*SpriteCommon.h

* �X�v���C�g���ʕ�
*/
class SpriteCommon
{
public://�����o�֐�
	//������
	void Initialize(DirectXCommon* dxCommon);
	//�e�N�X�`���ǂݍ���
	void LoadTexture(uint32_t index, const std::string& fileName);
	//�e�N�X�`���R�}���h�w��
	void SetTextureCommands(uint32_t index);
	//�`��O����
	void PreDraw();
	//�`��㏈��
	void PostDraw();
	//getter
	//DirectXCommon�w��Getter
	DirectXCommon* GetDirectXCommon() { return dxCommon_; }
	//�e�N�X�`���o�b�t�@�w��Getter
	ID3D12Resource* GetTextureBuffer(uint32_t index)const { return texBuff[index].Get(); }
	//�p�C�v���C���w��Getter
	ID3D12PipelineState* GetPipeLineState() { return pipelineState.Get(); }
	//���[�g�V�O�l�`���w��Getter
	ID3D12RootSignature* GetRootSignature() { return rootSignature.Get(); }

public:
	//SpriteCommon�C���X�^���X
	static SpriteCommon* GetInstance();
protected://�ÓI�����o�ϐ�
	//SRV�̍ő��
	static const size_t kMaxSRVCount = 2056;
	//�f�t�H���g�e�N�X�`���i�[�f�B���N�g��
	static std::string kDefaultTextureDirectoryPath;
protected:
	DirectXCommon* dxCommon_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;

	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;

	//Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap;

	//�e�N�X�`���o�b�t�@
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, kMaxSRVCount>texBuff;

	

};