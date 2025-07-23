#pragma once
#include<DxLib.h>
#include<memory>
#include"../Object/ObjectBase.h"
#include"../Player.h"
class PlayerAction;
class CollisionResolverBase
{
public:
	struct POSITION_INFO
	{
		VECTOR& moveDiff;	//移動前
		VECTOR& currentPos;	//現在
		VECTOR& movedPos;		//移動後
	};
	CollisionResolverBase(VECTOR& moveDiff, VECTOR& currentPos, VECTOR& movedPos
		, PlayerAction& _action
		, std::vector<ObjectBase::ColParam>& _colParam);
	virtual ~CollisionResolverBase(void) = 0;
	virtual void Resolve(void);
	virtual void Resolve(const std::weak_ptr<Collider> _hitCol);
	virtual void HitUpdownLine(void);
	virtual void HitMoveLine(void);
	virtual void HitBodyShere(void);
	virtual void HitBodyShere(const std::weak_ptr<Collider> _hitCol);
protected:
	VECTOR& moveDiff_;	//移動前
	VECTOR& currentPos_;	//現在
	VECTOR& movedPos_;		//移動後
	//当たり判定関係
	std::vector<ObjectBase::ColParam>& colParam_;

	//プレイヤー
	PlayerAction& action_;
private:
};

