#include"../Transform.h"
#include"Sphere.h"
#include"Capsule.h"
#include"Cube.h"
#include"Line.h"
#include"Model.h"

//***************************************************
//ƒ‚ƒfƒ‹
//***************************************************

Model::Model(const Transform& _parent) : Geometry(_parent)
{
	parentModel_ = &transformParent_.modelId;
}

Model::Model(const Model& _copyBase, const Transform& _parent) : Geometry(_parent)
{
	parentModel_ = _copyBase.GetParentModel();
}

Model::~Model(void)
{
	delete parentModel_;
	parentModel_ = nullptr;
}

void Model::Draw(void)
{
}

const bool Model::IsHit(const Geometry& _geometry) const
{
	return _geometry.IsHit(*this);
}

const bool Model::IsHit(const Model& _model) const
{
	return false;
}

const bool Model::IsHit(const Cube& _cube) const
{
	return false;
}

const bool Model::IsHit(const Sphere& _sphere) const
{
	//”»’è
	auto col = MV1CollCheck_Sphere(*GetParentModel(), -1, _sphere.GetTransParent().pos, _sphere.GetRadius());

	return col.HitNum >= 1;
}

const bool Model::IsHit(const Capsule& _capsule) const
{
	//”»’è
	auto col = MV1CollCheck_Capsule(*GetParentModel(), -1, _capsule.GetPosTop(), _capsule.GetPosDown(), _capsule.GetRadius());

	return col.HitNum >= 1;
}

const bool Model::IsHit(Line& _line)
{
	//”»’è
	auto col = MV1CollCheck_Line(*GetParentModel(), -1, _line.GetPosPoint1(), _line.GetPosPoint2());

	//“–‚½‚Á‚Ä‚¢‚½‚çî•ñXV
	if (col.HitFlag)
	{
		_line.SetHitInfo(col);
		SetHitInfo(col);
	}
	return col.HitFlag;
}