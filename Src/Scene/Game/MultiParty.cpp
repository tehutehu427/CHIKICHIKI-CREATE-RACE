#include "MultiParty.h"
#include "../../Manager/System/Camera.h"
#include "../../Manager/System/DateBank.h"
#include "../../Manager/System/SceneManager.h"
#include "../../Manager/Game/ScoreManager.h"
#include "../../Manager/Game/PlayerManager.h"
#include "../../Manager/Game/MapEditer.h"
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
	editBgmSrc_ = SoundManager::SRC::NONE;
	playBgmSrc_ = SoundManager::SRC::NONE;
	phaseChangeTimer_ = 0.0f;
	actionChangeTime_ = 0.0f;
	editChengeTime_ = 0.0f;
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

	//ランダムBGMを取得
	RandomBgm();

	//BGMボリュームを設定
	sndMng_.SetLoadedSoundsVolume();
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
	sndMng_.Stop(SoundManager::SRC::MULTI_CLEAR_BGM);

	MapEditer::GetInstance().DeleteAllItem();

	//マップの初期化
	mapIO_->Reset();

	//初期化
	Init();

	//BGMをランダム設定
	RandomBgm();

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

	//BGMを停止
	sndMng_.Stop(playBgmSrc_);

	//BGMをランダム設定
	RandomBgm();
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
		editChengeTime_ -= SceneManager::GetInstance().GetDeltaTime();
		if (editChengeTime_ < 0)
		{
			ChangePhase(PHASE::ACTION_PHASE);
		}
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

void MultiParty::ChangePhaseClear()
{
	GameScene::ChangePhaseClear();	

	//BGMの停止
	sndMng_.Stop(playBgmSrc_);

	//クリアジングル再生
	sndMng_.Play(SoundManager::SRC::MULTI_CLEAR_JINGLE, SoundManager::PLAYTYPE::BACK);

	auto camera = SceneManager::GetInstance().GetCamera(0).lock();
	camera->ChangeMode(Camera::MODE::FOLLOW_ROTATION);
	camera->SetFollow(&PlayerManager::GetInstance().GetPlayerTransform(ScoreManager::GetInstance().GetWinnerPlayerIndex()));
}
void MultiParty::ChangePhaseEdit()
{
	//親クラスの処理を呼びだし
	GameScene::ChangePhaseEdit();

	editChengeTime_ = EDIT_CHANGE_TIME;

	//画面を分割する
	scnMng_.SetIsSplitMode(true);
}

void MultiParty::ChangePhaseAction()
{
	//親クラスの処理を呼びだし
	GameScene::ChangePhaseAction();

	//画面を分割する
	scnMng_.SetIsSplitMode(true);

	//アクションフェーズの時間を設定
	actionChangeTime_ = ACTION_CHANGE_TIME;

	//BGMを停止
	sndMng_.Stop(editBgmSrc_);

	//BGMを再生
	sndMng_.Play(playBgmSrc_, SoundManager::PLAYTYPE::LOOP);
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
	//BGMを再生
	sndMng_.Play(SoundManager::SRC::ROUND_JINGLE, SoundManager::PLAYTYPE::BACK);
}

void MultiParty::ChangePhaseSelect()
{
	phaseUpdate_ = std::bind(&MultiParty::UpdateSelect, this);
	phaseDraw_ = std::bind(&MultiParty::DrawSelect, this);
	//画面分割はしない
	scnMng_.SetIsSplitMode(false);
	//BGMを再生
	sndMng_.Stop(SoundManager::SRC::ROUND_JINGLE);
	sndMng_.Play(editBgmSrc_, SoundManager::PLAYTYPE::LOOP);
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

void MultiParty::UpdateClear()
{
	GameScene::UpdateClear();

	if (!sndMng_.IsPlay(SoundManager::SRC::MULTI_CLEAR_JINGLE) && !sndMng_.IsPlay(SoundManager::SRC::MULTI_CLEAR_BGM))
	{
		sndMng_.Play(SoundManager::SRC::MULTI_CLEAR_BGM, SoundManager::PLAYTYPE::LOOP); //クリアBGMを再生
	}
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
	//プレイヤーの処理が終わったか確認
	if (PlayerManager::GetInstance().IsPlayersEnd())
	{			
		//アクションフェーズの時間を更新
		actionChangeTime_ -= SceneManager::GetInstance().GetDeltaTime();	

		//アクションフェーズの時間が経過したら
		if (actionChangeTime_ <= 0.0f)
		{
			//リザルトへ遷移	
			ChangePhase(PHASE::RESULT_PHASE);
			return;
		}
	}
}

void MultiParty::LoadSound()
{
	GameScene::LoadSound();
	sndMng_.LoadResource(SoundManager::SRC::PLAY_BGM_1);
	sndMng_.LoadResource(SoundManager::SRC::PLAY_BGM_2);
	sndMng_.LoadResource(SoundManager::SRC::MULTI_BGM_1);
	sndMng_.LoadResource(SoundManager::SRC::MULTI_BGM_2);
	sndMng_.LoadResource(SoundManager::SRC::ROUND_JINGLE);
	sndMng_.LoadResource(SoundManager::SRC::MULTI_CLEAR_BGM);
	sndMng_.LoadResource(SoundManager::SRC::MULTI_CLEAR_JINGLE);
}

void MultiParty::RandomBgm()
{
	constexpr int EDIT_BGM_NUM = 2;	//エディットBGMの数
	constexpr int PLAY_BGM_NUM = 2;	//プレイBGMの数

	int index = GetRand(EDIT_BGM_NUM);
	switch (index)
	{
	case 0:
		editBgmSrc_ = SoundManager::SRC::MULTI_BGM_1;
		break;
	case 1:
		editBgmSrc_ = SoundManager::SRC::MULTI_BGM_2;
		break;
	default:
		editBgmSrc_ = SoundManager::SRC::MULTI_BGM_1;
		break;
	}

	index = GetRand(PLAY_BGM_NUM);
	switch (index)
	{
	case 0:
		playBgmSrc_ = SoundManager::SRC::PLAY_BGM_1;
		break;
	case 1:
		playBgmSrc_ = SoundManager::SRC::PLAY_BGM_2;
		break;
	default:
		playBgmSrc_ = SoundManager::SRC::PLAY_BGM_1;
		break;
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
