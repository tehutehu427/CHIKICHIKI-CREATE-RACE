#pragma once
#include "PaletteIcon.h"

class PaletteCursor;

class MultiPaletteIcon : public PaletteIcon
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	MultiPaletteIcon(void);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~MultiPaletteIcon(void) override;

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load(void) override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(void)override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(void) override;

private:

	//生成数
	static constexpr int CREATE_NUM = 6;

	//描画列
	static constexpr int DRAW_COL = 3;

	//アイコン配置開始座標
	static constexpr int MULTI_ICON_POS_X = 350;

	//アイテム出現率の重み
	static constexpr int ITEM_WEIGHT = 3;
	
	//カーソル画像
	int* imgCursors_;

	std::vector<bool> isSkips_;

	//種類ごとにアイテム残数を管理する
	std::map<ItemBase::ITEM_TYPE, int> itemIconMap_;

	//カーソル
	std::vector<std::unique_ptr<PaletteCursor>> cursors_;

	//除外番号を除いてアイテム配列を生成
	void SetExcludingItemTypeArray(void)override;

	//指定された範囲から除外番号を除いてランダムで値を返す関数
	int GetRandTypeExcluding(void);

	//選択時の更新処理
	void UpdateSelect(void);

	//アイテムアイコンをクリックしたか調べる
	bool CheckItemIcon(const Vector2 _cPos, const int _playerIndex = 0) override;

	//他のプレイヤーが選択していないか調べる
	bool IsChosenByOtherPlayer(const int _iconIndex, const int _playerIndex);

	//アイコンを描画する
	void DrawItemIcon(void)override;
};