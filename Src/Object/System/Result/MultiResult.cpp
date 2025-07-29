#include "MultiResult.h"
#include "../../../Manager/Game/ScoreManager.h"
#include "../../../Manager/System/SceneManager.h"
#include "../../../Manager/System/SoundManager.h"
#include "../../../Manager/System/DateBank.h"
#include "../../../Scene/Game/MultiParty.h"
#include "../../../Utility/Utility.h"
#include "../../Editor/Palette/Palette.h"
#include "../MultiInputCheck.h"
#include "ScoreGage.h"
#include "ScoreGageManager.h"

MultiResult::MultiResult()
{
	RegisterStateUpdateFunction(STATE::READY, [this](MultiParty& _parent) { UpdateStateReady(_parent); });
	RegisterStateUpdateFunction(STATE::WAIT, [this](MultiParty& _parent) { UpdateStateWait(_parent); });
	RegisterStateUpdateFunction(STATE::SCORE_UPDATE, [this](MultiParty& _parent) { UpdateStateScore(_parent); });
	RegisterStateUpdateFunction(STATE::RESULT, [this](MultiParty& _parent) { UpdateStateResult(_parent); });
	RegisterStateUpdateFunction(STATE::INPUT_CHECK, [this](MultiParty& _parent) { UpdateStateInputCheck(_parent); });

	RegisterStateDrawFunction(STATE::READY, [this]() { DrawStateReady(); });
	RegisterStateDrawFunction(STATE::WAIT, [this]() { DrawStateWait(); });
	RegisterStateDrawFunction(STATE::SCORE_UPDATE, [this]() { DrawStateScore(); });
	RegisterStateDrawFunction(STATE::RESULT, [this]() { DrawStateResult(); });
	RegisterStateDrawFunction(STATE::INPUT_CHECK, [this]() { DrawStateInputCheck(); });

	state_ = STATE::READY;
	palette_ = nullptr;
	scoreGages_ = nullptr;
	inputCheck_ = nullptr;
	waitStep_ = 0.0f;
}

MultiResult::~MultiResult()
{
}

void MultiResult::Load()
{
	//読み込み
	palette_ = std::make_unique<Palette>();
	palette_->Load();

	//スコアゲージ
	scoreGages_ = std::make_unique<ScoreGageManager>();
	scoreGages_->Load();

	//入力チェック
	inputCheck_ = std::make_unique<MultiInputCheck>();
	inputCheck_->Load();

	SoundManager& sndMng = SoundManager::GetInstance();
	sndMng.LoadResource(SoundManager::SRC::DRUM_ROLL);
	sndMng.LoadResource(SoundManager::SRC::DRUM_ROLL_END);
}

void MultiResult::Init()
{
	//パレット
	palette_->Init();
	palette_->ChangeState(Palette::STATE::ADMISSION);	//初期は入場

	//スコアゲージ
	scoreGages_->Init();

	//入力確認
	inputCheck_->Init();
}

void MultiResult::Update(MultiParty& _parent)
{
	stateUpdateMap_[state_](_parent);
}

void MultiResult::Draw()
{
	//パレット
	palette_->Draw();

	//状態別描画処理の呼び出し
	stateDrawMap_[state_]();
}

void MultiResult::Reset()
{
	//パレット初期状態
	palette_->ChangeState(Palette::STATE::ADMISSION);

	//入力確認リセット
	inputCheck_->Reset();

	//ゲージの初期化
	scoreGages_->Init();

	//初期状態の変更
	ChangeState(STATE::READY);
}

void MultiResult::RegisterStateUpdateFunction(const STATE _state, std::function<void(MultiParty&)> _update)
{
	//処理の登録
	stateUpdateMap_[_state] = _update;
}

void MultiResult::RegisterStateDrawFunction(const STATE _state, std::function<void()> _draw)
{
	stateDrawMap_[_state] = _draw;
}

