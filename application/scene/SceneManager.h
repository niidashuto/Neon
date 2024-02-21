#pragma once
#include "BaseScene.h"
#include "AbstractSceneFactory.h"

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
	//void SetNextScene(BaseScene* nextScene) { nextScene_ = nextScene; }

	void ChangeScene(const std::string& sceneName);

	//�V�[���t�@�N�g���[�̃Z�b�^�[
	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { this->sceneFactory_ = sceneFactory; }

private:
	//���̃V�[��(���s���̃V�[��)
	BaseScene* scene_ = nullptr;

	//���̃V�[��
	BaseScene* nextScene_ = nullptr;

	//�V�[���t�@�N�g���[(�؂�Ă���)
	AbstractSceneFactory* sceneFactory_ = nullptr;
};