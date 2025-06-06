#include"../Object/Common/Collider.h"
#include"../Object/ObjectBase.h"
#include "CollisionManager.h"

CollisionManager* CollisionManager::collisionMng_ = nullptr;
void CollisionManager::CreateInstance(void)
{
	if (collisionMng_ == nullptr)
	{
		collisionMng_ = new CollisionManager();
	}
}

CollisionManager& CollisionManager::GetInstance(void)
{
	return *collisionMng_;
}

//void CollisionManager::Update(void)
//{
//	
//}

void CollisionManager::AddCollider(std::weak_ptr<Collider> _collider)
{
	colliders_.emplace_back(_collider);
}

void CollisionManager::ClearCollider(void)
{
	colliders_.clear();
}

Collider::COL_TAG CollisionManager::LineCol(VECTOR pos1, VECTOR pos2)
{
	Collider::COL_TAG tag = Collider::COL_TAG::NONE;
	for (int i = 0; i < colliders_.size(); i++)
	{
		for (int j = i; j < colliders_.size(); j++)
		{
			if (j = i)continue;
			auto hits = MV1CollCheck_Line(colliders_[j].lock()->modelId_, -1,
				pos1, pos2);
			if (hits.HitFlag > 0)
			{
				tag = colliders_[j].lock()->type_;
				break;
			}
		}
	}
	return tag;
}

CollisionManager::CollisionManager(void)
{

}

CollisionManager::~CollisionManager(void)
{

}
