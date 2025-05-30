#pragma once
#include "UiBase.h"

class EditorUi: public UiBase
{
public:

	//操作説明フォント
	static constexpr int EXPLAN_FONT_SIZE = 16;
	static constexpr int EXPLAN_FONT_THICK = 3;

	//入出力フォント
	static constexpr int IO_FONT_SIZE = 32;
	static constexpr int IO_FONT_THICK = 5;

	

	/// <summary>
	/// コンストラクタ
	/// </summary>
	EditorUi();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EditorUi() override;

	/// <summary>
	/// 読み込み
	/// </summary>
	void Load()override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

private:

	//操作説明用フォント
	int explanFont_;
	int ioFont_;

	//操作説明
	void DrawEditingExplan();

	//データ入出力のアイコン
	void DrawMapDataIOIcon();

	//プレイへの切り替え
	void DrawChangePlay();
};

