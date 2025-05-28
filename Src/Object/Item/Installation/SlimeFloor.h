#pragma once
#include "../ItemBase.h"
#include "../../../Common/IntVector3.h"

class SlimeFloor : public ItemBase
{
public:

	//マップ
	static constexpr IntVector3 MAP_SIZE = { 4,1,4 };				//マップサイズ
	static constexpr VECTOR MAP_LOCALPOS = { 220.0f,15.0f,220.0f };	//マップとの相対座標

	//サイズ
	static constexpr VECTOR MODEL_SIZE = { 440.0f,30.0f,440.0f };	//モデルのサイズ

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name=""></param>
	SlimeFloor(void);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~SlimeFloor(void);

	/// <summary>
	/// パラメーター設定
	/// </summary>
	/// <param name=""></param>
	void SetParam(void)override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name=""></param>
	void Update(void)override;
};

