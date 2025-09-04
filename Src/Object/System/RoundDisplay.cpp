#include "RoundDisplay.h"
#include <DxLib.h>
#include <cmath>
#include "../../Application.h"
#include "../../Manager/System/ResourceManager.h"
#include "../../Manager/Game/ScoreManager.h"
#include "../../Utility/Utility.h"

RoundDisplay::RoundDisplay()
{
	int i = -1;
	imgNumbers_ = &i;
	imgRound_ = -1;
	numberIndex_ = -1;
	numberDigit_ = -1;
}

RoundDisplay::~RoundDisplay()
{
	
}

void RoundDisplay::Load()
{
	ResourceManager& res = ResourceManager::GetInstance();
	imgRound_ = res.Load(ResourceManager::SRC::ROUND).handleId_;
	imgNumbers_ = res.Load(ResourceManager::SRC::NUMBERS).handleIds_;
}

void RoundDisplay::Init()
{
	//初期値
	numberIndex_ = 0;
	numberDigit_ = 0;
}

void RoundDisplay::Update()
{
	//アルファ値を変更しつつ表示する
}

void RoundDisplay::Draw()
{
	//縦座標を固定
	constexpr int ROUND_POS_X = Application::SCREEN_HALF_X - 100;
	constexpr int NUMBER_POS_X = Application::SCREEN_HALF_X + 230;
	constexpr int NUMBER_OFFSET_X = 70;
	constexpr int POS_Y = Application::SCREEN_HALF_Y;

	//ラウンドの描画
	DrawRotaGraph(
		ROUND_POS_X,
		POS_Y,
		1.0f,
		0.0f,
		imgRound_,
		true,
		false
	);

	//数字の描画
	for (int i = 0; i < numberDigit_; i++)
	{
		int index = Utility::GetDigit(numberIndex_, numberDigit_ - 1 - i);

		DrawRotaGraph(
			NUMBER_POS_X + NUMBER_OFFSET_X * i,
			POS_Y,
			1.0f,
			0.0f,
			imgNumbers_[index],
			true,
			false
		);
	}
}

void RoundDisplay::AddNumberIndex(const int _addIndex)
{
	//ナンバーインデックスを追加
	numberIndex_ += _addIndex;

	//ラウンド追加
	ScoreManager::GetInstance().SetRound(numberIndex_);

	//桁数
	numberDigit_ = Utility::GetDigitCount(numberIndex_);
}