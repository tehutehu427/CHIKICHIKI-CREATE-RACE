#pragma once
#include "../ItemBase.h"
class SmallBomb : public ItemBase
{
public:

	//マップ
	static constexpr IntVector3 MAP_SIZE = { 2,2,2 };	//マップサイズ

	//コンストラクタ
	SmallBomb();
	//デストラクタ
	~SmallBomb()override;

	//個人設定
	void SetParam(void)override;
	//更新
	void Update(void)override;

private:
};

