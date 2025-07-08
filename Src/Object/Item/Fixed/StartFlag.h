#pragma once
#include "../ItemBase.h"
class StartFlag :
    public ItemBase
{
public:

	//マップ
	static constexpr IntVector3 MAP_SIZE = { 3,3,3 };					//マップサイズ
	static constexpr IntVector3 HIT_SIZE = { 0,2,0 };					//当たり判定用サイズ
	static constexpr VECTOR MAP_LOCALPOS = { 3500.0f,3500.0f,3500.0f };	//マップとの相対座標


	//サイズ
	static constexpr VECTOR MODEL_SIZE = { 7000.0f,7000.0f,7000.0f };	//モデルのサイズ

	//コンストラクタ
	StartFlag();
	//デストラクタ
	~StartFlag()override;

	//個人設定
	void SetParam(void)override;
	//更新
	void Update(void)override;

	//当たり判定サイズ
	const IntVector3 GetHitSize(void)const override { return MAP_SIZE + HIT_SIZE; }

	/// <summary>
	/// 当たり判定後の処理
	/// </summary>
	/// <param name="_hitColTag">相手側の当たり判定</param>
	void OnHit(const std::weak_ptr<Collider> _hitCol)override;

private:

	Transform colModelTrans_;	//当たり判定用モデル
};

