#pragma once
#include "ItemBase.h"
#include "../../Common/IntVector3.h"

class SlimeFloor : public ItemBase
{
public:

	//マップサイズ
	static constexpr IntVector3 MAP_SIZE = { 4,1,4 };

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name=""></param>
	SlimeFloor(void);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~SlimeFloor(void)override = default;

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

