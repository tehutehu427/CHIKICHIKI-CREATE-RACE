#include "MultiParty.h"
#include "../../Manager/System/Camera.h"
#include "../../Manager/System/DateBank.h"
#include "../../Manager/System/SceneManager.h"
#include "../../Manager/Game/ScoreManager.h"
#include "../../Object/Editor/Palette/EditorPaletteBase.h"
#include "../../Object/Editor/Palette/MultiPalette.h"
#include "../../Object/System/Result/MultiResult.h"

MultiParty::MultiParty(void)
{
	result_ = nullptr;
	phaseChanges_.emplace(PHASE::SELECT_PHASE, std::bind(&MultiParty::ChangePhaseSelect, this));
	phaseChanges_.emplace(PHASE::RESULT_PHASE, std::bind(&MultiParty::ChangePhaseResult, this));
}

MultiParty::~MultiParty(void)
{
	//スコアマネージャーのインスタンスを削除
	ScoreManager::GetInstance().Destroy();
}

void MultiParty::Load(void)
{
	//親クラスの読み込み処理を呼ぶ
	GameScene::Load();

	//パレット生成
	palette_ = std::make_unique<MultiPalette>(editControllers_);
	palette_->Load();

	//リザルト
	result_ = std::make_unique<MultiResult>();
	result_->Load();

	//スコアマネージャーを生成
	ScoreManager::GetInstance().CreateInstance();
}

void MultiParty::Init(void)
{
	//親クラスの初期化処理を呼ぶ
	GameScene::Init();

	//初期化
	palette_->Init();

	//リザルト初期化
	result_->Init();

	//フェーズ遷移
	ChangePhase(PHASE::SELECT_PHASE);

	//カメラ設定
	for (int i = 0; i < DateBank::GetInstance().GetPlayerNum(); i++)
	{
		auto camera = SceneManager::GetInstance().GetCamera(i);
		//各プレイヤーへの設定等をしてください
	}
}

void MultiParty::NormalDraw(void)
{
	//親クラスの描画処理を呼ぶ
	GameScene::NormalDraw();
}

void MultiParty::UpdateAction(void)
{
	//親クラスのアクション時の更新処理を呼ぶ
	GameScene::UpdateAction();
}

void MultiParty::UpdateEdit(void)
{
	if (palette_->GetState() == EditorPaletteBase::STATE::WAIT)
	{
		for (auto& controller : editControllers_) { controller->Update(); }
	}
}

void MultiParty::ChangePhaseEdit()
{
	//親クラスの処理を呼びだし
	GameScene::ChangePhaseEdit();

	//画面を分割する
	scnMng_.SetIsSplitMode(true);
}

void MultiParty::ChangePhaseAction()
{
	//親クラスの処理を呼びだし
	GameScene::ChangePhaseAction();

	//画面を分割する
	scnMng_.SetIsSplitMode(true);
}

void MultiParty::ChangePhaseSelect()
{
	phaseUpdate_ = std::bind(&MultiParty::UpdateSelect, this);
	phaseDraw_ = std::bind(&MultiParty::DrawSelect, this);

	//画面分割はしない
	scnMng_.SetIsSplitMode(false);
}

void MultiParty::ChangePhaseResult()
{
	phaseUpdate_ = std::bind(&MultiParty::UpdateResult, this);
	phaseDraw_ = std::bind(&MultiParty::DrawResult, this);

	//画面分割はしない
	scnMng_.SetIsSplitMode(false);
}

void MultiParty::UpdateSelect()
{
	palette_->Update();

	//パレット処理が終了したとき
	if (palette_->GetState() == EditorPaletteBase::STATE::NONE)
	{
		ChangePhase(PHASE::RESULT_PHASE);
	}
}

void MultiParty::UpdateResult()
{
	result_->Update(*this);
}

void MultiParty::DrawSelect()
{
	palette_->Draw();
}

void MultiParty::DrawResult()
{
	result_->Draw();
}

void MultiParty::DebagUpdate()
{
	//次のフェーズへ状態遷移する
	if (keyConfig_.IsTrgDown(KeyConfig::CONTROL_TYPE::DECISION_MOUSE,KeyConfig::JOYPAD_NO::PAD1))
	{
		int phase = static_cast<int>(phase_);
		int nextPhase = phase + 1;
		if (nextPhase == static_cast<int>(PHASE::CLEAR_PHASE)) { nextPhase = 0; }
		ChangePhase(static_cast<PHASE>(nextPhase));
	}
}
