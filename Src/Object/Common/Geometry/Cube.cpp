#include "../Common/Quaternion.h"
#include"Model.h"
#include"Sphere.h"
#include"Capsule.h"
#include"Line.h"
#include"Cube.h"

//***************************************************
//箱
//***************************************************

Cube::Cube(const VECTOR& _pos, const Quaternion& _rot) : Geometry(_pos, _rot)
{
	halfSize_ = { 0.0f,0.0f,0.0f };

	bb_.axis[0] = { 1.0f,0.0f,0.0f };
	bb_.axis[1] = { 0.0f,1.0f,0.0f };
	bb_.axis[2] = { 0.0f,0.0f,1.0f };
}

Cube::Cube(const Cube& _copyBase, const VECTOR& _pos, const Quaternion& _rot) : Geometry(_pos, _rot)
{
	halfSize_ = _copyBase.GetHalfSize();

	bb_.axis[0] = { 1.0f,0.0f,0.0f };
	bb_.axis[1] = { 0.0f,1.0f,0.0f };
	bb_.axis[2] = { 0.0f,0.0f,1.0f };
}

Cube::~Cube(void)
{

}

void Cube::Draw(void)
{
	VECTOR min = GetRotPos(VScale(halfSize_, -1.0f));
	VECTOR max = GetRotPos(halfSize_);

	DrawCube3D(min, max, NORMAL_COLOR, NORMAL_COLOR, false);
}

const bool Cube::IsHit(const Geometry& _geometry) const
{
	bool ret = _geometry.IsHit(*this);

	return ret;
}

const bool Cube::IsHit(const Model& _model) const
{
	return false;
}

const bool Cube::IsHit(const Cube& _cube)const
{
	const float EPSILON = 1e-6f;

	VECTOR axisToTest[15];
	int axisCount = 0;

	// 3軸 + 3軸
	for (int i = 0; i < 3; ++i) {
		axisToTest[axisCount++] = bb_.axis[i];
		axisToTest[axisCount++] = _cube.bb_.axis[i];
	}

	// 外積軸（9本）
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			VECTOR cross = VCross(bb_.axis[i], _cube.bb_.axis[j]);
			if (std::sqrt(cross.x * cross.x + cross.y * cross.y + cross.z * cross.z) > EPSILON) {
				axisToTest[axisCount++] = VNorm(cross);
			}
		}
	}

	// 各軸で判定
	for (int i = 0; i < axisCount; ++i) {
		VECTOR L = axisToTest[i];

		float aProj =
			fabs(VDot(L, bb_.axis[0]) * halfSize_.x) +
			fabs(VDot(L, bb_.axis[1]) * halfSize_.y) +
			fabs(VDot(L, bb_.axis[2]) * halfSize_.z);

		float bProj =
			fabs(VDot(L, _cube.bb_.axis[0]) * _cube.halfSize_.x) +
			fabs(VDot(L, _cube.bb_.axis[1]) * _cube.halfSize_.y) +
			fabs(VDot(L, _cube.bb_.axis[2]) * _cube.halfSize_.z);

		float dist = fabs(VDot(L, VSub(_cube.GetColPos(), pos_)));

		if (dist > (aProj + bProj)) {
			return false; // 衝突していない
		}
	}

	return true; // すべての軸で分離できなかった → 衝突している
}

const bool Cube::IsHit(const Sphere& _sphere) const
{
	return false;
}

const bool Cube::IsHit(const Capsule& _capsule) const
{
	return false;
}

const bool Cube::IsHit(Line& _line)
{
	return false;
}
