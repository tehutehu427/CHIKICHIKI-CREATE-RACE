#pragma once
#include "../ItemBase.h"
class BigBomb : public ItemBase
{
public:

	//マップ
	static constexpr IntVector3 MAP_SIZE = { 5,5,5 };					//マップサイズ
	static constexpr VECTOR MAP_LOCALPOS = { 100.0f,100.0f,100.0f };	//マップとの相対座標

	//サイズ
	static constexpr VECTOR MODEL_SIZE = { 200.0f,200.0f,200.0f };		//モデルのサイズ

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

