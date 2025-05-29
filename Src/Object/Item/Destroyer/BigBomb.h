#pragma once
#include "../ItemBase.h"
class BigBomb : public ItemBase
{
public:

	//マップ
	static constexpr IntVector3 MAP_SIZE = { 5,5,5 };	//マップサイズ

	//コンストラクタ
	BigBomb();
	//デストラクタ
	~BigBomb()override;

	//個人設定
	void SetParam(void)override;
	//更新
	void Update(void)override;

private:
};

