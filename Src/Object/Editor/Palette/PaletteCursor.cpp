#include "PaletteCursor.h"
#include "../../../Application.h"
#include "../../../Utility/Utility.h"

PaletteCursor::PaletteCursor(const int _plNum, const int _img) : 
	playerNum_(_plNum),
	img_(_img)
{
	//初期化
	pad_ = InputManager::JOYPAD_NO::PAD1;
	pos_ = { -1,-1 };
	decide_ = false;
	rate_ = -1.0f;
}

PaletteCursor::~PaletteCursor()
{
}

void PaletteCursor::Init()
{
	//パッド情報
	pad_ = static_cast<InputManager::JOYPAD_NO>(playerNum_ + 1);

	//初期設定
	Reset();
}

void PaletteCursor::Update()
{
	//決定済みの場合
	if (decide_) { return; }

	auto& ins = InputManager::GetInstance();

	//スティック
	int stickX = ins.GetJPadInputState(pad_).AKeyLX;
	int stickY = ins.GetJPadInputState(pad_).AKeyLY;

	//操作処理
	if (stickX > 0)
	{
		pos_.x += MOVE_POW;
		if (pos_.x > Application::SCREEN_SIZE_X)
		{
			pos_.x = Application::SCREEN_SIZE_X;
		}
	}
	if (stickX < 0)
	{
		pos_.x -= MOVE_POW;
		if (pos_.x < 0)
		{
			pos_.x = 0;
		}
	}
	if (stickY < 0)
	{
		pos_.y -= MOVE_POW;
		if (pos_.y < 0)
		{
			pos_.y = 0;
		}
	}
	if (stickY > 0)
	{
		pos_.y += MOVE_POW;
		if (pos_.y > Application::SCREEN_SIZE_Y)
		{
			pos_.y = Application::SCREEN_SIZE_Y;
		}
	}

	//決定
	if (ins.IsPadBtnTrgDown(pad_, InputManager::JOYPAD_BTN::RIGHT))
	{
		decide_ = true;
	}
}

void PaletteCursor::Draw()
{
	//角度
	float angle = Utility::Deg2RadF(ANGLE);

	//決定済みの場合は描画しない
	if (decide_) { return; }

	//アイコン描画
	DrawRotaGraph(
		pos_.x,
		pos_.y,
		rate_,
		angle,
		img_,
		true,
		false);
}

void PaletteCursor::Reset()
{
	//決定
	decide_ = false;

	//座標
	pos_ = { DEFAULT_POS_X * playerNum_,DEFAULT_POS_Y };

	//拡大率
	rate_ = RATE_SCALE;
}