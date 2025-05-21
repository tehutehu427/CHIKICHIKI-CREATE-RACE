#pragma once
#include <map>
#include "PaletteIcon.h"

class SoloPaletteIcon : public PaletteIcon
{
public:

	//コンストラクタ
	SoloPaletteIcon();

	//デストラクタ
	~SoloPaletteIcon() override;

	//初期化
	void Init()override;

private:

	//種類ごとにアイテム残数を管理する
	std::map<ItemBase::ITEM_TYPE, int> itemIconMap_;
	
	//描画
	void DrawItemIcon()override;

	//クリック位置がアイコンか調べる
	void CheckItemIcon(const Vector2 _mPos)override;

	//種類の割り当て
	void AssignType()override;

};

