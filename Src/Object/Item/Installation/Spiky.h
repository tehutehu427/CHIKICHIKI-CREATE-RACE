#pragma once
#include "../ItemBase.h"
class Spiky : public ItemBase
{
public:

	//マップ
	static constexpr IntVector3 MAP_SIZE = { 1,1,1 };	//マップサイズ

	//コンストラクタ
	Spiky();
	//デストラクタ
	~Spiky()override;

	//個人設定
	void SetParam(void)override;
	//更新
	void Update(void)override;

private:
};

