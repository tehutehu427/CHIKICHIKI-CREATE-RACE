#include "ScoreGage.h"
#include <DxLib.h>
#include "../Utility/Utility.h"
#include "../Manager/Game/ScoreManager.h"
#include "../Manager/System/SceneManager.h"

ScoreGage::ScoreGage(const int _playerIndex) : 
	playerIndex_(_playerIndex)
{
	pos_ = {};
	size_ = {};
	imgScoreGage_ = 0;
	rate_ = 0.0f;
	state_ = STATE::NONE;
	stateChanges_.emplace(STATE::NONE, std::bind(&ScoreGage::ChangeStateNone, this));
	stateChanges_.emplace(STATE::WAIT, std::bind(&ScoreGage::ChangeStateWait, this));
	stateChanges_.emplace(STATE::ANIMATION, std::bind(&ScoreGage::ChangeStateAnimation, this));
}

ScoreGage::~ScoreGage()
{
}

void ScoreGage::Load()
{
}

void ScoreGage::Init()
{
	//初期状態
	ChangeState(STATE::NONE);

	//プレイヤー番号ごとに設定
	SetParamToPlayerNo();	

	//ゲージサイズ
	size_ = { 100,GAGE_SIZE_Y };

	//デバッグ
	//ScoreManager::GetInstance().AddScore(playerIndex_, ScoreManager::SCORE_TYPE::CLEAR);
}

void ScoreGage::Update()
{
	stateUpdate_();
}

void ScoreGage::Draw()
{
	//一定の状態の場合描画させない
	if (state_ == STATE::NONE) { return; }

	//スコアゲージの描画
	DrawBox(
		pos_.x,
		pos_.y,
		pos_.x + size_.x,
		pos_.y + size_.y,
		color_,
		true);
}

void ScoreGage::ChangeState(const STATE _state)
{
	//状態変更
	state_ = _state;

	//状態別変更処理
	stateChanges_[state_]();	
}

void ScoreGage::ChangeStateNone()
{
	stateUpdate_ = std::bind(&ScoreGage::UpdateStateNone, this);
}

void ScoreGage::ChangeStateWait()
{
	stateUpdate_ = std::bind(&ScoreGage::UpdateStateWait, this);
}

void ScoreGage::ChangeStateAnimation()
{
	stateUpdate_ = std::bind(&ScoreGage::UpdateStateAnimation, this);

	//長さの更新値を決定
	updateLength_ = size_.x + ScoreManager::GetInstance().GetScore(playerIndex_) * GAGE_LENGTH_PER_POINT;
}

void ScoreGage::UpdateStateNone()
{
}

void ScoreGage::UpdateStateWait()
{
}

void ScoreGage::UpdateStateAnimation()
{
	animStep_ += SceneManager::GetInstance().GetDeltaTime();

	//必要な情報を設定
	constexpr float ANIM_TIME = 3.0f;	//アニメーション時間
	float start = static_cast<float>(size_.x);	//開始位置
	float goal = static_cast<float>(updateLength_);	//目標位置

	//移動量を求める
	float move = Utility::EaseInQuad(animStep_, ANIM_TIME, start, goal);

	//座標更新
	size_.x = static_cast<int>(move);

	//目標位置に到達したら状態を変更
	if (size_.x >= updateLength_)
	{
		ChangeState(STATE::WAIT);
	}
}

void ScoreGage::SetParamToPlayerNo()
{
	//座標設定
	pos_ = { GAGE_POS_P1_X, GAGE_POS_P1_Y };

	//プレイヤー番号ごとに座標をずらす
	pos_.y += playerIndex_ * GAGE_POS_Y_OFFSET;

	//カラー
	switch (playerIndex_)
	{
	case 0:
		color_ = Utility::BLUE;
		break;

	case 1:
		color_ = Utility::PINK;
		break;

	case 2:
		color_ = Utility::GREEN;
		break;

	case 3:
		color_ = Utility::YELLOW;
		break;
	}
}
