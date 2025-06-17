#include "MultiResult.h"
#include "../../../Manager/Game/ScoreManager.h"
#include "../../../Manager/System/SceneManager.h"
#include "../../../Scene/Game/MultiParty.h"
#include "../MultiInputCheck.h"
#include "Palette.h"
#include "ScoreGage.h"
#include "ScoreGageManager.h"

MultiResult::MultiResult()
{
	RegisterStateFunction(STATE::READY, [this](MultiParty& _parent) { UpdateStateReady(_parent); });
	RegisterStateFunction(STATE::WAIT, [this](MultiParty& _parent) { UpdateStateWait(_parent); });
	RegisterStateFunction(STATE::SCORE_UPDATE, [this](MultiParty& _parent) { UpdateStateScore(_parent); });
	RegisterStateFunction(STATE::RESULT, [this](MultiParty& _parent) { UpdateStateResult(_parent); });

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
}

void MultiResult::Init()
{
	//パレット
	palette_->Init();
	palette_->ChangeState(Palette::STATE::ADMISSION);	//初期は入場

	//スコアゲージ
	scoreGages_->Init();

	//デバッグ
	ScoreManager::GetInstance().AddScore(0, ScoreManager::SCORE_TYPE::CLEAR);
	ScoreManager::GetInstance().AddScore(0, ScoreManager::SCORE_TYPE::CLEAR);
	ScoreManager::GetInstance().AddScore(0, ScoreManager::SCORE_TYPE::CLEAR);
	ScoreManager::GetInstance().AddScore(0, ScoreManager::SCORE_TYPE::CLEAR);
	ScoreManager::GetInstance().AddScore(0, ScoreManager::SCORE_TYPE::CLEAR);
	ScoreManager::GetInstance().AddScore(1, ScoreManager::SCORE_TYPE::CLEAR);
}

void MultiResult::Update(MultiParty& _parent)
{
	stateMap_[state_](_parent);
}

void MultiResult::Draw()
{
	palette_->Draw();

	scoreGages_->Draw();
}

void MultiResult::RegisterStateFunction(const STATE _state, std::function<void(MultiParty&)> _update)
{
	//処理の登録
	stateMap_[_state] = _update;
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
	}
}

void MultiResult::UpdateStateScore(MultiParty& _parent)
{
	//スコアゲージ更新
	scoreGages_->Update();

	//全員のスコアゲージのアニメーションを終えたら
	if (scoreGages_->IsFinishAnimation())
	{
		int clearLine = 5;

		//状態遷移するか確認
		//勝者がいる場合クリアフェーズへ移る
		if (ScoreManager::GetInstance().GetWinnerPlayerIndex(clearLine) != -1)
		{
			_parent.ChangePhase(MultiParty::PHASE::CLEAR_PHASE);
			return;
		}

		//状態遷移
		ChangeState(STATE::RESULT);

		//入力確認のリセット
		inputCheck_->Reset();
	}
}

void MultiResult::UpdateStateResult(MultiParty& _parent)
{
	inputCheck_->Update();

	//全員がボタンを押して確認ができたら
	if (inputCheck_->IsAllInput())
	{
		//状態遷移
		_parent.ChangePhase(MultiParty::PHASE::SELECT_PHASE);
		return;
	}

}
