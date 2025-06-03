#pragma once
#include "../ItemBase.h"

class Spring : public ItemBase
{
public:

	//マップ
	static constexpr IntVector3 MAP_SIZE = { 1,1,1 };				//マップサイズ
	static constexpr VECTOR MAP_LOCALPOS = { 16.0f,9.0f,16.0f };	//マップとの相対座標

	//サイズ
	static constexpr VECTOR MODEL_SIZE = { 32.0f,18.0f,32.0f };		//モデルのサイズ

	//コンストラクタ
	Spring();
	//デストラクタ
	~Spring()override;

	//個人設定
	void SetParam(void)override;
	//更新
	void Update(void)override;

};

