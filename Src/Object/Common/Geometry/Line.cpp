#include "../Common/Quaternion.h"
#include"Model.h"
#include"Cube.h"
#include"Sphere.h"
#include"Capsule.h"
#include"Line.h"

//***************************************************
//ê¸
//***************************************************

Line::Line(const VECTOR& _pos, const Quaternion& _rot, const VECTOR _localPosPoint1, const VECTOR _localPosPoint2) : Geometry(_pos,_rot),
	localPosPoint1_(_localPosPoint1),
	localPosPoint2_(_localPosPoint2)
{
	hitInfo_ = {};
}

Line::Line(const Line& _copyBase, const VECTOR& _pos, const Quaternion& _rot) : Geometry(_pos, _rot)
{
	localPosPoint1_ = _copyBase.GetLocalPosPoint1();
	localPosPoint2_ = _copyBase.GetLocalPosPoint2();
	hitInfo_ = {};
}

Line::~Line(void)
{
}

void Line::Draw(void)
{
	VECTOR point1 = GetRotPos(localPosPoint1_);
	VECTOR point2 = GetRotPos(localPosPoint2_);

	DrawLine3D(point1, point2, NORMAL_COLOR);
}

const bool Line::IsHit(Geometry& _geometry)
{
	bool ret = _geometry.IsHit(*this);

	return ret;
}

const bool Line::IsHit(Model& _model)
{
	bool ret = _model.IsHit(*this);

	return ret;
}

const bool Line::IsHit(Cube& _cube)
{
	return false;
}

const bool Line::IsHit(Sphere& _sphere)
{
	bool ret = _sphere.IsHit(*this);

	return ret;
}

const bool Line::IsHit(Capsule& _capsule)
{
	bool ret = _capsule.IsHit(*this);

	return ret;
}

const bool Line::IsHit(Line& _line)
{
	//å¿ÇËÇ»Ç≠è¨Ç≥Ç¢êî
	const float EPSILON = 1e-6f;

	VECTOR u = VSub(_line.GetPosPoint2(), _line.GetPosPoint1());
	VECTOR v = VSub(GetPosPoint2(), GetPosPoint1());
	VECTOR w = VSub(_line.GetPosPoint1(), GetPosPoint1());

	float a = VDot(u, u);
	float b = VDot(u, v);
	float c = VDot(v, v);
	float d = VDot(u, w);
	float e = VDot(v, w);

	float denom = a * c - b * b;
	float s = 0.0f, t = 0.0f;

	if (denom != 0.0f) {
		s = std::clamp((b * e - c * d) / denom, 0.0f, 1.0f);
	}

	t = (b * s + e) / c;
	t = std::clamp(t, 0.0f, 1.0f);

	s = (b * t - d) / a;
	s = std::clamp(s, 0.0f, 1.0f);

	VECTOR closest1 = VAdd(_line.GetPosPoint1(), VScale(u, s));
	VECTOR closest2 = VAdd(GetPosPoint1(), VScale(v, t));
	VECTOR diff = VSub(closest1, closest2);

	//ãóó£
	float distance = sqrt(VDot(diff, diff));

	//åç∑ÇµÇΩ
	return distance <= EPSILON;
}