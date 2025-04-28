#pragma once
#include "ItemBase.h"

class CannonShot : public ItemBase
{
public:

	//コンストラクタ
	CannonShot(VECTOR _pos, Quaternion _quaRot);
	//デストラクタ
	~CannonShot()override;

	//個人設定
	void SetParam(void)override;
	//更新
	void Update(void)override;
	//描画
	void Draw(void)override;

private:
};

