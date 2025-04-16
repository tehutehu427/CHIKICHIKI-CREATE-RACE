#pragma once
#include <memory>
class GravityManager
{
public:
	GravityManager(void);
	~GravityManager(void)=default;
	//重力
	static constexpr float GRAVITY_POW = 10.0f;

	//静的にインスタンス生成
	static void CreateInstance(void);

	//静的にインスタンスを取得する
	static std::unique_ptr<GravityManager>& GetInstance(void);



	void Init(void);


	/// <summary>
	///重力計算
	/// </summary>
	/// <param name="dir">重力方向</param>
	/// <param name="pow">重力計算に使う変数</param>
	/// <param name="per">重力倍率</param>
	/// <param name="per">重力倍率</param>
	void CalcGravity(VECTOR& _dir, VECTOR& _pow,float _per = 100.0f);

private:
	static std::unique_ptr<GravityManager> instance_;
};

