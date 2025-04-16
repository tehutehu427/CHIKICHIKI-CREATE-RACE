#include "../Manager/System/ResourceManager.h"
#include "../Manager/System/SceneManager.h"
#include "ObjectBase.h"

ObjectBase::ObjectBase(void)
	: resMng_(ResourceManager::GetInstance()),
	scnMng_(SceneManager::GetInstance())
{
	status_.isBreak = false;
	status_.isGravity = false;
	status_.size = { 0,0,0 };
}

ObjectBase::~ObjectBase(void)
{
}
