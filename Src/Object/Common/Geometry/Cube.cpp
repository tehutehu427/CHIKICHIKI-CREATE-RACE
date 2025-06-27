#include "../Common/Quaternion.h"
#include"Model.h"
#include"Sphere.h"
#include"Capsule.h"
#include"Line.h"
#include"Cube.h"

//***************************************************
//箱
//***************************************************

Cube::Cube(const VECTOR& _pos, const Quaternion& _rot, const VECTOR _min, const VECTOR _max) : Geometry(_pos, _rot)
{
	obb_.vMin = _min;
	obb_.vMax = _max;

	UpdateObbAxis();
}

Cube::Cube(const VECTOR& _pos, const Quaternion& _rot, const VECTOR _halfSize) : Geometry(_pos, _rot)
{
	obb_.vMin = VScale(_halfSize, -1.0f);
	obb_.vMax = _halfSize;

	UpdateObbAxis();
}

Cube::Cube(const Cube& _copyBase, const VECTOR& _pos, const Quaternion& _rot) : Geometry(_pos, _rot),obb_(_copyBase.obb_)
{
	UpdateObbAxis();
}

Cube::~Cube(void)
{

}

void Cube::Draw(void)
{
	VECTOR vertices[8];
	CalculateVertices(vertices);

	// 12本のエッジのインデックス
	static const int edges[12][2] = {
		{0,1},{0,2},{0,4}, {1,3},{1,5},
		{2,3},{2,6}, {3,7},
		{4,5},{4,6}, {5,7},{6,7}
	};

	for (int i = 0; i < 12; ++i)
	{
		DrawLine3D(vertices[edges[i][0]], vertices[edges[i][1]], NORMAL_COLOR);
	}
}

const bool Cube::IsHit(Geometry& _geometry)
{
	bool ret = _geometry.IsHit(*this);

	return ret;
}

const bool Cube::IsHit(Model& _model)
{
	return false;
}

const bool Cube::IsHit(Cube& _cube)
{
	return false;
}

const bool Cube::IsHit(Sphere& _sphere)
{
	return false;
}

const bool Cube::IsHit(Capsule& _capsule)
{
	return false;
}

const bool Cube::IsHit(Line& _line)
{
	return false;
}

void Cube::SetHalfSize(const VECTOR& _halfSize)
{
	obb_.vMin = VScale(_halfSize, -1.0f);
	obb_.vMax = _halfSize;
}

void Cube::UpdateObbAxis(void) 
{
	MATRIX rotMat;
	rotMat = quaRot_.ToMatrix();

	obb_.axis[0] = VTransform(VGet(1, 0, 0), rotMat); // Right
	obb_.axis[1] = VTransform(VGet(0, 1, 0), rotMat); // Up
	obb_.axis[2] = VTransform(VGet(0, 0, 1), rotMat); // Forward
}

void Cube::CalculateVertices(VECTOR outVertices[8]) const
{
	MATRIX rotMat;
	rotMat = quaRot_.ToMatrix();

	int idx = 0;
	for (int x = 0; x <= 1; ++x)
	{
		for (int y = 0; y <= 1; ++y)
		{
			for (int z = 0; z <= 1; ++z)
			{
				VECTOR local;
				local.x = (x == 0) ? obb_.vMin.x : obb_.vMax.x;
				local.y = (y == 0) ? obb_.vMin.y : obb_.vMax.y;
				local.z = (z == 0) ? obb_.vMin.z : obb_.vMax.z;

				VECTOR world = VTransform(local, rotMat);
				world = VAdd(world, pos_);

				outVertices[idx++] = world;
			}
		}
	}
}