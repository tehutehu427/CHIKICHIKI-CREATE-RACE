#pragma once
#include"../Object/Common/Transform.h"

class GravityManager
{

public:

	//重力
	static constexpr float GRAVITY_POW = 3.0f;

	/// <summary>
	/// 静的にインスタンス生成
	/// </summary>
	/// <param name=""></param>
	static void CreateInstance(void);

	//静的にインスタンスを取得する
	static GravityManager& GetInstance(void);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name=""></param>
	void Init(void);

	/// <summary>
	/// 解放
	/// </summary>
	/// <param name=""></param>
	void Destroy(void);

	/// <summary>
	///重力計算(ジャンプ行動など)
	/// </summary>
	/// <param name="dir">重力方向</param>
	/// <param name="pow">重力計算に使う変数</param>
	/// <param name="per">重力倍率</param>
	void CalcGravity(VECTOR& _dir, VECTOR& _pow,float _per = 100.0f);



private:

	//インスタンス
	static GravityManager* instance_;

	/// コンストラクタ
	GravityManager(void);
	//デストラクタ
	~GravityManager(void)=default;
	//コピーコンストラクタ
	GravityManager& operator=(const GravityManager& _copy) = default;
	GravityManager(const GravityManager& _copy) = delete;

	 //重力計算
	void Calculate(void);
	//回転用
	Transform trans_;
	//回転力
	float slerpPow_;
};

