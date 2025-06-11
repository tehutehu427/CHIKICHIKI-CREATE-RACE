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
	for (int i = 0; i < colliders_.size(); i++)
	{
		if (colliders_[i]->GetTag() == Collider::COL_TAG::PLAYER)
		{
			continue;
		}
		if (colliders_[i]->GetOwner().lock().get() == nullptr)
		{
			colliders_[i] = nullptr;
		}
	}

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

CollisionManager::Coll_Info CollisionManager::LineCol(Collider& _col, VECTOR _startPos, VECTOR _endPos)
{
	lineCol_.tag = Collider::COL_TAG::NONE;
	lineCol_.isHit = false;
	lineCol_.hitPos = Utility::VECTOR_ZERO;
	lineCol_.normal = Utility::VECTOR_ZERO;
	lineCol_.colTargetPos = Utility::VECTOR_ZERO;
	auto hit = MV1CollCheck_Line(_col.GetModelId(), -1, _startPos, _endPos);
	if (hit.HitFlag > 0)
	{
		lineCol_.tag = _col.GetTag();
		lineCol_.isHit = true;
		lineCol_.hitPos = hit.HitPosition;
		lineCol_.colTargetPos = _col.GetOwner().lock()->GetTransform().pos;
		return lineCol_;
	}
	

	return lineCol_;
}

Collider::COL_TAG CollisionManager::SphereCol(float _radius, VECTOR _pos)
{
	Collider::COL_TAG tag = Collider::COL_TAG::NONE;
	for (int i = 0; i < colliders_.size(); i++)
	{
		for (int j = i; j < colliders_.size(); j++)
		{
			if (j = i)continue;
			//auto hits = MV1CollCheck_Sphere(colliders_[j].lock()->modelId_, -1,
			//	_pos,_radius);
		}
	}
	return tag;
}

void CollisionManager::CheckItemsInPlayerColRange(Player& _player, IntVector3 _colPos)
{
	MapEditer& mapEdit = MapEditer::GetInstance();
	if (mapEdit.IsObjectAtMapPos(_colPos))
	{
		CheckCollider();
	}
}

void CollisionManager::CheckCollider(void)
{
	using COL_TYPE = Collider::COLLISION_TYPE;
	using COL_TAG = Collider::COL_TAG;
	for (int i = 0; i < colliders_.size(); i++)
	{
		for (int j = i + 1; j < colliders_.size(); j++)
		{
			auto colA = colliders_[i].get();
			auto colB = colliders_[j].get();
			if (colA == nullptr || colB == nullptr)
			{
				continue;
			}
			Collider::COL_TAG colAtag = colA->GetTag();
			Collider::COL_TAG colBtag = colB->GetTag();

			//ƒvƒŒƒCƒ„[“¯Žm‚È‚ç“–‚½‚è”»’è‚µ‚È‚¢
			if (colA->GetTag() == colB->GetTag())continue;

			if (colA->GetColType() == COL_TYPE::LINE && colB->GetColType() == COL_TYPE::MODEL)
			{
				auto owner = colA->GetOwner().lock();
				auto player = std::dynamic_pointer_cast<Player>(owner);
				//if (player == nullptr)continue;
				VECTOR playerUp = player->GetMovedPos();
				playerUp.y += Player::RADIUS;
				VECTOR playerDown = player->GetMovedPos();
				playerDown.y -= Player::RADIUS;
				if (LineCol(*colB, playerUp, playerDown).isHit)
				{
					Coll_Info lineCol = LineCol(*colB, playerUp, playerDown);
					player->HitAction(lineCol.tag,lineCol.isHit,lineCol.hitPos,lineCol.colTargetPos);
				};
			}
		}
	}
}

CollisionManager::CollisionManager(void)
{

}

void CollisionManager::MakeColllider(std::weak_ptr<ObjectBase>_owner, Collider::COLLISION_TYPE _type, Collider::COL_TAG _tag, int _modelId)
{
	std::unique_ptr<Collider>collider = std::make_unique<Collider>(_owner, _tag, _type,_modelId);
	colliders_.emplace_back(std::move(collider));
}

CollisionManager::~CollisionManager(void)
{
	colliders_.clear();
}
