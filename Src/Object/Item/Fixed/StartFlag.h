#pragma once
#include "../ItemBase.h"

class StartFlag : public ItemBase
{

public:

	//マップ
	static constexpr IntVector3 MAP_SIZE = { 3,3,3 };					//マップサイズ
	static constexpr IntVector3 HIT_SIZE = { 0,2,0 };					//当たり判定用サイズ
	static constexpr VECTOR MAP_LOCALPOS = { 3500.0f,3500.0f,3500.0f };	//マップとの相対座標

	//モデルのサイズ
	static constexpr VECTOR MODEL_SIZE = { 7000.0f,7000.0f,7000.0f };	

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name=""></param>
	StartFlag(void);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~StartFlag(void)override;

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
	/// 当たり判定サイズの取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>当たり判定サイズ(マップ座標基準)</returns>
	const IntVector3 GetHitSize(void)const override { return MAP_SIZE + HIT_SIZE; }

	/// <summary>
	/// 当たり判定後の処理
	/// </summary>
	/// <param name="_hitColTag">相手側の当たり判定</param>
	void OnHit(const std::weak_ptr<Collider> _hitCol)override;

private:

	//当たり判定用モデル
	Transform colModelTrans_;	
};

