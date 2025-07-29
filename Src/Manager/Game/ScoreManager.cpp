#include "ScoreManager.h"
#include <cassert>
#include <algorithm>
#include "../System/DateBank.h"
#include "PlayerManager.h"

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

	//初期化
	scores_.clear();			// 既存のスコアをクリア
	isBonusScoreTypes_.clear();

	// 各プレイヤーのスコアを0で初期化
	scores_.resize(playerNum, 0); 

	// 各プレイヤーの判定を初期化
	std::unordered_map<SCORE_TYPE, bool> ret = {};
	isBonusScoreTypes_.resize(playerNum, ret);
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
	//プレイヤー番号が範囲内か調べる
	assert(_playerIndex >= 0 && _playerIndex < static_cast<int>(scores_.size()));

	//スコア格納
	scores_[_playerIndex] += SCORE_TYPE_VALUES[static_cast<int>(_type)];

	//ボーナス判定格納
	isBonusScoreTypes_[_playerIndex][_type] = true;
}

void ScoreManager::SetPlayersScore()
{
	PlayerManager& pMng = PlayerManager::GetInstance();

	//コイン獲得者がいるか調べる
	for (int i = 0; i < scores_.size(); i++)
	{
		int num = pMng.GetPlayerCoinNum(i);

		//コイン獲得数が0より大きいとき
		if (num > 0)
		{	
			//獲得した数分スコア格納
			for (int j = 0; j < num; j++)
			{
				AddScore(i, SCORE_TYPE::COIN);
			}
		}
	}


	//クリアタイム取得
	std::vector<float> playerTimes = pMng.GetGoalTime();

	//配列のサイズが一緒か調べる
	int ret = playerTimes.size() == scores_.size() ? 0 : -1;
	assert(ret != -1);

	// プレイヤーのインデックスとタイムをペアで保持
	std::vector<std::pair<int, float>> indexAndTime;
	for (int i = 0; i < playerTimes.size(); i++)
	{
		if (!pMng.IsPlayerDeath(i)) {
			indexAndTime.emplace_back(i, playerTimes[i]);
		}
	}

	// タイムを基準に昇順ソート（小さいほど速い）
	std::sort(indexAndTime.begin(), indexAndTime.end(),
		[](const auto& a, const auto& b) {
			return a.second < b.second;
		});

	// ランクに応じてスコアを追加
	for (int rank = 0; rank < indexAndTime.size(); rank++)
	{
		int playerIndex = indexAndTime[rank].first;

		//クリア者にスコアを付与
		AddScore(playerIndex, SCORE_TYPE::CLEAR);

		//一位の場合追加点
		if (rank == 0)
		{
			AddScore(playerIndex, SCORE_TYPE::FIRST);
		}
	}
}

const int ScoreManager::GetWinnerPlayerIndex() const
{
	int clearLine = DateBank::GetInstance().GetMultiClearScore();	

	std::vector<int> winners;
	for (int i = 0; i < scores_.size(); i++)
	{
		// スコアがクリアライン以上のプレイヤーを探す
		if (scores_[i] >= clearLine)
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

const int ScoreManager::GetNowWinnerPlayerIndex() const
{
	// 配列が空の場合
	if (scores_.empty())
	{
		return -1;
	}
	
	// 最大スコアとそのインデックス
	int max = scores_[0];			// 最大値
	int maxIndex = 0;				// 最大値のインデックス

	// 配列を走査して最大値とその出現回数を調べる
	for (int i = 1; i < scores_.size(); i++)
	{
		if (scores_[i] > max)
		{
			max = scores_[i];		// 最大スコア更新
			maxIndex = i;			// インデックス更新
		}
	}

	return maxIndex;
}

void ScoreManager::ResetIsBonusScores()
{
	for (auto& scoreMap : isBonusScoreTypes_)
	{
		for (auto& [scoreType, flag] : scoreMap)
		{
			flag = false; // 各スコアタイプのフラグを false に設定
		}
	}
}

const bool ScoreManager::IsDraw() const
{
	// 配列が空の場合
	if (scores_.empty())
	{
		return false;
	}

	// 最大スコアとそのインデックス
	int max = scores_[0];			// 最大値
	int maxIndex = 0;				// 最大値のインデックス
	int count = 1;					// 最大値の個数（最初の1個）

	// 配列を走査して最大値とその出現回数を調べる
	for (int i = 1; i < scores_.size(); i++)
	{
		if (scores_[i] > max)
		{
			max = scores_[i];		// 最大スコア更新
			maxIndex = i;			// インデックス更新
			count = 1;				// 新しい最大値が見つかったのでカウントをリセット
		}
		else if (scores_[i] == max)
		{
			count++;				// 同じ最大値があればカウント追加
		}
	}

	// 最大値が複数存在するなら true、それ以外はfalseを返す
	return (count > 1) ? true : false;
}

ScoreManager::ScoreManager()
{
	scores_.clear();
}

int ScoreManager::GetScoreByRank(const int _rank)
{
	//1位は2点
	if (_rank == 0) { return SCORE_TYPE_VALUES[static_cast<int>(SCORE_TYPE::FIRST)]; }
	//それ以外でゴールした人は1点
	else { return SCORE_TYPE_VALUES[static_cast<int>(SCORE_TYPE::CLEAR)]; }
}
