#include "FreePlay.h"
#include "../../Object/Editor/MapDataIO.h"
#include "../../Object/Editor/EditController.h"
#include "../../Object/Editor/Palette/EditorPaletteBase.h"
#include "../../Object/System/CheckChangePhase.h"
#include "../../Object/System/ManualTab.h"
#include "../../Object/Editor/EditEscape.h"

FreePlay::FreePlay(void)
{
	checkChangePhase_ = nullptr;
	manual_ = nullptr;
	editEscape_ = nullptr;
}

FreePlay::~FreePlay(void)
{
}

void FreePlay::Load(void)
{
	//親クラスの読み込み
	GameScene::Load();

	//パレットの生成
	palette_ = std::make_unique<EditorPaletteBase>(editControllers_);
	palette_->Load();

	//フェーズ遷移
	checkChangePhase_ = std::make_unique<CheckChangePhase>(editControllers_[0]->GetCursorPos());
	checkChangePhase_->Load();

	//マニュアル
	manual_ = std::make_unique<ManualTab>(editControllers_[0]->GetCursorPos());
	manual_->Load();

	//編集終了
	editEscape_ = std::make_unique<EditEscape>(editControllers_[0]->GetCursorPos());
	editEscape_->Load();
}

void FreePlay::Init(void)
{
	//親クラスの初期化
	GameScene::Init();

	//初期化
	palette_->Init();

	//初期化
	checkChangePhase_->Init();

	//マップデータの初期化
	mapIO_->Init();

	//マニュアルの初期化
	manual_->Init();

	//編集
	editEscape_->Init();

	ChangePhase(PHASE::EDIT_PHASE);
}

void FreePlay::UpdateAction(void)
{
	//親クラスの更新
	GameScene::UpdateAction();
}

void FreePlay::UpdateEdit(void)
{	
	//マニュアル
	if (palette_->GetState() == EditorPaletteBase::STATE::WAIT || mapIO_->IsEdit() && editEscape_->IsEdit()) { manual_->Update(); }
	if (manual_->IsDisplay()) { return; }	//表示中は処理を止める	
	
	//編集終了
	if (palette_->GetState() == EditorPaletteBase::STATE::WAIT && mapIO_->IsEdit()) { editEscape_->Update(); }

	//マップデータの更新
	if (palette_->GetState() == EditorPaletteBase::STATE::WAIT && editEscape_->IsEdit()) { mapIO_->Update(); }

	//パレット
	if (mapIO_->IsEdit() && editEscape_->IsEdit()) { palette_->Update(); }


	for (int i = 0; i < DateBank::GetInstance().GetPlayerNum(); i++)
	{
		KeyConfig& ins = KeyConfig::GetInstance();
		auto keyType = DateBank::GetInstance().GetPlayerNum() == 1 ? KeyConfig::TYPE::ALL : KeyConfig::TYPE::PAD;
		if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::EDIT_GRID_ON_OFF, static_cast<KeyConfig::JOYPAD_NO>(i + 1), keyType))
		{
			isGrid_[i] = isGrid_[i] ? false : true;
		}
	}

	//コントローラー
	if (palette_->GetState() == EditorPaletteBase::STATE::WAIT &&
		mapIO_->IsEdit() &&
		editEscape_->IsEdit())
	{
		for (auto& controller : editControllers_) { controller->Update(); }
	}
	else
	{
		for (auto& controller : editControllers_) { controller->CursorUpdate(); }
	}
}

void FreePlay::ChangePhaseAction(void)
{
	//親クラスの処理を実行
	GameScene::ChangePhaseAction();

	//次のフェーズ遷移の設定
	checkChangePhase_->SetNextPhase(PHASE::EDIT_PHASE);
}

void FreePlay::ChangePhaseEdit(void)
{
	//親クラスの処理を実行
	GameScene::ChangePhaseEdit();

	//次のフェーズ遷移の設定
	checkChangePhase_->SetNextPhase(PHASE::ACTION_PHASE);
}

void FreePlay::NormalUpdate()
{	
	//親クラスの処理を呼び出し
	GameScene::NormalUpdate();

	//フェーズ遷移
	checkChangePhase_->Update(*this);
}

void FreePlay::NormalDraw(void)
{
	//親クラスの描画
	GameScene::NormalDraw();

	//フェーズ遷移アイコン
	checkChangePhase_->Draw();

	if (phase_ == PHASE::EDIT_PHASE)
	{
		//矢印等の描画
		auto screenIndex = SceneManager::GetInstance().GetScreenIndex();
		editControllers_[screenIndex]->DrawUI();
	}
}

void FreePlay::DrawEdit()
{
	//親クラスの描画
	GameScene::DrawEdit();

	//マップデータの描画
	mapIO_->Draw();

	//マニュアルの描画
	manual_->Draw();

	//編集終了
	editEscape_->Draw();

}
