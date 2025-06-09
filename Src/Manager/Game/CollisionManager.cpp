#include"../Utility/Utility.h"
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
				CheckItemsInPlayerColRange(_player, colPos);
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

void CollisionManager::Destroy(void)
{
	ClearCollider();
	if (collisionMng_ != nullptr)
	{
		delete collisionMng_;
		collisionMng_ = nullptr;
	}
}

CollisionManager::Coll_Info CollisionManager::LineCol(VECTOR pos1, VECTOR pos2)
{
	Coll_Info col;
	col.tag = Collider::COL_TAG::NONE;
	col.isHit = false;
	col.hitPos = Utility::VECTOR_ZERO;
	col.normal = Utility::VECTOR_ZERO;
	col.colTargetPos = Utility::VECTOR_ZERO;
	for (int i = 0; i < colliders_.size(); i++)
	{
		for (int j = i; j < colliders_.size(); j++)
		{
			if (j == i)continue;
			auto hits = MV1CollCheck_Line(colliders_[j].lock()->modelId_, -1,
				pos1, pos2);
			if (hits.HitFlag > 0)
			{
				col.tag = colliders_[j].lock()->type_;
				col.hitPos = hits.HitPosition;
				col.isHit = true;
				break;
			}
		}
	}
	return col;
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

void CollisionManager::CheckItemsInPlayerColRange(Player& _player, IntVector3 _colPos)
{
	MapEditer& mapEdit = MapEditer::GetInstance();
	ItemManager& itemMng = ItemManager::GetInstance();
	if (mapEdit.IsObjectAtMapPos(_colPos))
	{
		IntVector3 lPos = mapEdit.GetLeaderMapPos(_colPos);
		//for (auto& iLPos : itemLPos_)
		//{
		//	if (iLPos == lPos)return;
		//}

		//アイテムタイプ取得
		ItemBase::ITEM_TYPE type = mapEdit.GetItemType(_colPos);


		//アイテムのTransform取得
		Transform itemTrans = itemMng.GetItemTransform(lPos, type);
		VECTOR playerUp = _player.GetMovedPos();
		playerUp.y += Player::RADIUS;

		VECTOR playerDown = _player.GetMovedPos();
		playerDown.y -= Player::RADIUS;


		auto lineHit = LineCol(playerUp, playerDown);
		_player.HitAction(lineHit.tag,lineHit.isHit, lineHit.hitPos, lineHit.colTargetPos);



		//LineCol(_player, itemTrans);
		//ArroundColl(itemTrans);

		itemLPos_.push_back(lPos);
	}
}

CollisionManager::CollisionManager(void)
{

}

CollisionManager::~CollisionManager(void)
{

}
