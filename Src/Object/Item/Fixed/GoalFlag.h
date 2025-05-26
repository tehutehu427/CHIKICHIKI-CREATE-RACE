#pragma once
#include "../ItemBase.h"
class GoalFlag :
    public ItemBase
{
public:

	//マップ
	static constexpr IntVector3 MAP_SIZE = { 3,3,3 };	//マップサイズ

	//コンストラクタ
	GoalFlag();
	//デストラクタ
	~GoalFlag()override;

	//個人設定
	void SetParam(void)override;
	//更新
	void Update(void)override;

private:

};

