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

void CollisionManager::AddCollider(const std::shared_ptr<Collider> _collider)
{
	//コライダの追加
	colliders_.push_back(_collider);
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

	//当たり判定フレーム
	if (updateFrame_ < COL_UPDATE_FRAME)
	{
		//カウンタ
		updateFrame_++;
		return;
	}

	for (int i = 0; i < colliders_.size() - 1; i++)
	{
		//コライダが死亡済みなら
		if (colliders_[i]->IsDead())
		{
			//飛ばす
			continue;
		}

		for (int j = i + 1; j < colliders_.size(); j++)
		{
			//コライダが死亡済みなら
			if (colliders_[i]->IsDead() || colliders_[j]->IsDead())
			{
				//飛ばす
				continue;
			}

			//当たり判定をするか
			if (!JudgeIsCollision(i, j))
			{
				//当たり判定の条件に合わなかったので飛ばす
				continue;
			}

			//当たり判定
			if (IsCollision(colliders_[i], colliders_[j]))
			{

				//それぞれの当たった処理
				colliders_[i]->OnHit(colliders_[j]);
				colliders_[j]->OnHit(colliders_[i]);

				//当たった後の処理
				if (!colliders_[i]->IsDead())
				{
					colliders_[i]->GetGeometry().HitAfter();
				}
				if (!colliders_[j]->IsDead())
				{
					colliders_[j]->GetGeometry().HitAfter();
				}
			}
		}
	}
	//カウンタの初期化
	updateFrame_ = 0;
}

void CollisionManager::Destroy(void)
{
	//コライダの全削除
	colliders_.clear();

	//自身のインスタンス削除
	delete instance_;
	instance_ = nullptr;
}

const bool CollisionManager::IsPlayer(const Collider::TAG _tag)const
{
	return _tag == Collider::TAG::PLAYER1 || _tag == Collider::TAG::PLAYER2 || _tag == Collider::TAG::PLAYER3 || _tag == Collider::TAG::PLAYER4 || _tag == Collider::TAG::SHADOW;
}

const bool CollisionManager::IsItem(const Collider::TAG _tag) const
{
	//今のところはプレイヤー以外がアイテム
	return !IsPlayer(_tag) && _tag != Collider::TAG::PUNCH;
}

CollisionManager::CollisionManager(void)
{
	updateFrame_ = 0;

	//**********************************************************
	//ここに当たり判定する範囲の広さをタグごとで設定する
	//**********************************************************

	hitRange_[Collider::TAG::PLAYER1] = HIT_RANGE_NORMAL;
	hitRange_[Collider::TAG::PLAYER2] = HIT_RANGE_NORMAL;
	hitRange_[Collider::TAG::PLAYER3] = HIT_RANGE_NORMAL;
	hitRange_[Collider::TAG::PLAYER4] = HIT_RANGE_NORMAL;
	hitRange_[Collider::TAG::PUNCH] = HIT_RANGE_NORMAL;
	hitRange_[Collider::TAG::NORMAL_ITEM] = HIT_RANGE_NORMAL;
	hitRange_[Collider::TAG::MOVE_HORI_FLOOR] = HIT_RANGE_NORMAL;
	hitRange_[Collider::TAG::MOVE_VER_FLOOR] = HIT_RANGE_NORMAL;
	hitRange_[Collider::TAG::KILLER_ALL] = HIT_RANGE_NORMAL;
	hitRange_[Collider::TAG::KILLER_SPECIFIC] = HIT_RANGE_NORMAL;
	hitRange_[Collider::TAG::DESTROYER] = HIT_RANGE_NORMAL;
	hitRange_[Collider::TAG::SLIME_FLOOR] = HIT_RANGE_NORMAL;
	hitRange_[Collider::TAG::SPRING] = HIT_RANGE_NORMAL;
	hitRange_[Collider::TAG::SHADOW] = HIT_RANGE_NORMAL;
	hitRange_[Collider::TAG::COIN] = HIT_RANGE_NORMAL;


	hitRange_[Collider::TAG::START] = HIT_RANGE_START;
	hitRange_[Collider::TAG::GOAL] = HIT_RANGE_GOAL;

	hitRange_[Collider::TAG::CANNON_AIM] = HIT_RANGE_TARGET;
	hitRange_[Collider::TAG::WIND] = HIT_RANGE_WIND;
}

