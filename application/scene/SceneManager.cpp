#include "SceneManager.h"
#include <cassert>

SceneManager* SceneManager::GetInstance()
{
	static SceneManager instance;

	return &instance;
}

SceneManager::~SceneManager()
{
}

void SceneManager::Update()
{
	//�V�[���؂�ւ��@�\
	if (nextScene_) {
		//���V�[���̏I��
		if (scene_) {
			scene_->Finalize();
			delete scene_;
		}

		//�V�[���؂�ւ�
		scene_ = nextScene_;
		nextScene_ = nullptr;

		//�V�[���}�l�[�W���[���Z�b�g
		scene_->SetSceneManager(this);

		//���V�[��������������
		scene_->Initialize();
	}

	//���s���V�[�����X�V����
	scene_->Update();
}

void SceneManager::Draw()
{
	scene_->Draw();
}

void SceneManager::Finalize()
{
	scene_->Finalize();
	delete scene_;
}

void SceneManager::ChangeScene(const std::string& sceneName)
{
	assert(sceneFactory_);
	assert(nextScene_ == nullptr);

	//���V�[�����쐬
	nextScene_ = sceneFactory_->CreateScene(sceneName);
}
