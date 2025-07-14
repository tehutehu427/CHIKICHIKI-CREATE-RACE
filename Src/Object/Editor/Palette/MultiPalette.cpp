#include "MultiPalette.h"
#include "../EditController.h"
#include "../Manager/System/ResourceManager.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/System/DateBank.h"
#include "Icon/PaletteIcon.h"
#include "Icon/MultiPaletteIcon.h"
#include "Palette.h"

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
	imgTitle_ = res.Load(ResourceManager::SRC::SELECT_ITEM).handleId_;

	//パレットアイコン
	palIcon_ = std::make_unique<MultiPaletteIcon>();
	palIcon_->Load();

	//パレット
	pal_ = std::make_unique<Palette>();
	pal_->Load();

	//音源の読み込み
	LoadSounds();
}

void MultiPalette::Init(void)
{
	//パレット
	pal_->Init();

	//アイコン
	palIcon_->Init();

	//初期状態
	ChangeState(STATE::PALETTE_ANIM);

	//パレットの初期状態
	pal_->ChangeState(Palette::STATE::EXPANSION);
}

void MultiPalette::Draw(void)
{
	//元の描画
	EditorPaletteBase::Draw();

	//見出しの描画
	if (state_ == STATE::SELECT)
	{
		constexpr int POS_Y = 50; //見出しのY座標
		constexpr float RATE = 0.6f; //見出しの拡大率

		DrawRotaGraph(
			Application::SCREEN_HALF_X,
			POS_Y,
			RATE,
			0.0f,
			imgTitle_,
			true
		);
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
		editControllers_[i]->SetItemType(palIcon_->GetSelectType(i));
	}

	//状態変更
	ChangeState(STATE::PALETTE_ANIM);

	//パレットの状態変更
	pal_->ChangeState(Palette::STATE::REDUCTION);

	//アイコンの状態も変更
	palIcon_->ChangeState(PaletteIcon::STATE::NONE);
}
