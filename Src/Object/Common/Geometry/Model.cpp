#include "../Common/Quaternion.h"
#include"Sphere.h"
#include"Capsule.h"
#include"Cube.h"
#include"Line.h"
#include"Model.h"

//***************************************************
//ƒ‚ƒfƒ‹
//***************************************************

Model::Model(const VECTOR& _pos, const Quaternion& _rot, const int _modelId) : Geometry(_pos, _rot), parentModelId_(_modelId)
{
	hitInfo_ = {};
}

Model::Model(const Model& _copyBase, const VECTOR& _pos, const Quaternion& _rot) : Geometry(_pos,_rot)
{
	parentModelId_ = _copyBase.GetParentModel();
	hitInfo_ = {};
}

Model::~Model(void)
{

}

void Model::Draw(void)
{
}

const bool Model::IsHit(const Geometry& _geometry) const
{
	bool ret = _geometry.IsHit(*this);

	return ret;
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
	auto col = MV1CollCheck_Sphere(GetParentModel(), -1, _sphere.GetColPos(), _sphere.GetRadius());

	return col.HitNum >= 1;
}

const bool Model::IsHit(const Capsule& _capsule) const
{
	//”»’è
	auto col = MV1CollCheck_Capsule(GetParentModel(), -1, _capsule.GetPosTop(), _capsule.GetPosDown(), _capsule.GetRadius());

	return col.HitNum >= 1;
}

const bool Model::IsHit(Line& _line)
{
	//”»’è
	auto col = MV1CollCheck_Line(GetParentModel(), -1, _line.GetPosPoint1(), _line.GetPosPoint2());

	//“–‚½‚Á‚Ä‚¢‚½‚çî•ñXV
	if (col.HitFlag)
	{
		_line.SetHitInfo(col);
		SetHitInfo(col);
	}
	return col.HitFlag;
}