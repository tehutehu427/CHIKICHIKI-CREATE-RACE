#pragma once
#include "../ItemBase.h"
#include "../../../Common/IntVector3.h"

class SlimeFloor : public ItemBase
{
public:


	//マップ
	static constexpr IntVector3 MAP_SIZE = { 4,1,4 };				//マップサイズ
	static constexpr VECTOR MAP_LOCALPOS = { 200.0f,15.0f,200.0f };	//マップとの相対座標

	//サイズ
	static constexpr VECTOR MODEL_SIZE = { 400.0f,30.0f,400.0f };	//モデルのサイズ

	//コンストラクタ
	SlimeFloor();
	//デストラクタ
	~SlimeFloor()override;

	//個人設定
	void SetParam(void)override;
	//更新
	void Update(void)override;

	/// <summary>
	/// 当たり判定後の処理
	/// </summary>
	/// <param name="_hitColTag">相手側の当たり判定</param>
	void OnHit(const std::weak_ptr<Collider> _hitCol)override;

};

