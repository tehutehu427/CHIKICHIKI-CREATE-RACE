#include "../ObjectBase.h"
#include "Geometry/Geometry.h"
#include "Collider.h"

Collider::Collider(ObjectBase& _parent, const std::vector<TAG> _tags, Geometry& _geometry) :
	parent_(_parent),
	tags_(_tags),
	geometry_(_geometry)
{
	isHit_ = false;
	isDead_ = false;
}

Collider::~Collider(void)
{
}

void Collider::OnHit(const std::weak_ptr<Collider> _collider)
{
	//この当たり判定が当たった
	isHit_ = true;

	//親に相手のコライダを渡す
	parent_.OnHit(_collider);
}
