#pragma once

//�O���錾
class SceneManager;

//�V�[�����N���X
class BaseScene {
public://�����o�֐�


	virtual ~BaseScene() = default;

	//������
	virtual void Initialize() = 0;
	//�X�V
	virtual void Update() = 0;
	//�`��
	virtual void Draw() = 0;
	//�I������
	virtual void Finalize() = 0;

	virtual void SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }

private:
	//�V�[���}�l�[�W���[(�؂�Ă���)
	SceneManager* sceneManager_ = nullptr;
};