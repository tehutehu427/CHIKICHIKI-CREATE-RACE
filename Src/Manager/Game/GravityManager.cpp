#include<DxLib.h>
#include "GravityManager.h"

std::unique_ptr<GravityManager> GravityManager::instance_ = nullptr;
GravityManager::GravityManager(void)
{
}

void GravityManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = std::make_unique<GravityManager>();
	}
	instance_->Init();
}

std::unique_ptr<GravityManager>& GravityManager::GetInstance(void)
{
	if (instance_ == nullptr)
	{
		GravityManager::CreateInstance();
	}
	return instance_;
}

void GravityManager::Init(void)
{
}

void GravityManager::CalcGravity(VECTOR& _dir, VECTOR& _pow, float _per)
{
	// 重力方向
	VECTOR dirGravity = _dir;
	// 重力の強さ
	float gravityPow = GRAVITY_POW * _per / 100.0f;
	// 重力
	// 重力を作る
	//_pow に重力計算を行う(加速度)
	VECTOR gravity = VScale(dirGravity, gravityPow);
	_pow = VAdd(_pow, gravity);


	//// 内積
	//float dot = VDot(dirGravity, _pow);
	//if (dot >= 0.0f)
	//{
	//	//重力方向と反対方向(マイナス)でなければ、ジャンプ力を無くす
	//	_pow = gravity;
	//}
}