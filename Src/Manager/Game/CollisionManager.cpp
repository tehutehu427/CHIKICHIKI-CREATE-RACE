#include "CollisionManager.h"

CollisionManager* CollisionManager::collisionMng_ = nullptr;
void CollisionManager::CreateInstance(void)
{
	if (collisionMng_ == nullptr)
	{
		collisionMng_ = new CollisionManager();
	}
}

CollisionManager* CollisionManager::GetInstance(void)
{
	return collisionMng_;
}

CollisionManager::CollisionManager(void)
{
}

CollisionManager::~CollisionManager(void)
{
}
