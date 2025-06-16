#include "ScoreManager.h"
#include <cassert>
#include "../System/DateBank.h"

ScoreManager* ScoreManager::instance_ = nullptr;

ScoreManager::~ScoreManager()
{
}

void ScoreManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ScoreManager();
	}
	// 初期化処理を呼び出す
	instance_->Init();
}

ScoreManager& ScoreManager::GetInstance(void)
{
	return *instance_;
}

void ScoreManager::Init()
{
	// プレイヤー数に応じてスコアを初期化
	int playerNum = DateBank::GetInstance().GetPlayerNum();
	scores_.resize(playerNum, 0); // 各プレイヤーのスコアを0で初期化
}

void ScoreManager::Destroy()
{
	if (instance_ != nullptr)
	{
		delete instance_;
		instance_ = nullptr;
	}
}

void ScoreManager::AddScore(const int _playerIndex, const SCORE_TYPE _type)
{
	assert(_playerIndex >= 0 && _playerIndex < static_cast<int>(scores_.size()));
	scores_[_playerIndex] += SCORE_TYPE_VALUES[static_cast<int>(_type)];
}

const int ScoreManager::GetWinnerPlayerIndex(const int _clearLine) const
{
	std::vector<int> winners;
	for (int i = 0; i < scores_.size(); i++)
	{
		// スコアがクリアライン以上のプレイヤーを探す
		if (scores_[i] >= _clearLine)
		{
			winners.push_back(i);
		}
	}

	// クリアしたプレイヤーがいない場合、-1を返す
	if (winners.empty())
	{
		return -1;
	}
	// 1人だけクリアした場合、そのプレイヤーのインデックスを返す
	else if (winners.size() == 1) 
	{
		return winners[0];
	}
	// 複数のプレイヤーがクリアした場合、最もスコアが高いプレイヤーを返す
	else if (winners.size() > 1) 
	{
		int maxScore = -1;		//最大スコア
		int winnerIndex = -1;	//勝者
		for (int index : winners)
		{
			//現在の最大スコアより大きい場合、更新
			if (scores_[index] > maxScore)
			{
				maxScore = scores_[index];	
				winnerIndex = index;
			}
			//同じ場合(引き分けの場合)
			else if (scores_[index] == maxScore)
			{
				//勝者を-1にしておく
				winnerIndex = -1;
			}
		}
		return winnerIndex;
	}	
	else
	{
		return -1;
	}
}

ScoreManager::ScoreManager()
{
	scores_.clear();
}