CollisionManager::~CollisionManager(void)
{
}

const bool CollisionManager::IsWithInHitRange(const std::weak_ptr<Collider> _col1, const std::weak_ptr<Collider> _col2) const
{
	//総合
	bool ret = false;

	//双方の距離
	double sqrtDis = Utility::SqrMagnitude(
		_col1.lock()->GetGeometry().GetColPos(),
		_col2.lock()->GetGeometry().GetColPos());

	//双方のタグ
	for (const auto tag1 : _col1.lock()->GetTags())
	{
		for (const auto tag2 : _col2.lock()->GetTags())
		{
			//距離範囲の比較
			float range = hitRange_.at(tag1) >= hitRange_.at(tag2) ? hitRange_.at(tag1) : hitRange_.at(tag2);

			//範囲内かの比較
			ret = sqrtDis <= range * range;

			//当たったなら強制終了
			if (ret)
			{
				//当たった
				return true;
			}
		}
	}

	//当たらなかった
	return false;
}

const bool CollisionManager::JudgeIsCollision(const int _col1Num, const int _col2Num)const
{
	//範囲内か
	if (!IsWithInHitRange(colliders_[_col1Num], colliders_[_col2Num]))
	{
		//範囲内でなかった
		return false;
	}
	
	//双方のタグ
	for (auto tag1 : colliders_[_col1Num]->GetTags())
	{
		for (auto tag2 : colliders_[_col2Num]->GetTags())
		{
			//違うタグか
			if (tag1 == tag2)
			{
				//同じタグを持っていた
				return false;
			}

			//設定されたタグか
			if (!JudgeIsColTag(tag1, tag2))
			{
				//設定されたタグではなかった
				return false;
			}
		}
	}

	//1人目のタグ
	for (auto tag1 : colliders_[_col1Num]->GetTags())
	{
		//2人目の当たり判定しないタグ
		for (auto notColTag2 : colliders_[_col2Num]->GetNotHitTags())
		{
			if (tag1 == notColTag2)
			{
				//1人目のタグが2人目の当たり判定しないタグと同一だった
				return false;
			}
		}
	}

	//2人目のタグ
	for (auto tag2 : colliders_[_col2Num]->GetTags())
	{
		//1人目の当たり判定しないタグ
		for (auto notColTag1 : colliders_[_col1Num]->GetNotHitTags())
		{
			if (tag2 == notColTag1)
			{
				//2人目のタグが1人目の当たり判定しないタグと同一だった
				return false;
			}
		}
	}

	//全判定をクリアしたので当たり判定をする
	return true;
}

const bool CollisionManager::JudgeIsColTag(const Collider::TAG _tag1, const Collider::TAG _tag2) const
{
	//ここにタグごとの正確な判定の取る取らないを決める

	//総合
	bool ret = true;

	//どちらともアイテムなら
	if (IsItem(_tag1) && IsItem(_tag2))
	{
		//当たり判定しない
		ret = false;
	}

	return ret;
}

const bool CollisionManager::IsCollision(const std::weak_ptr<Collider> _col1, const std::weak_ptr<Collider> _col2)const
{
	//1つ目の当たり判定形状
	auto& geo1 = _col1.lock()->GetGeometry();
	
	//2つ目の当たり判定形状
	auto& geo2 = _col2.lock()->GetGeometry();

	//当たり判定
	bool ret = geo1.IsHit(geo2);
	
	//当たり判定
	return ret;
}