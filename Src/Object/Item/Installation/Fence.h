#pragma once
#include "../ItemBase.h"

class Fence : public ItemBase
{
public:

	//マップ
	static constexpr IntVector3 MAP_SIZE = { 2,1,1 };				//マップサイズ
	static constexpr VECTOR MAP_LOCALPOS = { 110.0f,70.0f,10.0f };	//マップとの相対座標

	//サイズ
	static constexpr VECTOR MODEL_SIZE = { 220.0f,140.0f,20.0f };	//モデルのサイズ

	//コンストラクタ
	Fence();
	//デストラクタ
	~Fence()override;

	//個人設定
	void SetParam(void)override;
	//更新
	void Update(void)override;

private:

};

