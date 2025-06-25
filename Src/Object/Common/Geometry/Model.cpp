#include "../Common/Quaternion.h"
#include"Sphere.h"
#include"Capsule.h"
#include"Cube.h"
#include"Line.h"
#include"Model.h"

//***************************************************
//モデル
//***************************************************

Model::Model(const VECTOR& _pos, const Quaternion& _rot, const int _modelId) : Geometry(_pos, _rot), parentModelId_(_modelId)
{
	hitLineInfo_ = {};
	hitInfo_ = {};
}

Model::Model(const Model& _copyBase, const VECTOR& _pos, const Quaternion& _rot) : Geometry(_pos,_rot)
{
	parentModelId_ = _copyBase.GetParentModel();
	hitLineInfo_ = {};
	hitInfo_ = {};
}

Model::~Model(void)
{
	//当たり判定情報の解放(球、カプセル)
	//MV1CollResultPolyDimTerminate(hitInfo_);
}

void Model::Draw(void)
{
}

const bool Model::IsHit(Geometry& _geometry)
{
	bool ret = _geometry.IsHit(*this);

	return ret;
}

const bool Model::IsHit(Model& _model)
{
	return false;
}

const bool Model::IsHit(Cube& _cube)
{
	return false;
}

const bool Model::IsHit(Sphere& _sphere)
{
	//判定
	auto col = MV1CollCheck_Sphere(GetParentModel(), -1, _sphere.GetColPos(), _sphere.GetRadius());

	//当たったか
	bool ret = col.HitNum >= 1;

	//当たっていたら情報更新
	if (ret)
	{
		_sphere.SetHitInfo(col);
		SetHitInfo(col);
	}

	return ret;
}

const bool Model::IsHit(Capsule& _capsule)
{
	//判定
	auto col = MV1CollCheck_Capsule(GetParentModel(), -1, _capsule.GetPosTop(), _capsule.GetPosDown(), _capsule.GetRadius());

	//当たったか
	bool ret = col.HitNum >= 1;

	//当たっていたら情報更新
	if (ret)
	{
		_capsule.SetHitInfo(col);
		SetHitInfo(col);
	}

	return ret;
}

const bool Model::IsHit(Line& _line)
{
	//判定
	auto col = MV1CollCheck_Line(GetParentModel(), -1, _line.GetPosPoint1(), _line.GetPosPoint2());

	//当たっていたら情報更新
	if (col.HitFlag)
	{
		_line.SetHitInfo(col);
		SetHitLineInfo(col);
	}
	return col.HitFlag;
}