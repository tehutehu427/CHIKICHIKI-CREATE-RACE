#pragma once
#include "ItemBase.h"

class Fence : public ItemBase
{
public:

	//マップサイズ
	static constexpr IntVector3 MAP_SIZE = { 2,1,1 };

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
	//解放
	void Release(void)override;

private:

};

