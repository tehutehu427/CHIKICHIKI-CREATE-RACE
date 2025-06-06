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

//void CollisionManager::Update(void)
//{
//	
//}

void CollisionManager::LineCol(ObjectBase& _object, VECTOR pos1, VECTOR pos2)
{

}

CollisionManager::CollisionManager(void)
{

}

CollisionManager::~CollisionManager(void)
{

}
