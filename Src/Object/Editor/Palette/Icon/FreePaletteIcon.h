#pragma once
#include "PaletteIcon.h"

class FreePaletteIcon : public PaletteIcon
{
public:

	//コンストラクタ
	FreePaletteIcon();

	//デストラクタ
	~FreePaletteIcon();

	//初期化
	void Init()override;

private:

	//クリック位置がアイコンか調べる
	void CheckItemIcon(const Vector2 _mPos)override;

	//種類の割り当て
	void AssignType()override;

};

