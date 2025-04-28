#include "CannonShot.h"

CannonShot::CannonShot(VECTOR _pos, Quaternion _quaRot)
{
	transform_.pos = _pos;
	transform_.quaRot = _quaRot;
}

CannonShot::~CannonShot()
{
}

void CannonShot::SetParam(void)
{
	size_ = { 0,0,0 };

}

void CannonShot::Update(void)
{
}

void CannonShot::Draw(void)
{
}
