#include "MultiParty.h"
#include "../../Manager/System/Camera.h"
#include "../../Manager/System/DateBank.h"
#include "../../Manager/System/SceneManager.h"
#include "../../Manager/Game/ScoreManager.h"
#include "../../Manager/Game/PlayerManager.h"
#include "../../Object/Editor/Palette/EditorPaletteBase.h"
#include "../../Object/Editor/EditController.h"
#include "../../Object/Editor/MapDataIO.h"
#include "../../Object/Editor/Palette/MultiPalette.h"
#include "../../Object/System/Result/MultiResult.h"
#include "../../Object/System/RoundDisplay.h"

MultiParty::MultiParty(void)
{
	result_ = nullptr;
	round_ = nullptr;
	phaseChangeTimer_ = 0.0f;
	phaseChanges_.emplace(PHASE::ROUND_PHASE, std::bind(&MultiParty::ChangePhaseRound, this));
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

	//ラウンド
	round_ = std::make_unique<RoundDisplay>();
	round_->Load();

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

	//ラウンドを初期化
	round_->Init();

	//フェーズ遷移
	ChangePhase(PHASE::ROUND_PHASE);

	//カメラ設定
	for (int i = 0; i < DateBank::GetInstance().GetPlayerNum(); i++)
	{
		auto camera = SceneManager::GetInstance().GetCamera(i);
		//各プレイヤーへの設定等をしてください
	}
}

void MultiParty::Reset()
{
	//マップの初期化
	mapIO_->Reset();

	//初期化
	Init();

	//ラウンド別リセット
	RoundReset();

	//スコア初期化
	ScoreManager::GetInstance().Init();
}

void MultiParty::RoundReset()
{
	//セレクトのリセット
	palette_->Init();

	//エディットのリセット
	for (auto& e : editControllers_) { e->Reset(); }

	//リザルトのリセット
	result_->Reset();

	//フェーズを遷移
	ChangePhase(PHASE::ROUND_PHASE);
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
	for (int i = 0; i < DateBank::GetInstance().GetPlayerNum(); i++)
	{
		SetDrawScreen(SceneManager::GetInstance().GetScreen(i));
		SceneManager::GetInstance().GetCamera(i).lock()->CameraSetting(); // カメラの更新
		editControllers_[i]->Update();
		KeyConfig& ins = KeyConfig::GetInstance();
		auto keyType = DateBank::GetInstance().GetPlayerNum() == 1 ? KeyConfig::TYPE::ALL : KeyConfig::TYPE::PAD;
		if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::EDIT_GRID_ON_OFF, static_cast<KeyConfig::JOYPAD_NO>(i + 1), keyType))
		{
			isGrid_[i] = isGrid_[i] ? false : true;
		}
	}
	SetDrawScreen(SceneManager::GetInstance().GetMainScreen());
	bool ready = true;
	for (auto& controller : editControllers_)
	{
		if (!controller->GetReady())
		{
			ready = false;
			break;
		}

	}
	if (ready)
	{
		ChangePhase(PHASE::ACTION_PHASE);
	}
}

void MultiParty::DrawAction(void)
{
	GameScene::DrawAction();
}

void MultiParty::DrawEdit(void)
{
	GameScene::DrawEdit();
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

void MultiParty::ChangePhaseRound()
{
	//処理を登録
	phaseUpdate_ = std::bind(&MultiParty::UpdateRound, this);
	phaseDraw_ = std::bind(&MultiParty::DrawRound, this);
	//画面分割はしない
	scnMng_.SetIsSplitMode(false);
	//ラウンドを追加
	round_->AddNumberIndex(1);
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

	//クリア時間に応じてスコアを格納
	ScoreManager::GetInstance().SetPlayersScore();
}

void MultiParty::UpdateRound()
{
	//ラウンドを更新
	round_->Update();

	//フェーズ遷移用タイマーを更新
	phaseChangeTimer_ += SceneManager::GetInstance().GetDeltaTime();

	//時間になったらフェーズ遷移
	if (phaseChangeTimer_ > ROUND_CHANGE_TIME)
	{
		//初期化
		phaseChangeTimer_ = 0.0f;

		//セレクトへ遷移
		ChangePhase(PHASE::SELECT_PHASE);
	}
}

void MultiParty::UpdateSelect()
{
	palette_->Update();

	//パレット処理が終了したとき
	if (palette_->GetState() == EditorPaletteBase::STATE::NONE)
	{
		ChangePhase(PHASE::EDIT_PHASE);
	}
}

void MultiParty::UpdateResult()
{
	result_->Update(*this);
}

void MultiParty::DrawRound()
{
	round_->Draw();
}

void MultiParty::DrawSelect()
{
	palette_->Draw();
}

void MultiParty::DrawResult()
{
	result_->Draw();
}

void MultiParty::CheckPlayerFinish()
{
	if (PlayerManager::GetInstance().IsPlayersEnd())
	{
		ChangePhase(PHASE::RESULT_PHASE);
	}
}

void MultiParty::DebagUpdate()
{
	//次のフェーズへ状態遷移する
	//if (keyConfig_.IsTrgDown(KeyConfig::CONTROL_TYPE::DECISION_MOUSE,KeyConfig::JOYPAD_NO::PAD1))
	//{
	//	int phase = static_cast<int>(phase_);
	//	int nextPhase = phase + 1;
	//	if (nextPhase == static_cast<int>(PHASE::CLEAR_PHASE)) { nextPhase = 0; }
	//	ChangePhase(static_cast<PHASE>(nextPhase));
	//}
}
