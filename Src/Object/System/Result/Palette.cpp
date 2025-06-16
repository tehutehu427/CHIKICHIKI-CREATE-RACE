#include "Palette.h"
#include <DxLib.h>
#include "../../../Application.h"
#include "../../../Utility/Utility.h"
#include "../../../Manager/System/ResourceManager.h"
#include "../../../Manager/System/SceneManager.h"

namespace
{
	//初期座標
	Vector2 fistPos ={ 
		static_cast<int>(Application::SCREEN_SIZE_X + Palette::SIZE_X * Palette::RATE_MAX / 2),
		Application::SCREEN_HALF_Y };

	//中央座標
	Vector2 centerPos = { Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y };
}

Palette::Palette()
{
	stateChanges_.emplace(STATE::NONE, std::bind(&Palette::ChangeStateNone, this));
	stateChanges_.emplace(STATE::ADMISSION, std::bind(&Palette::ChangeStateAdmission, this));
	stateChanges_.emplace(STATE::EXIT, std::bind(&Palette::ChangeStateExit, this));
	stateChanges_.emplace(STATE::EXPANSION, std::bind(&Palette::ChangeStateExpansion, this));
	stateChanges_.emplace(STATE::REDUCTION, std::bind(&Palette::ChangeStateReduction, this));
	stateChanges_.emplace(STATE::CENTER, std::bind(&Palette::ChangeStateCenter, this));

	imgPalette_ = -1;
	pos_ = {};
	rate_ = 0.0f;
	state_ = STATE::NONE;
	time_ = 0.0f;
}

Palette::~Palette()
{
}

void Palette::Load()
{
	//リソースの読み込み
	ResourceManager& res = ResourceManager::GetInstance();
	imgPalette_ = res.Load(ResourceManager::SRC::PALETTE).handleId_;
}

void Palette::Init()
{

	//初期状態
	ChangeState(STATE::NONE);
}

void Palette::Update()
{
	stateUpdate_();
}

void Palette::Draw()
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

void Palette::ChangeStateNone()
{
	//座標を初期化
	pos_ = fistPos;

	stateUpdate_ = std::bind(&Palette::UpdateStateNone, this);
}

void Palette::ChangeStateExpansion()
{
	//座標を中心へ設定
	pos_ = centerPos;

	//拡大率を初期化
	rate_ = 0.0f;

	stateUpdate_ = std::bind(&Palette::UpdateStateExpansion, this);
}

void Palette::ChangeStateReduction()
{
	//座標を中心へ設定
	pos_ = centerPos;

	//拡大率を初期化
	rate_ = 0.0f;

	stateUpdate_ = std::bind(&Palette::UpdateStateReduction, this);
}

void Palette::ChangeStateAdmission()
{
	//座標を中心へ設定
	pos_ = fistPos;

	//拡大率を設定
	rate_ = RATE_MAX;

	//時間初期化
	time_ = 0.0f;

	stateUpdate_ = std::bind(&Palette::UpdateStateAdmission, this);
}

void Palette::ChangeStateExit()
{
	//座標を中心へ設定
	pos_ = centerPos;

	//拡大率を設定
	rate_ = RATE_MAX;

	//時間初期化
	time_ = 0.0f;

	stateUpdate_ = std::bind(&Palette::UpdateStateExit, this);
}

void Palette::ChangeStateCenter()
{
	//座標を中心へ設定
	pos_ = centerPos;

	//拡大率を設定
	rate_ = RATE_MAX;

	stateUpdate_ = std::bind(&Palette::UpdateStateNone, this);
}

void Palette::UpdateStateNone()
{
	//処理は実行しない
}

void Palette::UpdateStateExpansion()
{
	rate_ += RATE_SPEED;

	if (rate_ >= RATE_MAX)
	{
		ChangeState(STATE::CENTER);
	}
}

void Palette::UpdateStateReduction()
{
	rate_ -= RATE_SPEED;

	if (rate_ <= 0.0f)
	{
		ChangeState(STATE::NONE);
	}
}

void Palette::UpdateStateAdmission()
{
	// 目的地との距離
	float distance = centerPos.x - fistPos.x;  

	//アニメーション総時間
	float duration = fabs(distance) / MOVE_SPEED;

	//時間更新
	time_ += SceneManager::GetInstance().GetDeltaTime();

	//座標計算
	pos_.x = Utility::EaseOutQuad (time_, 1.5f, fistPos.x, centerPos.x);

	//目的地か調べる
	if (pos_.x <= centerPos.x)
	{
		ChangeState(STATE::CENTER);
	}
}

void Palette::UpdateStateExit()
{
	// 目的地との距離
	float distance = fabs(fistPos.x - centerPos.x);

	//アニメーション総時間
	float duration = distance / MOVE_SPEED;

	//時間更新
	time_ += SceneManager::GetInstance().GetDeltaTime();

	//座標計算
	pos_.x += Utility::EaseInQuad(time_, 1.5f, centerPos.x, fistPos.x);

	//目的地か調べる
	if (pos_.x <= centerPos.x)
	{
		ChangeState(STATE::NONE);
	}
}
