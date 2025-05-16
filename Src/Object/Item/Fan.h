#pragma once
#include "ItemBase.h"
class Fan :
    public ItemBase
{
public:

	//マップサイズ
	static constexpr IntVector3 MAP_SIZE = { 4,3,4 };

	//コンストラクタ
	Fan();
	//デストラクタ
	~Fan()override;

	//個人設定
	void SetParam(void)override;
	//更新
	void Update(void)override;

private:
};

