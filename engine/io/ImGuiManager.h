#pragma once
#include "DirectXCommon.h"

/**
*ImGuiManager.h

* ImGuiマネージャー
*/
class ImGuiManager
{
public:
	void Initialize(WinApp* winApp, DirectXCommon* dxCommon);

	void Finalize();

	void Begin();

	void End();

	void Draw();
private:
	//SRV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;

	DirectXCommon* dxCommon_ = nullptr;
};

