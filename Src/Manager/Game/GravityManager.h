#pragma once
#include <memory>
class GravityManager
{
public:
	GravityManager(void);
	~GravityManager(void)=default;
	//重力
	static constexpr float GRAVITY_POW = 10.0f;

	void Init(void);

	/// <summary>
	///重力計算
	/// </summary>
	/// <param name="dir">重力方向</param>
	/// <param name="pow">重力計算に使う変数</param>
	void CalcGravity(VECTOR& _dir,VECTOR& _pow);
private:

};

