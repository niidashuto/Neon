#pragma once
#include "Object3d.h"
#include "ParticleManager.h"
#include "DirectXCommon.h"
#include "Audio.h"
#include "Sprite.h"
#include "Camera.h"

class GamePlayScene
{
public://�����o�֐�

	//������
	void Initialize();

	//�I��
	void Finalize();

	//���t���[������
	void Update();

	//�`��
	void Draw();

private:
	static DirectXCommon* dxCommon_;

	static SpriteCommon* spriteCommon_;

};

