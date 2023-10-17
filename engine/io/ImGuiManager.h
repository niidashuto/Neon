#pragma once
#include "DirectXCommon.h"

/**
*ImGuiManager.h

* ImGuiマネージャー
*/
class ImGuiManager
{
public:
	//初期化
	void Initialize(WinApp* winApp, DirectXCommon* dxCommon);
	//削除
	void Finalize();
	//描画前
	void Begin();
	//描画後
	void End();
	//描画
	void Draw();
private:
	//SRV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;

	DirectXCommon* dxCommon_ = nullptr;
};

