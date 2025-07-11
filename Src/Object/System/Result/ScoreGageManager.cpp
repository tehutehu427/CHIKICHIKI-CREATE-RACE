#include "ScoreGageManager.h"
#include "../../../Application.h"
#include "../../../Manager/System/DateBank.h"
#include "../../../Manager/System/ResourceManager.h"
#include "../../../Utility/Utility.h"

ScoreGageManager::ScoreGageManager()
{
}

ScoreGageManager::~ScoreGageManager()
{
}

void ScoreGageManager::Load()
{
	ResourceManager& res = ResourceManager::GetInstance();
	imgTitle_ = res.Load(ResourceManager::SRC::PROGRESS).handleId_;

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

void ScoreGageManager::DecorationDraw()
{	
	constexpr int LENGTH = 350;
	constexpr float THICKNESS = 5.0f;
	constexpr int TITLE_POS_Y = 50;

	//縮小開始ライン
	DrawLine(
		ScoreGage::GAGE_SIZE_X + ScoreGage::GAGE_POS_P1_X,
		ScoreGage::GAGE_POS_P1_Y, 
		ScoreGage::GAGE_SIZE_X + ScoreGage::GAGE_POS_P1_X,
		ScoreGage::GAGE_POS_P1_Y + LENGTH,
		Utility::BLACK,
		THICKNESS
		);

	//クリアライン
	DrawLine(
		ScoreGage::GAGE_SIZE_X + ScoreGage::GAGE_POS_P1_X + ScoreGage::GAGE_LENGTH_MAX,
		ScoreGage::GAGE_POS_P1_Y,
		ScoreGage::GAGE_SIZE_X + ScoreGage::GAGE_POS_P1_X + ScoreGage::GAGE_LENGTH_MAX,
		ScoreGage::GAGE_POS_P1_Y + LENGTH,
		Utility::BLACK,
		THICKNESS
	);

	//見出しの描画
	DrawRotaGraph(
		Application::SCREEN_HALF_X,
		TITLE_POS_Y,
		1.0f,
		0.0f,
		imgTitle_,
		true
	);
}
