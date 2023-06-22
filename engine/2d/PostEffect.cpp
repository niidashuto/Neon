#include "PostEffect.h"
#include <d3dx12.h>
#include "WinApp.h"


using namespace DirectX;

const float PostEffect::clearColor[4] = { 0.25f,0.5f,0.1f,0.0f };

PostEffect::PostEffect()
	:Sprite()
{

}

void PostEffect::Initialize(SpriteCommon* spriteCommon_, uint32_t textureIndex)
{
	HRESULT result;

	Sprite::Initialize(spriteCommon_,textureIndex);

	//CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	//CD3DX12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(Vertex) * 4);

	////頂点バッファの生成
	//result = spriteCommon_->GetDirectXCommon()->GetDevice()->CreateCommittedResource(
	//	&heapProp,//ヒープ設定
	//	D3D12_HEAP_FLAG_NONE,
	//	&resDesc,//リソース設定
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&vertBuff));
	//assert(SUCCEEDED(result));

	////頂点データ
	//Vertex vertices[4] = {
	//	{{-0.5f,-0.5f,0.0f},{0.0f,1.0f}},
	//	{{-0.5f,+0.5f,0.0f},{0.0f,0.0f}},
	//	{{+0.5f,-0.5f,0.0f},{1.0f,1.0f}},
	//	{{+0.5f,+0.5f,0.0f},{1.0f,0.0f}},
	//};

	////頂点バッファへのデータ転送
	//Vertex* vertMap = nullptr;
	//result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	//if (SUCCEEDED(result))
	//{
	//	memcpy(vertMap, vertices, sizeof(vertices));
	//	vertBuff->Unmap(0, nullptr);
	//}

	////頂点バッファビューの作成
	//vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	//vbView.SizeInBytes = sizeof(Vertex) * 4;
	//vbView.StrideInBytes = sizeof(Vertex);

	//CD3DX12_HEAP_PROPERTIES cbHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	//CD3DX12_RESOURCE_DESC cbResourceDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff);

	////定数バッファの生成
	//result = spriteCommon_->GetDirectXCommon()->GetDevice()->CreateCommittedResource(
	//	&cbHeapProp,//ヒープ設定
	//	D3D12_HEAP_FLAG_NONE,
	//	&cbResourceDesc,//リソース設定
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&constBuff));
	//assert(SUCCEEDED(result));



	

	//テクスチャリソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		WinApp::window_width,
		(UINT)WinApp::window_height,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	CD3DX12_CLEAR_VALUE clear = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, clearColor);

	//テクスチャバッファの生成
	result = spriteCommon_->GetDirectXCommon()->GetDevice()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&clear,
		IID_PPV_ARGS(&texBuff));
	assert(SUCCEEDED(result));

	{//テクスチャを赤クリア
		//画素数(1280x720=921600ピクセル)
		const UINT pixelCount = WinApp::window_width * WinApp::window_height;
		//画像一行分のデータサイズ
		const UINT rowPitch = sizeof(UINT) * WinApp::window_width;
		//画像全体のデータサイズ
		const UINT depthPitch = rowPitch * WinApp::window_height;
		//画像イメージ
		UINT* img = new UINT[pixelCount];
		for (int i = 0; i < pixelCount; i++)
		{
			img[i] = 0xff0000ff;
		}
		//テクスチャバッファにデータ転送
		result = texBuff->WriteToSubresource(0, nullptr,
			img, rowPitch, depthPitch);
		assert(SUCCEEDED(result));

		delete[] img;

	}

	//SRV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダーから見えるように
	srvDescHeapDesc.NumDescriptors = 1;
	//SRV用デスクリプタヒープ生成
	result = spriteCommon_->GetDirectXCommon()->GetDevice()->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));
	assert(SUCCEEDED(result));
	//SRV設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//設定構造体
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; //RGBA float
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	//デスクリプタヒープにSRV作成
	spriteCommon_->GetDirectXCommon()->GetDevice()->CreateShaderResourceView(texBuff.Get(),
		&srvDesc,
		descHeapSRV->GetCPUDescriptorHandleForHeapStart());

	//RTV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc = {};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 1;
	//RTV用デスクリプタヒープ生成
	result = spriteCommon_->GetDirectXCommon()->GetDevice()->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV));
	assert(SUCCEEDED(result));

	//レンダーターゲットビューに設定
	D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
	//シェーダーの計算結果をSRGBに変換して書き込む
	renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	//デスクリプタヒープにRTVを作成
	spriteCommon_->GetDirectXCommon()->GetDevice()->CreateRenderTargetView(texBuff.Get(),
		&renderTargetViewDesc,
		descHeapRTV->GetCPUDescriptorHandleForHeapStart()
	);

	//深度バッファリソース設定
	CD3DX12_RESOURCE_DESC depthResDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_D32_FLOAT,
			WinApp::window_width,
			WinApp::window_height,
			1, 0,
			1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
		);

	CD3DX12_HEAP_PROPERTIES depthHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	CD3DX12_CLEAR_VALUE clearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);
	//深度バッファの生成
	result = spriteCommon_->GetDirectXCommon()->GetDevice()->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&clearValue,
		IID_PPV_ARGS(&depthBuff));
	assert(SUCCEEDED(result));

	//DSV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc{};
	DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DescHeapDesc.NumDescriptors = 1;
	//DSV用デスクリプタヒープを作成
	result = spriteCommon_->GetDirectXCommon()->GetDevice()->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(&descHeapDSV));
	assert(SUCCEEDED(result));

	//デスクリプタヒープにDSV作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	spriteCommon_->GetDirectXCommon()->GetDevice()->CreateDepthStencilView(depthBuff.Get(),
		&dsvDesc,
		descHeapDSV->GetCPUDescriptorHandleForHeapStart());



}

