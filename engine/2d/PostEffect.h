#pragma once
#include "Sprite.h"
//�|�X�g�G�t�F�N�g
class PostEffect
{
public:
    //�萔�o�b�t�@�f�[�^�\����(�}�e���A��)
    struct ConstBufferDataMaterial {
        DirectX::XMFLOAT4 color;
    };

    //�萔�o�b�t�@�f�[�^�\����(3D�ϊ��s��)
    struct ConstBufferDataTransform {
        DirectX::XMMATRIX mat;//3D�ϊ��s��
    };

    //���_�f�[�^
    struct Vertex {
        DirectX::XMFLOAT3 pos;
        DirectX::XMFLOAT2 uv;
    };

    //���_�ԍ�
    enum VertexNumber {
        LB,//����
        LT,//����
        RB,//�E��
        RT,//�E��
        verticesCount,
    };

public:
    //�R���X�g���N�^
    PostEffect();
    //������
    void Initialize(SpriteCommon* spCommon, uint32_t textureIndex = UINT32_MAX);

    void Update();
    //�e�N�X�`������
    void CreateTexture();
    // SRV����
    void CreateSRV();
    // RTV����
    void CreateRTV();
    // �[�x�o�b�t�@����
    void CreateDepthBuffer();
    // DSV����
    void CreateDSV();

    void CreateGraphicsPipelineState();

    //�`��
    void Draw(ID3D12GraphicsCommandList* cmdList);
    //�`��O����
    void PreDraw(ID3D12GraphicsCommandList* cmdList);

    //�`��㏈��
    void PostDraw(ID3D12GraphicsCommandList* cmdList);

    void SetColor(const DirectX::XMFLOAT4& color) { this->color_ = color; }

private:
    //�e�N�X�`���o�b�t�@
    Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;
    //SRV�p�f�X�N���v�^�q�[�v
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapSRV;
    //�[�x�o�b�t�@
    Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff;
    //RTV�p�f�X�N���v�^�q�[�v
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapRTV;
    //DSV�p�f�X�N���v�^�q�[�v
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapDSV;

    Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;

    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;

    SpriteCommon* spriteCommon_ = nullptr;

    //��ʃN���A�J���[
    static const float clearcolor[4];

    Vertex vertices[4];

    D3D12_VERTEX_BUFFER_VIEW vbView{};

    Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;

    Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial;
    ConstBufferDataMaterial* constMapMaterial = nullptr;

    Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform;
    ConstBufferDataTransform* constMapTransform = nullptr;

    //��\���t���O
    bool isInvisible_ = false;

    //�X�v���C�g���
    DirectX::XMFLOAT4 color_ = { 1,1,1,1 };
};