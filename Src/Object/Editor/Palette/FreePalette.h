#pragma once
#include "EditorPaletteBase.h"

class FreePalette : public EditorPaletteBase
{
public:

	// コンストラクタ
	FreePalette(EditController& _controller);

	// デストラクタ
	~FreePalette(void) override;

	//読み込み処理
	void Load(void) override;

	//初期化処理
	void Init(void) override;

	//更新処理
	void Update(void) override;

	//描画処理
	void Draw(void) override;
};

