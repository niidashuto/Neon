#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include "Camera.h"
#include "Particle.h"

/// <summary>
/// 3D�I�u�W�F�N�g
/// </summary>
class ParticleManager
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �T�u�N���X
	// ���_�f�[�^�\����
	//struct VertexPosNormalUv
	//{
	//	XMFLOAT3 pos; // xyz���W
	//	XMFLOAT3 normal; // �@���x�N�g��
	//	XMFLOAT2 uv;  // uv���W
	//};


	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		//XMFLOAT4 color;	// �F (RGBA)
		XMMATRIX mat;	// �R�c�ϊ��s��
		XMMATRIX matBillboard;	//�r���{�[�h�s��
	};

private: // �萔
	static const int division = 50;					// ������
	static const float radius;				// ��ʂ̔��a
	static const float prizmHeight;			// ���̍���
	static const int planeCount = division * 2 + division * 2;		// �ʂ̐�
	static const int vertexCount = 1024;		// ���_��
	//static const int indexCount = 3 * 2;	//�C���f�b�N�X��
public: // �ÓI�����o�֐�
	/// <summary>
	/// �ÓI������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	static void StaticInitialize(ID3D12Device* device);

	/// <summary>
	/// �`��O����
	/// </summary>
	/// <param name="cmdList">�`��R�}���h���X�g</param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �`��㏈��
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// 3D�I�u�W�F�N�g����
	/// </summary>
	/// <returns></returns>
	static ParticleManager* Create();

private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* device_;
	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;

private:// �ÓI�����o�֐�

	/// �O���t�B�b�N�p�C�v���C������
	/// </summary>
	/// <returns>����</returns>
	static void InitializeGraphicsPipeline();

public: // �����o�֐�
	bool Initialize();
	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �p�[�e�B�N������
	/// </summary>
	/// <param name="p">�p�[�e�B�N��</param>
	/// <param name="setpos">�ʒu</param>
	/// <param name="setvel">�ړ���</param>
	/// <param name="setacc">�d�͕��z</param>
	/// <param name="setnum">��C�ɉ���</param>
	///  <param name="setscale">x = �J�n�X�P�[�� , y = �I���X�P�[��</param>
	void Active(Particle* p, const float& setpos, const float& setvel, const float& setacc, const int& setnum, const XMFLOAT2& setscale);

private: // �����o�ϐ�
	ComPtr<ID3D12Resource> constBuff; // �萔�o�b�t�@
	// ���[�J���X�P�[��
	XMFLOAT3 scale = { 1.0f,1.0f,1.0f };

	Particle* particle_ = nullptr;

	Camera* camera_ = nullptr;

public://�A�N�Z�b�T�u����
	//�p�[�e�B�N�����f��
	void SetParticleModel(Particle* pmodel) { this->particle_ = pmodel; }
	//�J����
	void SetCamera(Camera* camera) { this->camera_ = camera; }
};


