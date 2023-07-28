#pragma once
#include "Sprite.h"
//ポストエフェクト
class PostEffect
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
        verticesCount,
    };

public:
    //コンストラクタ
    PostEffect();
    //初期化
    void Initialize(SpriteCommon* spCommon, uint32_t textureIndex = UINT32_MAX);

    void Update();
    //テクスチャ生成
    void CreateTexture();
    // SRV生成
    void CreateSRV();
    // RTV生成
    void CreateRTV();
    // 深度バッファ生成
    void CreateDepthBuffer();
    // DSV生成
    void CreateDSV();

    void CreateGraphicsPipelineState();

    //描画
    void Draw(ID3D12GraphicsCommandList* cmdList);
    //描画前処理
    void PreDraw(ID3D12GraphicsCommandList* cmdList);

    //描画後処理
    void PostDraw(ID3D12GraphicsCommandList* cmdList);

    void SetColor(const DirectX::XMFLOAT4& color) { this->color_ = color; }

private:
    //テクスチャバッファ
    Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;
    //SRV用デスクリプタヒープ
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapSRV;
    //深度バッファ
    Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff;
    //RTV用デスクリプタヒープ
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapRTV;
    //DSV用デスクリプタヒープ
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapDSV;

    Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;

    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;

    SpriteCommon* spriteCommon_ = nullptr;

    //画面クリアカラー
    static const float clearcolor[4];

    Vertex vertices[4];

    D3D12_VERTEX_BUFFER_VIEW vbView{};

    Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;

    Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial;
    ConstBufferDataMaterial* constMapMaterial = nullptr;

    Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform;
    ConstBufferDataTransform* constMapTransform = nullptr;

    //非表示フラグ
    bool isInvisible_ = false;

    //スプライト情報
    DirectX::XMFLOAT4 color_ = { 1,1,1,1 };
};