void PostEffect::Draw(ID3D12GraphicsCommandList* cmdList)
{
	if (isInvisible_)
	{
		return;
	}


	//パイプラインステートの設定
	//cmdList->SetPipelineState(spriteCommon_->GetPipeLineState());

	//cmdList->SetGraphicsRootSignature(spriteCommon_->GetRootSignature());

	//cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//SRVヒープの先頭ハンドルを取得(SRVを指しているはず)
	//UINT descriptorhandleIncrementSize = spriteCommon_->GetDirectXCommon()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
	spriteCommon_->GetDirectXCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(
		1,
		descHeapSRV->GetGPUDescriptorHandleForHeapStart());

	//テクスチャコマンド
	//spriteCommon_->SetTextureCommands(textureIndex_);

	//頂点バッファビューの設定コマンド
	spriteCommon_->GetDirectXCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	//定数バッファビュー(CBV)の設定コマンド
	spriteCommon_->GetDirectXCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
	spriteCommon_->GetDirectXCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());
	//描画コマンド
	spriteCommon_->GetDirectXCommon()->GetCommandList()->DrawInstanced(4, 1, 0, 0);//全ての頂点を使って描画
}

void PostEffect::PreDrawScene(ID3D12GraphicsCommandList* cmdList)
{
	CD3DX12_RESOURCE_BARRIER resBarrier = CD3DX12_RESOURCE_BARRIER::Transition(texBuff.Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_RENDER_TARGET);

	//リソースバリアを変更
	cmdList->ResourceBarrier(1,
		&resBarrier);

	//レンダーターゲットビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH =
		descHeapRTV->GetCPUDescriptorHandleForHeapStart();
	//深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH =
		descHeapDSV->GetCPUDescriptorHandleForHeapStart();
	//レンダーターゲットをセット
	cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);
	CD3DX12_VIEWPORT viewPort = CD3DX12_VIEWPORT(0.0f, 0.0f, WinApp::window_width, WinApp::window_height);
	//ビューポートの設定
	cmdList->RSSetViewports(1, &viewPort);
	CD3DX12_RECT rect = CD3DX12_RECT(0, 0, WinApp::window_width, WinApp::window_height);
	//シザリング矩形の設定
	cmdList->RSSetScissorRects(1, &rect);

	//画面クリア
	cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
	//深度バッファのクリア
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void PostEffect::PostDrawScene(ID3D12GraphicsCommandList* cmdList)
{
	CD3DX12_RESOURCE_BARRIER resBarrierChange = CD3DX12_RESOURCE_BARRIER::Transition(texBuff.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	//リソースバリアを変更(描画可能->シェーダーリソース)
	cmdList->ResourceBarrier(1, &resBarrierChange);

}
