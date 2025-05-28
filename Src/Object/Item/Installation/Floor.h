#pragma once
#include "../ItemBase.h"
class Floor : public ItemBase
{
public:

	//マップ
	static constexpr IntVector3 MAP_SIZE = { 4,1,4 };				//マップサイズ
	static constexpr VECTOR MAP_LOCALPOS = { 220.0f,15.0f,220.0f };	//マップとの相対座標

	//サイズ
	static constexpr VECTOR MODEL_SIZE = { 440.0f,30.0f,440.0f };	//モデルのサイズ

	//コンストラクタ
	Floor();
	//デストラクタ
	~Floor()override;

	//個人設定
	void SetParam(void)override;
	//更新
	void Update(void)override;

private:
};

