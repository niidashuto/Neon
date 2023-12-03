#pragma once
#include "BaseScene.h"

//�V�[���Ǘ�
class SceneManager
{
public://�V���O���g���C���X�^���X
	static SceneManager* GetInstance();

public://�����o�֐�
	~SceneManager();

	//�X�V
	void Update();
	//�`��
	void Draw();
	//�I��
	void Finalize();



public:
	//���V�[���\��
	void SetNextScene(BaseScene* nextScene) { nextScene_ = nextScene; }

private:
	//���̃V�[��(���s���̃V�[��)
	BaseScene* scene_ = nullptr;

	//���̃V�[��
	BaseScene* nextScene_ = nullptr;
};