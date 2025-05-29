#include <DxLib.h>
#include "IntVector3.h"

//IntVector3::IntVector3(void)
//{
//	x = 0;
//	y = 0;
//	z = 0;
//}
//
//IntVector3::IntVector3(int vX, int vY, int vZ)
//{
//	x = vX;
//	y = vY;
//	z = vZ;
//}
//
//IntVector3::~IntVector3(void)
//{
//}

const IntVector3 IntVector3::operator+(const IntVector3 _value)const
{
	return { x + _value.x , y + _value.y, z + _value.z};
}

void IntVector3::operator+=(const IntVector3 _value)
{
	x += _value.x;
	y += _value.y;
	z += _value.z;
}

const IntVector3 IntVector3::operator-(const IntVector3 _value)const
{
	return { x - _value.x , y - _value.y, z - _value.z };
}

void IntVector3::operator-=(const IntVector3 _value)
{
	x -= _value.x;
	y -= _value.y;
	z -= _value.z;
}

const IntVector3 IntVector3::operator*(const int _value)const
{
	return { x * _value , y * _value, z * _value };
}

void IntVector3::operator*=(const int _value)
{
	x *= _value;
	y *= _value;
	z *= _value;
}

const IntVector3 IntVector3::operator/(const int _value)const
{
	return { x / _value , y / _value, z / _value };
}

void IntVector3::operator/=(const int _value)
{
	x /= _value;
	y /= _value;
	z /= _value;
}

const bool IntVector3::operator==(const IntVector3 _value)
{
	bool diffX = x == _value.x;
	bool diffY = y == _value.y;
	bool diffZ = z == _value.z;

	return diffX && diffY && diffZ;
}

const bool IntVector3::operator<(const IntVector3 _value)
{
	bool diffX = x < _value.x;
	bool diffY = y < _value.y;
	bool diffZ = z < _value.z;

	return diffX && diffY && diffZ;
}

const bool IntVector3::operator<=(const IntVector3 _value)
{
	bool diffX = x <= _value.x;
	bool diffY = y <= _value.y;
	bool diffZ = z <= _value.z;

	return diffX && diffY && diffZ;
}

const bool IntVector3::operator>(const IntVector3 _value)
{
	bool diffX = x > _value.x;
	bool diffY = y > _value.y;
	bool diffZ = z > _value.z;

	return diffX && diffY && diffZ;
}

const bool IntVector3::operator>=(const IntVector3 _value)
{
	bool diffX = x >= _value.x;
	bool diffY = y >= _value.y;
	bool diffZ = z >= _value.z;

	return diffX && diffY && diffZ;
}

VECTOR IntVectorToVECTOR(IntVector3 _iv)
{
	float vx = static_cast<float>(_iv.x);
	float vy = static_cast<float>(_iv.y);
	float vz = static_cast<float>(_iv.z);

	return VGet(vx, vy, vz);
}

IntVector3 VECTORToIntVector(VECTOR _v)
{
	IntVector3 intVec = INT_VECTOR_ZERO;

	intVec.x = static_cast<int>(_v.x);
	intVec.y = static_cast<int>(_v.y);
	intVec.z = static_cast<int>(_v.z);

	return intVec;
}
