#pragma once

#include "AbstractSceneFactory.h"

//���̃Q�[���̃V�[���H��
class SceneFactory : public AbstractSceneFactory
{
public:
	//�V�[������
	BaseScene* CreateScene(const std::string& sceneName)override;
};
