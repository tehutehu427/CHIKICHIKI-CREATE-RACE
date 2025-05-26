#pragma once
#include "../ItemBase.h"

class Fence : public ItemBase
{
public:

	//マップ
	static constexpr IntVector3 MAP_SIZE = { 2,1,1 };	//マップサイズ
	static constexpr VECTOR MAP_LOCALPOS = { 100.0f,60.0f,10.0f };	//マップとの相対座標

	//コンストラクタ
	Fence();
	//デストラクタ
	~Fence()override;

	//個人設定
	void SetParam(void)override;
	//更新
	void Update(void)override;
	//描画
	void Draw(void)override;

private:

};

