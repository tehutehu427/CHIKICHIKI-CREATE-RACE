#include "Palette.h"
#include <DxLib.h>
#include "../../../Application.h"
#include "../../../Utility/Utility.h"
#include "../../../Manager/System/ResourceManager.h"
#include "../../../Manager/System/SceneManager.h"

namespace
{
	//画面外座標
	Vector2 screenOutPos =
	{ 
		static_cast<int>(Application::SCREEN_SIZE_X + Palette::SIZE_X * Palette::RATE_MAX / 2),
		Application::SCREEN_HALF_Y };

	//中央座標
	Vector2 centerPos = 
	{
		Application::SCREEN_HALF_X,
		Application::SCREEN_HALF_Y
	};

	//エディット用画面端座標
	Vector2 edgePos = 
	{ 
		Application::SCREEN_SIZE_X + Palette::EDGE_OFFSET,
		centerPos.y 
	};
}

Palette::Palette(void)
{
	stateChanges_.emplace(STATE::NONE, std::bind(&Palette::ChangeStateNone, this));
	stateChanges_.emplace(STATE::ADMISSION, std::bind(&Palette::ChangeStateAdmission, this));
	stateChanges_.emplace(STATE::ADMISSION_EDGE, std::bind(&Palette::ChangeStateAdmissionEdge, this));
	stateChanges_.emplace(STATE::EXIT, std::bind(&Palette::ChangeStateExit, this));
	stateChanges_.emplace(STATE::EXIT_EDGE, std::bind(&Palette::ChangeStateExitEdge, this));
	stateChanges_.emplace(STATE::EXPANSION, std::bind(&Palette::ChangeStateExpansion, this));
	stateChanges_.emplace(STATE::REDUCTION, std::bind(&Palette::ChangeStateReduction, this));
	stateChanges_.emplace(STATE::CENTER, std::bind(&Palette::ChangeStateCenter, this));
	stateChanges_.emplace(STATE::EDGE, std::bind(&Palette::ChangeStateEdge, this));

	imgPalette_ = -1;
	pos_ = {};
	startPos_ = {};
	finishPos_ = {};
	rate_ = 0.0f;
	state_ = STATE::NONE;
	nextState_ = STATE::NONE;
	time_ = 0.0f;
}

void Palette::Load(void)
{
	//リソースの読み込み
	ResourceManager& res = ResourceManager::GetInstance();
	imgPalette_ = res.Load(ResourceManager::SRC::PALETTE).handleId_;
}

void Palette::Init(void)
{
	//初期状態
	ChangeState(STATE::NONE);
}

void Palette::Update(void)
{
	stateUpdate_();
}

void Palette::Draw(void)
{
	if (state_ == STATE::NONE) { return; }	

	DrawRotaGraph(
		pos_.x,
		pos_.y,
		rate_,
		0.0f,
		imgPalette_,
		true,
		false);
}

void Palette::ChangeState(const STATE _state)
{
	//同じ状態だったら処理しない
	if (state_ == _state)return;

	//状態変更
	state_ = _state;

	//各状態への初期処理
	stateChanges_[state_]();
}

bool Palette::IsInsidePalette(const Vector2& _pos)
{
	Vector2 size = { static_cast<int>(SIZE_X * rate_),static_cast<int>(SIZE_Y * rate_) };
	Vector2 leftTop = { pos_.x - size.x / 2, pos_.y - size.y / 2 };
	Vector2 rightBottom = { pos_.x + size.x / 2, pos_.y + size.y / 2 };
	return Utility::IsPointInRect(_pos, leftTop, rightBottom);
}

void Palette::ChangeStateNone(void)
{
	//座標を初期化
	pos_ = screenOutPos;

	stateUpdate_ = std::bind(&Palette::UpdateStateNone, this);
}

void Palette::ChangeStateExpansion(void)
{
	//座標を中心へ設定
	pos_ = centerPos;

	//拡大率を初期化
	rate_ = 0.0f;

	//次の状態遷移先を設定
	nextState_ = STATE::CENTER;

	stateUpdate_ = std::bind(&Palette::UpdateStateExpansion, this);
}

