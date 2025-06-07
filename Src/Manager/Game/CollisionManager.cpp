#include"../Object/Common/Collider.h"
#include"../Object/ObjectBase.h"
#include"../../Object/Player/Player.h"
#include"../../Manager/Game/MapEditer.h"
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

void CollisionManager::Update(Player& _player)
{
	VECTOR pMovedPos = _player.GetMovedPos();
	IntVector3 mapPos = MapEditer::GetInstance().WorldToMapPos(pMovedPos);
	for (int x = -Player::COL_RANGE; x <= Player::COL_RANGE; x++)
	{
		for (int y = -Player::COL_RANGE; y <= Player::COL_RANGE; y++)
		{
			for (int z = -Player::COL_RANGE; z <= Player::COL_RANGE; z++)
			{
				IntVector3 colPos = mapPos + IntVector3{ x, y, z };
				if (colPos.x < 0 || colPos.y < 0 || colPos.z < 0)continue;
				//HitPlayerAndItem(_player, colPos);
			}
		}
	}
	//itemLPos_.clear();
}

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

Collider::COL_TAG CollisionManager::SphereCol(float _radius, VECTOR _pos)
{
	Collider::COL_TAG tag = Collider::COL_TAG::NONE;
	for (int i = 0; i < colliders_.size(); i++)
	{
		for (int j = i; j < colliders_.size(); j++)
		{
			if (j = i)continue;
			auto hits = MV1CollCheck_Sphere(colliders_[j].lock()->modelId_, -1,
				_pos,_radius);
			
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
