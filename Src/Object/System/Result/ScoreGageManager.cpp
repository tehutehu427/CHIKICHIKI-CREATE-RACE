#include "ScoreGageManager.h"
#include "../../../Application.h"
#include "../../../Manager/System/DateBank.h"
#include "../../../Manager/Game/ScoreManager.h"
#include "../../../Manager/System/ResourceManager.h"
#include "../../../Utility/Utility.h"

ScoreGageManager::ScoreGageManager(void)
{
	alphaDir_ = -1; 
	imgTitle_ = -1;
	mesAlpha_ = -1; 
	imgDraw_ = -1;
	imgPushButton_ = -1;
	imgIsWinning_ = -1;
	imgPlayers_ = nullptr;
}

ScoreGageManager::~ScoreGageManager(void)
{
}

void ScoreGageManager::Load(void)
{
	ResourceManager& res = ResourceManager::GetInstance();
	imgTitle_ = res.Load(ResourceManager::SRC::PROGRESS).handleId_;
	imgPushButton_ = res.Load(ResourceManager::SRC::PUSH_B_BUTTON_MES).handleId_;
	imgIsWinning_ = res.Load(ResourceManager::SRC::IS_WINNING_MES).handleId_;
	imgPlayers_ = res.Load(ResourceManager::SRC::PLAYER_PLATES).handleIds_;
	imgDraw_ = res.Load(ResourceManager::SRC::DRAW_MES).handleId_;

	int playerNum = DateBank::GetInstance().GetPlayerNum();
	for (int i = 0; i < playerNum; ++i)
	{
		auto scoreGage = std::make_unique<ScoreGage>(i);
		scoreGage->Load();
		scoreGages_.emplace_back(std::move(scoreGage));
	}
}

void ScoreGageManager::Init(void)
{
	for (auto& scoreGage : scoreGages_)
	{
		scoreGage->Init();
	}

	mesAlpha_ = 0; //メッセージのアルファ値
	alphaDir_ = 1; //アルファ値の変化方向

}

void ScoreGageManager::Update(void)
{
	for (auto& scoreGage : scoreGages_)
	{
		scoreGage->Update();
	}
}

void ScoreGageManager::Draw(void)
{
	DrawGageDecoration();

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

const bool ScoreGageManager::IsFinishAnimation(void) const
{
	for (auto& scoreGage : scoreGages_)
	{
		//ゲージがまだアニメーション状態の時
		if (scoreGage->GetState() != ScoreGage::STATE::AFTER_WAIT)
		{
			//アニメーションが終わっていないのでfalseを返す
			return false;
		}
	}
	//全てのゲージがアニメーションを終えてたのでtrueを返す
	return true;
}

void ScoreGageManager::DrawGageDecoration(void)
{	
	constexpr int LENGTH = 350;
	constexpr float THICKNESS = 5.0f;

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

}

void ScoreGageManager::DrawPushButton(void)
{	
	constexpr float ALPHA_STEP = 1.5f; //アルファ値の変化量
	constexpr float ALPHA_MIN = 50.0f; //アルファ値の変化量

	//アルファ値を変え
	mesAlpha_ = Utility::PingPongUpdate(mesAlpha_, ALPHA_STEP, Utility::ALPHA_MAX, ALPHA_MIN, alphaDir_);

	//ボタンを押してね画像の描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, mesAlpha_);
	DrawRotaGraph(
		Application::SCREEN_HALF_X,
		Application::SCREEN_HALF_Y,
		1.0f,
		0.0f,
		imgPushButton_,
		true,
		false
	);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void ScoreGageManager::DrawTitle(void)
{
	constexpr int TITLE_POS_Y = 50;
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

void ScoreGageManager::DrawIsWinning(void)
{	
	ScoreManager & score = ScoreManager::GetInstance();
	
	//見出しのY座標
	constexpr int TITLE_POS_Y = 50;

	if (score.IsDraw())
	{
		//引き分け
		DrawRotaGraph(
			Application::SCREEN_HALF_X,
			TITLE_POS_Y,
			0.7f,
			0.0f,
			imgDraw_,
			true
		);
	}
	else
	{	
		constexpr int PLYAER_OFFSET_POS_X = -150;
		constexpr int WINNING_OFFSET_POS_X = 194;
		int index = score.GetNowWinnerPlayerIndex();

		//プレイヤー名の描画
		DrawRotaGraph(
			Application::SCREEN_HALF_X + PLYAER_OFFSET_POS_X,
			TITLE_POS_Y - 10,
			0.7f,
			0.0f,
			imgPlayers_[index],
			true
		);

		//勝利メッセージの描画
		DrawRotaGraph(
			Application::SCREEN_HALF_X + WINNING_OFFSET_POS_X,
			TITLE_POS_Y,
			1.0f,
			0.0f,
			imgIsWinning_,
			true
		);
	}
}