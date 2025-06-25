#include "Vector2.h"

// コンストラクタ
Vector2::Vector2(void)
{
	x = 0;
	y = 0;
}

// コンストラクタ
Vector2::Vector2(int vX, int vY)
{
	x = vX;
	y = vY;
}

Vector2::~Vector2(void)
{
}

Vector2 Vector2::AddVector2(const Vector2& _value1, const Vector2& _value2)
{
	return Vector2{ _value1.x + _value2.x,_value1.y + _value2.y };
}

Vector2 Vector2::SubVector2(const Vector2& _value1, const Vector2& _value2)
{
	return Vector2{ _value1.x - _value2.x,_value1.y - _value2.y };
}
