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

	//パレットアイコン
	palIcon_ = std::make_unique<MultiPaletteIcon>();
	palIcon_->Load();

	//パレット
	pal_ = std::make_unique<Palette>();
	pal_->Load();
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
