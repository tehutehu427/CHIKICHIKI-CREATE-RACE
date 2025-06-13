#include"../Transform.h"
#include "Geometry.h"

//***************************************************
//Šî–{
//***************************************************

Geometry::~Geometry(void)
{
}

Geometry::Geometry(const Transform& _parent) : transformParent_(_parent)
{
}

const bool Geometry::IsHit(const Line& _line) const
{
	return false;
}

const bool Geometry::IsHit(Line& _line)
{
	return false;
}

const VECTOR Geometry::GetRotPos(const VECTOR& _localPos) const
{
	VECTOR localRotPos = transformParent_.quaRot.PosAxis(_localPos);
	return VAdd(transformParent_.pos, localRotPos);
}