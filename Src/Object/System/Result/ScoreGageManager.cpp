#include "ScoreGageManager.h"
#include "../../../Manager/System/DateBank.h"

ScoreGageManager::ScoreGageManager()
{
}

ScoreGageManager::~ScoreGageManager()
{
}

void ScoreGageManager::Load()
{
	int playerNum = DateBank::GetInstance().GetPlayerNum();
	for (int i = 0; i < playerNum; ++i)
	{
		auto scoreGage = std::make_unique<ScoreGage>(i);
		scoreGage->Load();
		scoreGages_.emplace_back(std::move(scoreGage));
	}
}

void ScoreGageManager::Init()
{
	for (auto& scoreGage : scoreGages_)
	{
		scoreGage->Init();
	}

}

void ScoreGageManager::Update()
{
	for (auto& scoreGage : scoreGages_)
	{
		scoreGage->Update();
	}
}

void ScoreGageManager::Draw()
{
	for (auto& scoreGage : scoreGages_)
	{
		scoreGage->Draw();
	}
}

void ScoreGageManager::ChangeAllState(const ScoreGage::STATE _state)
{
	for (auto& scoreGage : scoreGages_)
	{
		scoreGage->ChangeState(_state);
	}
}

const bool ScoreGageManager::IsFinishAnimation() const
{
	for (auto& scoreGage : scoreGages_)
	{
		//ゲージがまだアニメーション状態の時
		if (scoreGage->GetState() == ScoreGage::STATE::ANIMATION)
		{
			//アニメーションが終わっていないのでfalseを返す
			return false;
		}
	}
	//全てのゲージがアニメーションを終えてたのでtrueを返す
	return true;
}
