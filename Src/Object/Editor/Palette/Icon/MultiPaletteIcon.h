#pragma once
#include "PaletteIcon.h"

class PaletteCursor;

class MultiPaletteIcon : public PaletteIcon
{
public:

	//生成数
	static constexpr int CREATE_NUM = 6;

	//描画列
	static constexpr int DRAW_COL = 3;

	//アイコン配置開始座標
	static constexpr int MULTI_ICON_POS_X = 350;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	MultiPaletteIcon();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~MultiPaletteIcon() override;

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init()override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

private:
	
	//カーソル画像
	int* imgCursors_;

	//種類ごとにアイテム残数を管理する
	std::map<ItemBase::ITEM_TYPE, int> itemIconMap_;

	//カーソル
	std::vector<std::unique_ptr<PaletteCursor>> cursors_;

	//除外番号を除いてアイテム配列を生成
	void SetExcludingItemTypeArray()override;

	//指定された範囲から除外番号を除いてランダムで値を返す関数
	int GetRandTypeExcluding();

	//選択時の更新処理
	void UpdateSelect();

	//アイテムアイコンをクリックしたか調べる
	bool CheckItemIcon(const Vector2 _cPos, const int _playerIndex = 0) override;

	//他のプレイヤーが選択していないか調べる
	bool IsChosenByOtherPlayer(const int _iconIndex, const int _playerIndex);

	//アイコンを描画する
	void DrawItemIcon()override;
};

