#include "CollisionResolverBase.h"

CollisionResolverBase::CollisionResolverBase(VECTOR& _moveDiff, VECTOR& _currentPos, VECTOR& _movedPos
	, PlayerAction& _action
	, std::vector<ObjectBase::ColParam>& _colParam):
	moveDiff_(_moveDiff)
	,currentPos_(_currentPos)
	,movedPos_(_movedPos)
	,action_(_action)
	,colParam_(_colParam)
{
}

CollisionResolverBase::~CollisionResolverBase(void)
{
}

void CollisionResolverBase::Resolve(void)
{
}

void CollisionResolverBase::Resolve(const std::weak_ptr<Collider> _hitCol)
{
}

void CollisionResolverBase::HitUpdownLine(void)
{
}

void CollisionResolverBase::HitMoveLine(void)
{
}

void CollisionResolverBase::HitBodyShere(const std::weak_ptr<Collider> _hitCol)
{
}

void CollisionResolverBase::HitBodyShere(void)
{
}
