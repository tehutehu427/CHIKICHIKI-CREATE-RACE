#include<DxLib.h>
#include "GravityManager.h"

GravityManager::GravityManager(void)
{
}

void GravityManager::Init(void)
{
}

void GravityManager::CalcGravity(VECTOR& _dir, VECTOR& _pow)
{
	// 重力方向
	VECTOR dirGravity = _dir;
	// 重力の強さ
	float gravityPow = GRAVITY_POW;
	// 重力
	// 重力を作る
	// メンバ変数 jumpPow_ に重力計算を行う(加速度)
	VECTOR gravity = VScale(dirGravity, gravityPow);
	_pow = VAdd(_pow, gravity);


	// 内積
	float dot = VDot(dirGravity, _pow);
	if (dot >= 0.0f)
	{
		// 重力方向と反対方向(マイナス)でなければ、ジャンプ力を無くす
		_pow = gravity;
	}
}

