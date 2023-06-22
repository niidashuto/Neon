#pragma once
#include <DirectXMath.h>
#include <d3d12.h>
#include <wrl.h>
#include<array>
#include<string>

#include "DirectXCommon.h"


//スプライト共通部
class SpriteCommon
{
public://メンバ関数
	//初期化
	void Initialize(DirectXCommon* dxCommon);

	void LoadTexture(uint32_t index, const std::string& fileName);

	void SetTextureCommands(uint32_t index);
	//描画前処理
	void PreDraw();
	//描画後処理
	void PostDraw();
	//getter
	DirectXCommon* GetDirectXCommon() { return dxCommon_; }
	ID3D12Resource* GetTextureBuffer(uint32_t index)const { return texBuff[index].Get(); }

	ID3D12PipelineState* GetPipeLineState() { return pipelineState.Get(); }
	ID3D12RootSignature* GetRootSignature() { return rootSignature.Get(); }

public:
	static SpriteCommon* GetInstance();
protected://静的メンバ変数
	//SRVの最大個数
	static const size_t kMaxSRVCount = 2056;
	//デフォルトテクスチャ格納ディレクトリ
	static std::string kDefaultTextureDirectoryPath;
protected:
	DirectXCommon* dxCommon_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;

	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;

	//Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap;

	//テクスチャバッファ
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, kMaxSRVCount>texBuff;

	

};