#pragma once
#include <DirectXMath.h>
#include "SpriteCommon.h"


//スプライト
class Sprite
{
public:
	//定数バッファデータ構造体(マテリアル)
	struct ConstBufferDataMaterial {
		DirectX::XMFLOAT4 color;
	};

	//定数バッファデータ構造体(3D変換行列)
	struct ConstBufferDataTransform {
		DirectX::XMMATRIX mat;//3D変換行列
	};

	//頂点データ
	struct Vertex {
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
	};

	//頂点番号
	enum VertexNumber {
		LB,//左下
		LT,//左上
		RB,//右下
		RT,//右上
	};

public://メンバ関数
	//初期化
	void Initialize(SpriteCommon* spriteCommon, uint32_t textureIndex = UINT32_MAX);

	void Update();

	void Draw();
public:
	//setter
	void SetPosition(const DirectX::XMFLOAT2& position) { position_ = position; }
	void SetRotation(float rotation) { rotationZ = rotation; }
	void SetColor(DirectX::XMFLOAT4 color) { color_ = color; }
	void SetSize(const DirectX::XMFLOAT2& size) { size_ = size; }
	void SetAnchorPoint(const DirectX::XMFLOAT2& anchorPoint) { anchorPoint_ = anchorPoint; }
	void SetIsFlipX(const bool& isFlipX) { isFlipX_ = isFlipX; }
	void SetIsFlipY(const bool& isFlipY) { isFlipY_ = isFlipY; }
	void SetIsInvisible(const bool& isInvisible) { isInvisible_ = isInvisible; }
	void SetTextureIndex(uint32_t index) { this->textureIndex_ = index; }
	void SetTextureLeftTop(const DirectX::XMFLOAT2& textureLeftTop) { this->textureLeftTop_ = textureLeftTop; }
	void SetTextureSize(const DirectX::XMFLOAT2& textureSize) { this->textureSize_ = textureSize; }

	//getter
	const DirectX::XMFLOAT2& GetPosition() const { return position_; }
	float GetRotation() const { return rotationZ; }
	const DirectX::XMFLOAT4 GetColor()const { return color_; }
	const DirectX::XMFLOAT2& GetSize() const { return size_; }
	const DirectX::XMFLOAT2& GetAnchorPoint()const { return anchorPoint_; }
	const bool GetIsFlipX()const { return isFlipX_; }
	const bool GetIsFlipY()const { return isFlipY_; }
	const bool GetIsInvisible()const { return isInvisible_; }
	const uint32_t GetTextureIndex()const { return textureIndex_; }
	const DirectX::XMFLOAT2& GetTextureLeftTop()const { return textureLeftTop_; }
	const DirectX::XMFLOAT2& GetTextureSize()const { return textureSize_; }
private://メンバ関数
	//テクスチャサイズをイメージに合わせる
	void AdjustTextureSize();

protected:
	//スプライト情報
	DirectX::XMFLOAT4 color_ = { 1,1,1,1 };
	float rotationZ = 0.0f;
	DirectX::XMFLOAT2 position_ = { 0.0f,0.0f };
	DirectX::XMFLOAT2 size_ = { 100.0f,100.0f };
	//アンカーポイント
	DirectX::XMFLOAT2 anchorPoint_ = { 0.0f,0.0f };
	//テクスチャ情報
	//テクスチャ左上座標
	DirectX::XMFLOAT2 textureLeftTop_ = { 0.0f,0.0f };
	//テクスチャ切り出しサイズ
	DirectX::XMFLOAT2 textureSize_ = { 100.0f,100.0f };

	//テクスチャ番号
	uint32_t textureIndex_ = 0;

	//左右フリップ
	bool isFlipX_ = false;
	//上下フリップ
	bool isFlipY_ = false;
	//非表示フラグ
	bool isInvisible_ = false;

	SpriteCommon* spriteCommon_;

	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;

	D3D12_VERTEX_BUFFER_VIEW vbView{};

	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial;
	ConstBufferDataMaterial* constMapMaterial = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform;
	ConstBufferDataTransform* constMapTransform = nullptr;

	Vertex vertices[4];

};