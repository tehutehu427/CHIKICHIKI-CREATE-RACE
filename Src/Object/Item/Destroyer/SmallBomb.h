#pragma once
#include "../ItemBase.h"
class SmallBomb : public ItemBase
{
public:

	//マップ
	static constexpr IntVector3 MAP_SIZE = { 2,2,2 };					//マップサイズ
	static constexpr VECTOR MAP_LOCALPOS = { 100.0f,100.0f,100.0f };	//マップとの相対座標

	//サイズ
	static constexpr VECTOR MODEL_SIZE = { 200.0f,200.0f,200.0f };		//モデルのサイズ

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

