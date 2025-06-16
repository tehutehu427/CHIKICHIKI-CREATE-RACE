#include<DxLib.h>
#include"../Utility/Utility.h"
#include "GravityManager.h"

GravityManager* GravityManager::instance_ = nullptr;
GravityManager::GravityManager(void)
{
}

GravityManager::~GravityManager(void)
{
}

void GravityManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new GravityManager();
	}
	instance_->Init();
}

GravityManager& GravityManager::GetInstance(void)
{
	if (instance_ == nullptr)
	{
		GravityManager::CreateInstance();
	}
	return *instance_;
}

void GravityManager::Destroy(void)
{
	if (instance_ != nullptr)
	{
		delete instance_;
		instance_ = nullptr;
	}
}

void GravityManager::Init(void)
{
	slerpPow_ = 0.0f;
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

	Calculate();

	//// 内積
	//float dot = VDot(dirGravity, _pow);
	//if (dot >= 0.0f)
	//{
	//	//重力方向と反対方向(マイナス)でなければ、ジャンプ力を無くす
	//	_pow = gravity;
	//}
}

void GravityManager::Calculate(void)
{
	//重力方向
	VECTOR dirGravity = Utility::DIR_D;
	//重力の逆方向
	VECTOR dirUpGravity = Utility::ReverseValue(dirGravity);

	// 現在の上方向(つまり、重力の反対方向)
	VECTOR up = trans_.GetUp();

	// ２つのベクトル間の回転量(差)を求める
	Quaternion rot = Quaternion::FromToRotation(up, dirUpGravity);

	// 求めた回転量で、現在の重力制御を回転させる(差が埋まる)
	trans_.quaRot = Quaternion::Slerp(trans_.quaRot, rot.Mult(trans_.quaRot), slerpPow_);
}
