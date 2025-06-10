#pragma once
#include "EditorPaletteBase.h"

class PaletteCursor;

class MultiPalette : public EditorPaletteBase
{
public:

	static constexpr float RATE_SPEED = 0.05f;

	// コンストラクタ
	MultiPalette(std::vector<std::unique_ptr<EditController>>& editControllers);

	// デストラクタ
	~MultiPalette(void) override;

	//読み込み処理
	void Load(void) override;

	//初期化処理
	void Init(void) override;

	//描画処理
	void Draw(void) override;

private:
	
	//カーソル画像
	int* imgCursors_;

	//カーソル
	std::vector<std::unique_ptr<PaletteCursor>> cursors_;

	//状態別更新
	void UpdateWait()override;			//クリック待ち
	void UpdateClose()override;			//閉じる
	void UpdateOpen()override;			//開ける
	void UpdateSelect()override;		//選ぶ
};

