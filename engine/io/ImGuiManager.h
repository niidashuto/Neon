#pragma once
#include "DirectXCommon.h"

/**
*ImGuiManager.h

* ImGui�}�l�[�W���[
*/
class ImGuiManager
{
public:
	//������
	void Initialize(WinApp* winApp, DirectXCommon* dxCommon);
	//�폜
	void Finalize();
	//�`��O
	void Begin();
	//�`���
	void End();
	//�`��
	void Draw();
private:
	//SRV�p�f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;

	DirectXCommon* dxCommon_ = nullptr;
};

