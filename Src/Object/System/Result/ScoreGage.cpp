#include "ScoreGage.h"
#include <DxLib.h>
#include "../Utility/Utility.h"
#include "../Manager/Game/ScoreManager.h"
#include "../Manager/System/ResourceManager.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/System/DateBank.h"
#include "BonusUis.h"

ScoreGage::ScoreGage(const int _playerIndex) : 
	playerIndex_(_playerIndex)
{
	imgGages_ = nullptr;
	bonus_ = nullptr;
	imgGageOutline_ = -1;
	pos_ = {};
	size_ = {};
	imgScoreGage_ = 0;
	animStep_ = 0.0f;
	lengthPerPoint_ = 0.0f;
	updateLength_  = 0.0f; 
	state_ = STATE::NONE;
	stateChanges_.emplace(STATE::NONE, std::bind(&ScoreGage::ChangeStateNone, this));
	stateChanges_.emplace(STATE::WAIT, std::bind(&ScoreGage::ChangeStateWait, this));
	stateChanges_.emplace(STATE::ANIMATION, std::bind(&ScoreGage::ChangeStateAnimation, this));
	stateChanges_.emplace(STATE::AFTER_WAIT, std::bind(&ScoreGage::ChangeStateAfterWait, this));
}

ScoreGage::~ScoreGage(void)
{
}

void ScoreGage::Load(void)
{
	//リソースの読み込み
	ResourceManager& res = ResourceManager::GetInstance();
	imgGageOutline_ = res.Load(ResourceManager::SRC::PLAYER_GAGE_OUTLINE).handleId_;
	imgGages_ = res.Load(ResourceManager::SRC::PLAYER_GAGES).handleIds_;

	bonus_ = std::make_unique<BonusUis>(playerIndex_);
	bonus_->Load();


	//パラメーター設定
	SetParamToPlayerNo();
}

void ScoreGage::Init(void)
{
	//初期状態
	ChangeState(STATE::NONE);

	//プレイヤー番号ごとに設定
	SetParamToPlayerNo();	

	//アニメーションステップ初期化
	animStep_ = 0.0f;

	//ゲージサイズ
	size_ = { GAGE_SIZE_X,GAGE_SIZE_Y };

	scoreMax_ = DateBank::GetInstance().GetMultiClearScore();

	//1スコア当たりのゲージ長さ
	lengthPerPoint_ = GAGE_LENGTH_MAX / scoreMax_;

	bonus_->Init();
}

void ScoreGage::Update(void)
{
	stateUpdate_();
}

void ScoreGage::Draw(void)
{
	//一定の状態の場合描画させない
	if (state_ == STATE::NONE) { return; }

	constexpr int OUTLINE = 5;

	//輪郭線
	DrawBox(
		pos_.x - OUTLINE,
		pos_.y - OUTLINE,
		pos_.x + size_.x + OUTLINE,
		pos_.y + size_.y + OUTLINE,
		Utility::BLACK,
		true
	);

	//スコアゲージの描画
	DrawRectGraph(
		pos_.x,
		pos_.y,
		0,0,
		size_.x,
		size_.y,
		imgGages_[playerIndex_],
		true
	);

	bonus_->Draw();
}

void ScoreGage::ChangeState(const STATE _state)
{
	//状態変更
	state_ = _state;

	//状態別変更処理
	stateChanges_[state_]();	
}

void ScoreGage::SetLengthPerPoint(const int _lengthPerPoint)
{
	lengthPerPoint_ = _lengthPerPoint;
}

void ScoreGage::ChangeStateNone(void)
{
	stateUpdate_ = std::bind(&ScoreGage::UpdateStateNone, this);
}

void ScoreGage::ChangeStateWait(void)
{
	stateUpdate_ = std::bind(&ScoreGage::UpdateStateNone, this);
}

void ScoreGage::ChangeStateAnimation(void)
{
	ScoreManager& scoreMng = ScoreManager::GetInstance();

	//更新処理
	stateUpdate_ = std::bind(&ScoreGage::UpdateStateAnimation, this);

	//現在の勝者のスコアを取得
	const int nowScore = scoreMng.GetScore(scoreMng.GetNowWinnerPlayerIndex());

	//スコア最大値を更新
	scoreMax_ = scoreMax_ > nowScore ? scoreMax_ : nowScore;

	//1点に対するスコアの増幅値を計算
	lengthPerPoint_ = GAGE_LENGTH_MAX / scoreMax_;

	//長さの更新値を決定
	updateLength_ = size_.x + ScoreManager::GetInstance().GetScore(playerIndex_) * lengthPerPoint_;


	bonus_->SetBonus();
}

void ScoreGage::ChangeStateAfterWait(void)
{
	stateUpdate_ = std::bind(&ScoreGage::UpdateStateAfterWait, this);
}

void ScoreGage::UpdateStateAnimation(void)
{
	animStep_ += SceneManager::GetInstance().GetDeltaTime();

	//必要な情報を設定
	constexpr int OFFSET = 2;	//目標位置に到達したときのオフセット
	float start = static_cast<float>(size_.x);	//開始位置
	float goal = static_cast<float>(updateLength_);	//目標位置

	//移動量を求める
	float move = Utility::EaseInQuad(animStep_, ANIM_TIME, start, goal);

	//座標更新
	size_.x = static_cast<int>(move);

	bonus_->Update();

	//目標位置に到達したら状態を変更
	if (size_.x >= updateLength_ - OFFSET && animStep_ >= ANIM_TIME)
	{
		ChangeState(STATE::AFTER_WAIT);
	}
}

void ScoreGage::UpdateStateAfterWait(void)
{
	bonus_->UpdateShake();
}

void ScoreGage::SetParamToPlayerNo(void)
{
	//座標設定
	pos_ = { GAGE_POS_P1_X, GAGE_POS_P1_Y };

	//プレイヤー番号ごとに座標をずらす
	pos_.y += playerIndex_ * GAGE_POS_Y_OFFSET;
}