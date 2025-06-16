#pragma once
#include <memory>
#include"../Object/Common/Transform.h"
class GravityManager
{
public:
	GravityManager(void);
	~GravityManager(void);
	//重力
	static constexpr float GRAVITY_POW = 3.0f;

	//静的にインスタンス生成
	static void CreateInstance(void);

	//静的にインスタンスを取得する
	static GravityManager& GetInstance(void);

	static void Destroy(void);

	void Init(void);


	/// <summary>
	///重力計算(ジャンプ行動など)
	/// </summary>
	/// <param name="dir">重力方向</param>
	/// <param name="pow">重力計算に使う変数</param>
	/// <param name="per">重力倍率</param>
	void CalcGravity(VECTOR& _dir, VECTOR& _pow,float _per = 100.0f);

	//重力計算
	void Calculate(void);

private:
	static GravityManager* instance_;

	//回転用
	Transform trans_;

	//回転力
	float slerpPow_;
};

