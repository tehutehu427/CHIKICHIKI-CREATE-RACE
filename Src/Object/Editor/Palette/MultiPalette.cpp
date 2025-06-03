#include "MultiPalette.h"
#include "../EditController.h"
#include "../Manager/System/ResourceManager.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/System/DateBank.h"
#include "Icon/PaletteIcon.h"
#include "Icon/FreePaletteIcon.h"
#include "Icon/SoloPaletteIcon.h"
#include "Icon/MultiPaletteIcon.h"
#include "PaletteCursor.h"

MultiPalette::MultiPalette(std::vector<std::unique_ptr<EditController>>& _editControllers)
	: EditorPaletteBase(_editControllers)
{
}

MultiPalette::~MultiPalette(void)
{
}

void MultiPalette::Load(void)
{
	//リソースの読み込み
	ResourceManager& res = ResourceManager::GetInstance();
	imgPalette_ = res.Load(ResourceManager::SRC::PALETTE).handleId_;
	imgCursors_ = res.Load(ResourceManager::SRC::CURSORS).handleIds_;

	//カーソル
	for (int i = 0; i < DateBank::GetInstance().GetPlayerNum(); i++)
	{
		cursors_.push_back(std::make_unique<PaletteCursor>(i, imgCursors_[i]));
	}

	//パレットアイコン
	palIcon_ = std::make_unique<MultiPaletteIcon>();
	palIcon_->Load();
}

void MultiPalette::Init(void)
{
	for (auto& cursor : cursors_)
	{
		cursor->Init();
	}

	ChangeState(STATE::NONE);
}

void MultiPalette::Draw(void)
{
	//描画処理
	DrawRotaGraph(
		pal_.pos.x,
		pal_.pos.y,
		pal_.rate,
		pal_.angle,
		imgPalette_,
		true,
		false);

	//選択描画
	palIcon_->Draw();

	//カーソル描画
	for (auto& cursor : cursors_)
	{
		cursor->Draw();
	}
}

void MultiPalette::UpdateWait()
{
	//特に処理なし
}

void MultiPalette::UpdateClose()
{
	pal_.rate -= RATE_SPEED;
	
	if (pal_.rate <= 0.0f)
	{
		pal_.rate = 0.0f;
		ChangeState(STATE::NONE);
	}
}

void MultiPalette::UpdateOpen()
{
	pal_.rate += RATE_SPEED;

	if (pal_.rate >= 1.0f)
	{
		pal_.rate = 1.0f;
		ChangeState(STATE::SELECT);
	}
}

void MultiPalette::UpdateSelect()
{
	//各プレイヤーごとにカーソルを用意
	//アイテムを選ばせる処理

	for (auto& cursor : cursors_)
	{
		cursor->Update();
	}
}
