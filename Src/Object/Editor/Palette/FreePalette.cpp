#include "FreePalette.h"

FreePalette::FreePalette(EditController& _controller)
	: EditorPaletteBase(_controller)
{
}

FreePalette::~FreePalette(void)
{
}

void FreePalette::Load(void)
{
	//親クラスの読み込み処理を呼ぶ
	EditorPaletteBase::Load();
}

void FreePalette::Init(void)
{
	//親クラスの初期化処理を呼ぶ
	EditorPaletteBase::Init();
}

void FreePalette::Update(void)
{
	//親クラスの更新処理を呼ぶ
	EditorPaletteBase::Update();
}

void FreePalette::Draw(void)
{
	//親クラスの描画処理を呼ぶ
	EditorPaletteBase::Draw();
}
