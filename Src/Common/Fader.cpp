#include <DxLib.h>
#include "../Application.h"
#include "../Utility/Utility.h"
#include "Fader.h"

Fader::Fader()
{
	state_ = STATE::NONE;
	alpha_ = -1;
	alphaMax_ = -1;
	speed_ = -1;
	isPreEnd_ = false;
	isEnd_ = false;
	imgMask_ = -1;
	tmpScreen_ = -1;
}

Fader::~Fader()
{
	DeleteGraph(imgMask_);
	DeleteGraph(tmpScreen_);
}

void Fader::Init(void)
{
	state_ = STATE::NONE;
	alpha_ = 255;
	isPreEnd_ = true;
	isEnd_ = true;

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

	if (isEnd_)
	{
		return;
	}

	switch (state_)
	{
	case STATE::NONE:
		return;

	case STATE::FADE_OUT:
		alpha_ += SPEED_ALPHA;
		if (alpha_ > 255)
		{
			// フェード終了
			alpha_ = 255;
			if (isPreEnd_)
			{
				// 1フレーム後(Draw後)に終了とする
				isEnd_ = true;
			}
			isPreEnd_ = true;
		}

		break;

	case STATE::FADE_IN:
		alpha_ -= SPEED_ALPHA;
		if (alpha_ < 0)
		{
			// フェード終了
			alpha_ = 0;
			if (isPreEnd_)
			{
				// 1フレーム後(Draw後)に終了とする
				isEnd_ = true;
			}
			isPreEnd_ = true;
		}
		break;

	default:
		return;
	}

}

void Fader::Draw(void)
{

	switch (state_)
	{
	case STATE::NONE:
		return;
	case STATE::FADE_OUT:
	case STATE::FADE_IN:
	/*	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)alpha_);
		DrawBox(
			0, 0,
			Application::SCREEN_SIZE_X,
			Application::SCREEN_SIZE_Y,
			0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);*/
		SpriteMask();
		break;
	}

}

void Fader::SetFade(STATE state)
{
	state_ = state;
	if (state_ != STATE::NONE)
	{
		isPreEnd_ = false;
		isEnd_ = false;
	}
}

void Fader::SpriteMask()
{
	constexpr float ALPHA_SPEED = 0.1f;

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

	//拡大率
	float rate = (ALPHA_MAX - alpha_) / ALPHA_MAX * ALPHA_SPEED;
	rate = std::clamp(rate, 0.0f, 1.0f);

	//白色の円を描画する
	//alpha値を利用して大きさを制御
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	DrawRotaGraph(
		Application::SCREEN_HALF_X,
		Application::SCREEN_HALF_Y,
		rate,
		0.0f,
		imgMask_,
		true);

	//描画領域を元に戻す
	SetDrawScreen(DX_SCREEN_BACK);

	//描画を色の乗算モードにする
	SetDrawBlendMode(DX_BLENDMODE_MUL, 0);

	//元々のゲーム画面にマスク画像を描画する
	DrawGraph(0, 0, tmpScreen_, false);

	//描画モードを元に戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}