#pragma once
#include <vector>
#include <unordered_map>

class ScoreManager
{
public:

	/// <summary>
	/// 種類
	/// </summary>
	enum class SCORE_TYPE
	{
		DEATH,		//死亡
		CLEAR,		//クリア
		FIRST,		//一番乗り
		COIN,		//コイン獲得	
		KILL,		//キルボーナス	
		MAX
	};

	//種類ごとに得点
	static constexpr int SCORE_TYPE_VALUES[static_cast<int>(SCORE_TYPE::MAX)] = {
		0,	//DEATH
		1,	//CLEAR
		1,	//FIRST
		1,	//COIN
		1,	//KILL
	};

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ScoreManager();
	 
	/// <summary>
	/// 明示的にインスタンス生成する
	/// </summary>
	/// <param name=""></param>
	static void CreateInstance(void);

	/// <summary>
	/// 静的インスタンスの取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>インスタンス
	static ScoreManager& GetInstance(void);
	 
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 解放
	/// </summary>
	void Destroy();
	 
	/// <summary>
	/// スコアの追加
	/// </summary>
	/// <param name="_playerIndex">プレイヤー番号</param>
	/// <param name="_type">種類</param>
	void AddScore(const int _playerIndex, SCORE_TYPE _type);

	/// <summary>
	/// プレイヤーらのスコアを設定
	/// </summary>
	void SetPlayersScore();

	/// <summary>
	/// 勝者のプレイヤー番号を取得
	/// </summary>
	/// <param name="_clearLine">クリア値</param>
	/// <returns>プレイヤー番号、もしいなければ-1</returns>
	const int GetWinnerPlayerIndex() const;

	/// <summary>
	/// 現在のスコアが高いプレイヤー番号を返す
	/// </summary>
	/// <returns>勝者のインデックスを返し、引き分けの場合-1を返す</returns>
	const int GetNowWinnerPlayerIndex() const;

	/// <summary>
	/// スコアを返す
	/// </summary>
	/// <param name="_playerIndex">プレイヤー番号</param>
	/// <returns>指定したプレイヤーのスコア値</returns>
	const int GetScore(const int _playerIndex) const { return scores_[_playerIndex]; }

	/// <summary>
	/// ボーナスを獲得してるか返す
	/// </summary>
	/// <param name="_playerIndex">プレイヤー番号</param>
	/// <param name="_type">スコアの種類</param>
	/// <returns>獲得してたらtrue,してなければfalse</returns>
	const bool GetIsBonus(const int _playerIndex, const SCORE_TYPE _type) { return isBonusScoreTypes_[_playerIndex][_type]; }

	/// <summary>
	/// リセット
	/// </summary>
	void ResetIsBonusScores();

	/// <summary>
	/// 引き分けか返す
	/// </summary>
	/// <returns>trueの場合引き分け</returns>
	const bool IsDraw()const;

private:

	//静的インスタンス
	static ScoreManager* instance_;	
	
	// コンストラクタ
	ScoreManager();

	// デフォルトコンストラクタは使用不可
	ScoreManager(const ScoreManager&) = delete;

	// コピー代入演算子は使用不可
	ScoreManager& operator=(const ScoreManager&) = delete;

	//ランクに応じたスコアを取得
	int GetScoreByRank(const int _rank);

	//スコア
	std::vector<int> scores_;

	//ボーナスフラッグ
	std::vector<std::unordered_map<SCORE_TYPE, bool>> isBonusScoreTypes_;

};

