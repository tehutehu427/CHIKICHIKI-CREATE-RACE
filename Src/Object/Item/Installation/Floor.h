#pragma once
#include "../ItemBase.h"

class Floor : public ItemBase
{

public:

	//マップ
	static constexpr IntVector3 MAP_SIZE = { 4,1,4 };				//マップサイズ
	static constexpr VECTOR MAP_LOCALPOS = { 220.0f,15.0f,220.0f };	//マップとの相対座標

	//モデルのサイズ
	static constexpr VECTOR MODEL_SIZE = { 440.0f,30.0f,440.0f };	

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name=""></param>
	Floor(void);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~Floor(void)override;

	/// <summary>
	/// 個人設定
	/// </summary>
	/// <param name=""></param>
	void SetParam(void)override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name=""></param>
	void Update(void)override;

	/// <summary>
	/// 当たり判定後の処理
	/// </summary>
	/// <param name="_hitColTag">相手側の当たり判定</param>
	void OnHit(const std::weak_ptr<Collider> _hitCol)override;
};

