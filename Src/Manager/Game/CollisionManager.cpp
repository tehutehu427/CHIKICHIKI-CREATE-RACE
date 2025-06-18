#include"../Object/Common/Transform.h"
#include"../Object/Common/Geometry/Geometry.h"
#include"../Object/Common/Collider.h"
#include"../Object/ObjectBase.h"
#include"../Utility/Utility.h"
#include "CollisionManager.h"

CollisionManager* CollisionManager::instance_ = nullptr;

void CollisionManager::CreateInstance(void)
{
	//インスタンスの初期生成
	if (instance_ == nullptr)
	{
		instance_ = new CollisionManager();
	}
}

void CollisionManager::Sweep(void)
{
	//終了したコライダを並び変える
	auto it = std::remove_if(colliders_.begin(), colliders_.end(),
		[](const std::shared_ptr<Collider> _col)
		{
			return _col->IsDead();
		});

	//終了したコライダを削除する
	colliders_.erase(it, colliders_.end());
}

void CollisionManager::Update(void)
{
	//コライダが一つもないなら処理を飛ばす
	if (colliders_.size() <= 0)return;

	for (int i = 0; i < colliders_.size() - 1; i++)
	{
		for (int j = i + 1; j < colliders_.size(); j++)
		{
			//当たり判定をする範囲内　又は　タグが同じかどうか
			if (Utility::SqrMagnitude(
				colliders_[i]->GetGeometry().GetColPos(),
				colliders_[j]->GetGeometry().GetColPos())
				>= HIT_DIS_SQUARE
				|| colliders_[i]->GetTag() == colliders_[j]->GetTag())
			{
				//範囲外　又は　同一タグだったので飛ばす
				colliders_[i]->NotHit();
				colliders_[j]->NotHit();
				continue;
			}

			//当たり判定
			if(IsCollision(colliders_[i],colliders_[j]))
			{
				//それぞれの当たった処理
				colliders_[i]->OnHit(colliders_[j]);
				colliders_[j]->OnHit(colliders_[i]);
			}
		}
	}
}

void CollisionManager::Destroy(void)
{
	//コライダの全削除
	colliders_.clear();

	//自身のインスタンス削除
	delete instance_;
	instance_ = nullptr;
}

CollisionManager::CollisionManager(void)
{
}

CollisionManager::~CollisionManager(void)
{
}

bool CollisionManager::IsCollision(const std::weak_ptr<Collider> _col1, const std::weak_ptr<Collider> _col2)
{
	//1つ目の当たり判定形状
	const auto& geo1 = _col1.lock()->GetGeometry();
	
	//2つ目の当たり判定形状
	const auto& geo2 = _col2.lock()->GetGeometry();

	//当たり判定
	return geo1.IsHit(geo2);
}