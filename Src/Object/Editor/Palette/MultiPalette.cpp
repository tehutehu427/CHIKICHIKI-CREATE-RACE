#include "MultiPalette.h"
#include "../EditController.h"
#include "../Manager/System/ResourceManager.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/System/DateBank.h"
#include "Icon/PaletteIcon.h"
#include "Icon/MultiPaletteIcon.h"

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

	//パレットアイコン
	palIcon_ = std::make_unique<MultiPaletteIcon>();
	palIcon_->Load();
}

void MultiPalette::Init(void)
{
	//初期化
	pal_.pos = { Application::SCREEN_HALF_X,Application::SCREEN_HALF_Y };
	pal_.rate = 0.0f;
	pal_.size = {
		static_cast<int>(static_cast<float>(PALETTE_SIZE_X) * pal_.rate),
		static_cast<int>(static_cast<float>(PALETTE_SIZE_Y) * pal_.rate) };

	palIcon_->Init();

	ChangeState(STATE::OPEN);
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

	if (pal_.rate >= PALETTE_RATE)
	{
		pal_.rate = PALETTE_RATE;
		ChangeState(STATE::SELECT);
	}
}

void MultiPalette::UpdateSelect()
{
	//パレットアイコンに関する処理
	palIcon_->Update();

	//生成開始をしないとき
	if (!palIcon_->IsCreate())
	{
		//処理終了
		return;
	}

	//人数分コントローラに設定
	for (int i = 0; i < editControllers_.size(); i++)
	{
		editControllers_[i]->SetItemType(palIcon_->GetSelectType());
	}

	//状態変更
	ChangeState(STATE::CLOSE);

	//アイコンの状態も変更
	palIcon_->ChangeState(PaletteIcon::STATE::NONE);
}
