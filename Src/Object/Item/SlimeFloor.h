#pragma once
#include "ItemBase.h"
#include "../../Common/IntVector3.h"

class SlimeFloor : public ItemBase
{
public:

	//マップサイズ
	static constexpr IntVector3 MAP_SIZE = { 4,1,4 };

	//コンストラクタ
	SlimeFloor(void);

	//デストラクタ
	~SlimeFloor(void)override;

	//個人設定
	void SetParam(void)override;

	//更新
	void Update(void)override;

private:

};

