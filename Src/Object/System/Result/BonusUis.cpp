#include "BonusUis.h"
#include <DxLib.h>
#include "../../../Manager/System/ResourceManager.h"
#include "../../../Manager/System/SceneManager.h"
#include "../../../Manager/Game/ScoreManager.h"
#include "../../../Utility/Utility.h"


BonusUis::BonusUis(const int _playerIndex) :playerIndex_(_playerIndex)
{
	timer_ = 0.0f;
}

BonusUis::~BonusUis()
{
}

void BonusUis::Load()
{
	ResourceManager& res = ResourceManager::GetInstance();
	uiInfos_[static_cast<int>(TYPE::CLEAR)].image = res.Load(ResourceManager::SRC::RESULT_GOAL).handleId_;
	uiInfos_[static_cast<int>(TYPE::FIRST)].image = res.Load(ResourceManager::SRC::RESULT_NO1).handleId_;
	uiInfos_[static_cast<int>(TYPE::COIN)].image = res.Load(ResourceManager::SRC::RESULT_COIN).handleId_;
}

void BonusUis::Init()
{
	constexpr int MARGIN_X = 80;
	constexpr int MARGIN_Y = 100;
	constexpr int OFFSET_Y = 30;

	for (int i = 0; i < static_cast<int>(TYPE::MAX); i++)
	{
		int offsetY = playerIndex_ * MARGIN_Y;
		if (i % 2 == 1)
		{
			offsetY -= OFFSET_Y;
		}
		else
		{
			offsetY += OFFSET_Y;
		}

		Info& it = uiInfos_[i];
		it.pos = { POS_X + i * MARGIN_X ,POS_Y + offsetY };
		it.alpha = 0;
		it.rate = RATE_MIN;
		it.isDisplay = false;
	}
}

void BonusUis::Update()
{
	constexpr float ANIM_TIME = 1.5f;
	constexpr float START = 0.5f;

	//タイマー更新
	timer_ += SceneManager::GetInstance().GetDeltaTime();

	for (int i = 0; i < static_cast<int>(TYPE::MAX); i++)
	{
		if (timer_ < i * START ||		//開始時間以下の時,または
			!uiInfos_[i].isDisplay)		//表示しないとき
		{
			continue;//飛ばす
		}

		//アルファ拡大
		uiInfos_[i].alpha += ALPHA_SPEED;

		//アニメーション
		uiInfos_[i].rate = Utility::EaseInOutBack(timer_ - i, ANIM_TIME, RATE_MIN, RATE_MAX);
	}
}

void BonusUis::UpdateShake()
{
	constexpr float SPEED = 2.0f;
	constexpr float AMPLITUDE = 0.002f;

	//タイマー更新
	timer_ += SceneManager::GetInstance().GetDeltaTime();

	for (int i = 0; i < static_cast<int>(TYPE::MAX); i++)
	{
		//アニメーション
		uiInfos_[i].rate = Utility::GetShake(uiInfos_[i].rate, timer_, SPEED, AMPLITUDE);
	}
}

void BonusUis::Draw()
{
	for (Info& ui : uiInfos_)
	{
		//表示しない場合
		if (!ui.isDisplay)
		{
			//飛ばす
			continue;
		}

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ui.alpha);
		DrawRotaGraph(
			ui.pos.x,
			ui.pos.y,
			//0.5f,
			ui.rate,
			0.0f,
			ui.image,
			true,
			false
		);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void BonusUis::SetBonus()
{
	ScoreManager& score = ScoreManager::GetInstance();

	Reset();

	if (score.GetIsBonus(playerIndex_, ScoreManager::SCORE_TYPE::CLEAR))
	{
		uiInfos_[static_cast<int>(TYPE::CLEAR)].isDisplay = true;
	}
	if (score.GetIsBonus(playerIndex_, ScoreManager::SCORE_TYPE::FIRST))
	{
		uiInfos_[static_cast<int>(TYPE::FIRST)].isDisplay = true;
	}
	if (score.GetIsBonus(playerIndex_, ScoreManager::SCORE_TYPE::COIN))
	{
		uiInfos_[static_cast<int>(TYPE::COIN)].isDisplay = true;
	}

}

void BonusUis::Reset()
{
	for (int i = 0; i < static_cast<int>(TYPE::MAX); i++)
	{
		uiInfos_[i].isDisplay = false;
		uiInfos_[i].alpha = 0.0f;
		uiInfos_[i].rate = RATE_MIN;
	}
	timer_ = 0.0f;
}