void Palette::ChangeStateReduction(void)
{
	//座標を中心へ設定
	pos_ = centerPos;

	//拡大率を初期化
	rate_ = RATE_MAX;

	//次の状態遷移先を設定
	nextState_ = STATE::NONE;

	stateUpdate_ = std::bind(&Palette::UpdateStateReduction, this);
}

void Palette::ChangeStateAdmission(void)
{
	//座標を画面外へ設定
	pos_ = screenOutPos;

	//拡大率を設定
	rate_ = RATE_MAX;

	//時間初期化
	time_ = 0.0f;	

	//開始位置
	startPos_ = screenOutPos;

	//終了位置
	finishPos_ = centerPos;

	//次の状態遷移先を設定
	nextState_ = STATE::CENTER;

	stateUpdate_ = std::bind(&Palette::UpdateStateAdmission, this);
}

void Palette::ChangeStateAdmissionEdge(void)
{
	//座標を端へ設定
	pos_ = edgePos;

	//拡大率を設定
	rate_ = RATE_MAX;

	//時間初期化
	time_ = 0.0f;

	//開始位置
	startPos_ = edgePos;

	//終了位置
	finishPos_ = centerPos;

	//次の状態遷移先を設定
	nextState_ = STATE::CENTER;

	stateUpdate_ = std::bind(&Palette::UpdateStateAdmission, this);
}

void Palette::ChangeStateExit(void)
{
	//座標を中心へ設定
	pos_ = centerPos;

	//拡大率を設定
	rate_ = RATE_MAX;

	//時間初期化
	time_ = 0.0f;

	//開始位置
	startPos_ = centerPos;

	//終了位置
	finishPos_ = screenOutPos;

	//次の状態遷移先を設定
	nextState_ = STATE::NONE;

	stateUpdate_ = std::bind(&Palette::UpdateStateExit, this);
}

void Palette::ChangeStateExitEdge(void)
{
	//座標を中心へ設定
	pos_ = centerPos;

	//拡大率を設定
	rate_ = RATE_MAX;

	//時間初期化
	time_ = 0.0f;

	//開始位置
	startPos_ = centerPos;

	//終了位置
	finishPos_ = edgePos;

	//次の状態遷移先を設定
	nextState_ = STATE::EDGE;

	stateUpdate_ = std::bind(&Palette::UpdateStateExit, this);
}

void Palette::ChangeStateCenter(void)
{
	//座標を中心へ設定
	pos_ = centerPos;

	//拡大率を設定
	rate_ = RATE_MAX;

	stateUpdate_ = std::bind(&Palette::UpdateStateNone, this);
}

void Palette::ChangeStateEdge(void)
{
	//座標を端の方へ設定
	pos_ = edgePos;

	//拡大率を設定
	rate_ = RATE_MAX;

	stateUpdate_ = std::bind(&Palette::UpdateStateNone, this);
}

void Palette::UpdateStateNone(void)
{
	//処理は実行しない
}

void Palette::UpdateStateExpansion(void)
{
	rate_ += RATE_SPEED;

	if (rate_ >= RATE_MAX)
	{
		ChangeState(nextState_);
	}
}

void Palette::UpdateStateReduction(void)
{
	rate_ -= RATE_SPEED;

	if (rate_ <= 0.0f)
	{
		ChangeState(nextState_);
	}
}

void Palette::UpdateStateAdmission(void)
{
	//移動処理
	Move();

	//目的地か調べる
	if (pos_.x <= finishPos_.x)
	{
		ChangeState(nextState_);
	}
}

void Palette::UpdateStateExit(void)
{
	constexpr int OFFSET_POS_X = 1;	//ゴール調整用

	//移動処理
	Move();

	//目的地か調べる
	if (pos_.x >= finishPos_.x- OFFSET_POS_X)
	{
 		ChangeState(nextState_);
	}
}

void Palette::Move(void)
{	
	//アニメーション時間
	constexpr float ANIM_TIME = 1.5f;

	// 目的地との距離
	float distance = fabs(finishPos_.x - startPos_.x);

	//アニメーション総時間
	float duration = distance / MOVE_SPEED;

	//時間更新
	time_ += SceneManager::GetInstance().GetDeltaTime();

	//座標計算
	pos_.x = Utility::EaseOutQuad (time_, ANIM_TIME, startPos_.x, finishPos_.x);

}