void MultiResult::UpdateStateReady(MultiParty& _parent)
{
	//パレット更新
	palette_->Update();
	
	//パレットの状態がCENTERになったら
	if (palette_->GetState() == Palette::STATE::CENTER)
	{
		//状態遷移
		ChangeState(STATE::WAIT);

		//待機時間設定
		waitStep_ = WAIT_TIME;

		//スコアゲージを待機状態へ
		scoreGages_->ChangeAllState(ScoreGage::STATE::WAIT);
	}
}

void MultiResult::UpdateStateWait(MultiParty& _parent)
{
	//ステップ更新
	waitStep_-= SceneManager::GetInstance().GetDeltaTime();
	
	//スコアゲージ更新
	scoreGages_->Update();

	//待機時間が経過したら
	if (waitStep_ <= 0.0f)
	{
		//状態遷移
		ChangeState(STATE::SCORE_UPDATE);

		//スコアゲージの状態を更新
		scoreGages_->ChangeAllState(ScoreGage::STATE::ANIMATION);

		//ドラムロールの再生
		SoundManager::GetInstance().Play(SoundManager::SRC::DRUM_ROLL, SoundManager::PLAYTYPE::BACK);
	}
}

void MultiResult::UpdateStateScore(MultiParty& _parent)
{
	//スコアゲージ更新
	scoreGages_->Update();

	//全員のスコアゲージのアニメーションを終えたら
	if (scoreGages_->IsFinishAnimation())
	{
		SoundManager& sndMng = SoundManager::GetInstance();

		//ドラムロールの停止
		sndMng.Stop(SoundManager::SRC::DRUM_ROLL);

		//ドラムロールの終了音を再生
		sndMng.Play(SoundManager::SRC::DRUM_ROLL_END, SoundManager::PLAYTYPE::BACK);

		//待機時間設定
		waitStep_ = AFTER_WAIT_TIME;

		//状態遷移
		ChangeState(STATE::RESULT);
	}
}

void MultiResult::UpdateStateResult(MultiParty& _parent)
{
	//ステップ更新
	waitStep_ -= SceneManager::GetInstance().GetDeltaTime();

	//スコアゲージ更新
	scoreGages_->Update();

	if (waitStep_ <= 0)
	{		
		//状態遷移するか確認
		//勝者がいる場合クリアフェーズへ移る
		if (ScoreManager::GetInstance().GetWinnerPlayerIndex() != -1)
		{
			_parent.ChangePhase(MultiParty::PHASE::CLEAR_PHASE);
			return;
		}

		//パッド入力状況のリセット
		inputCheck_->ResetInput();

		//状態遷移
		ChangeState(STATE::INPUT_CHECK);
	}
}

void MultiResult::UpdateStateInputCheck(MultiParty& _parent)
{	
	inputCheck_->Update();

	//スコアゲージ更新
	scoreGages_->Update();

	//全員がボタンを押して確認ができたら
	if (inputCheck_->IsAllInput())
	{
		//状態遷移
		_parent.RoundReset();
		return;
	}
}

void MultiResult::DrawStateReady()
{
}

void MultiResult::DrawStateWait()
{
	//スコアゲージ
	scoreGages_->Draw();

	//タイトルの描画
	scoreGages_->DrawTitle();
}

void MultiResult::DrawStateScore()
{
	//スコアゲージ
	scoreGages_->Draw();

	//タイトルの描画
	scoreGages_->DrawTitle();
}

void MultiResult::DrawStateResult()
{
	//スコアゲージ
	scoreGages_->Draw();

	//勝者の描画
	scoreGages_->DrawIsWinning();
}

void MultiResult::DrawStateInputCheck()
{
	//スコアゲージ
	scoreGages_->Draw();

	//勝者の描画
	scoreGages_->DrawIsWinning();

	//ボタンを押してねの描画
	scoreGages_->DrawPushButton();

	//入力確認
	inputCheck_->Draw();
}

