#include <DxLib.h>
#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Manager/System/SceneManager.h"
#include "Fader.h"

Fader::Fader()
{
	state_ = STATE::NONE;
	isPreEnd_ = false;
	isEnd_ = false;
	imgMask_ = -1;
	tmpScreen_ = -1;

	//処理の登録
	RegisterStateUpdate(STATE::FADE_IN, [&]() {UpdateFadeIn(); });
	RegisterStateUpdate(STATE::FADE_OUT, [&]() {UpdateFadeOut(); });
	RegisterStateUpdate(STATE::NONE, [&]() {UpdateNone(); });
}

Fader::~Fader()
{
	DeleteGraph(imgMask_);
	DeleteGraph(tmpScreen_);
}

void Fader::Init(void)
{
	state_ = STATE::NONE;
	isPreEnd_ = true;
	isEnd_ = true;
	rate_ = 0.0f;
	time_ = 0.0f;

	//リソースを読み込み
	imgMask_ = LoadGraph((Application::PATH_IMAGE + "Fader/Fade.png").c_str());

	tmpScreen_ = MakeScreen(
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		true
	);
}

void Fader::Update(void)
{
	//終了してるときは何も実行しない
	if (isEnd_)
	{
		return;
	}
	//状態別更新処理
	stateUpdateMap_[state_]();
}

void Fader::Draw(void)
{
	//状態がないときは実行しない
	if (state_ == STATE::NONE)
	{
		return;
	}

	//画像マスク処理
	SpriteMask();
}

void Fader::SetFade(const STATE _state)
{
	state_ = _state;
	if (state_ != STATE::NONE)
	{
		isPreEnd_ = false;
		isEnd_ = false;
	}
}

void Fader::RegisterStateUpdate(const STATE _state, const std::function<void(void)> _func)
{
	stateUpdateMap_[_state] = _func;
}

void Fader::UpdateFadeIn()
{
	time_ += SceneManager::GetInstance().GetDeltaTime();

	// rate を EaseInQuad で計算
	rate_ = Utility::EaseInQuad(
		time_,
		TOTAL_TIME,
		0.0f,        // 開始値
		RATE_MAX     // 終了値
	);

	if (time_ >= TOTAL_TIME || rate_ >= RATE_MAX)
	{
		rate_ = RATE_MAX;
		if (isPreEnd_)
		{
			isEnd_ = true;
			time_ = 0.0f;
		}
		isPreEnd_ = true;
	}
}

void Fader::UpdateFadeOut()
{
	time_ += SceneManager::GetInstance().GetDeltaTime();

	// rate を EaseOutQuad で計算
	rate_ = Utility::EaseOutQuad(
		time_,          // 経過時間
		TOTAL_TIME,     // 総時間
		RATE_MAX,       // 開始値
		0.0f            // 終了値
	);

	if (time_ >= TOTAL_TIME || rate_ <= 0.0f)
	{
		rate_ = 0.0f;
		if (isPreEnd_)
		{
			isEnd_ = true;
			time_ = 0.0f;
		}
		isPreEnd_ = true;
	}
}

void Fader::SpriteMask()
{
	// 描画領域をマスク画像領域に切り替える
	// 元々は、背面スクリーンになっている
	SetDrawScreen(tmpScreen_);

	//画面全体を黒に塗る
	DrawBox(
		0, 0,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		Utility::BLACK,
		true);

	//白色の画像を描画
	DrawRotaGraph(
		Application::SCREEN_HALF_X,
		Application::SCREEN_HALF_Y,
		rate_,
		0.0f,
		imgMask_,
		true);

	//描画領域を元に戻す
	SetDrawScreen(SceneManager::GetInstance().GetMainScreen());

	//描画を色の乗算モードにする
	SetDrawBlendMode(DX_BLENDMODE_MUL, 0);

	//元々のゲーム画面にマスク画像を描画する
	DrawGraph(0, 0, tmpScreen_, false);

	//描画モードを元に戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}