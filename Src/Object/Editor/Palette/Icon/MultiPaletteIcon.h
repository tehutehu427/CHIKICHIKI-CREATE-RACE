#pragma once
#include "PaletteIcon.h"

class MultiPaletteIcon : public PaletteIcon
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	MultiPaletteIcon();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~MultiPaletteIcon() override;

	//初期化
	void Init()override;

private:

	//種類ごとにアイテム残数を管理する
	std::map<ItemBase::ITEM_TYPE, int> itemIconMap_;

	//クリック位置がアイコンか調べる
	void CheckItemIcon(const Vector2 _mPos)override;

	//種類の割り当て
	void AssignType()override;

	//各アイコンの描画
	virtual void DrawItemIcon();		//アイテムアイコン
};

