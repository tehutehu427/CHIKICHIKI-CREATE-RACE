#pragma once
#include "ItemBase.h"

class Cannon : public ItemBase
{
public:

	//マップサイズ
	static constexpr IntVector3 MAP_SIZE = { 2,2,2 };

	//狙う範囲半径
	static constexpr float  AIM_RADIUS = 200.0f;

	//コンストラクタ
	Cannon();
	//デストラクタ
	~Cannon()override;

	//個人設定
	void SetParam(void)override;
	//更新
	void Update(void)override;
	//描画
	void Draw(void)override;
	//解放
	void Release(void)override;

private:

	//砲身用モデル情報
	Transform barrelTrans_;
};

