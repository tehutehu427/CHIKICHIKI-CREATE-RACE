#pragma once
#include <vector>

class ScoreManager
{
public:

	/// <summary>
	/// 種類
	/// </summary>
	enum class SCORE_TYPE
	{
		DEATH,	//死亡
		CLEAR,	//クリア
		FIRST,	//一番乗り
		BONUS_ITEM,	//ボーナスアイテム	
		MAX
	};

	//種類ごとに得点
	static constexpr int SCORE_TYPE_VALUES[static_cast<int>(SCORE_TYPE::MAX)] = {
		0,	//DEATH
		1,	//CLEAR
		2,	//FIRST
		1,	//BONUS_ITEM
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
	/// 勝者のプレイヤー番号を取得
	/// </summary>
	/// <param name="_clearLine">クリア値</param>
	/// <returns>プレイヤー番号、もしいなければ-1</returns>
	const int GetWinnerPlayerIndex(const int _clearLine) const;

	/// <summary>
	/// スコアを返す
	/// </summary>
	/// <param name="_playerIndex">プレイヤー番号</param>
	/// <returns>指定したプレイヤーのスコア値</returns>
	inline const int GetScore(const int _playerIndex) const { return scores_[_playerIndex]; }

private:

	//静的インスタンス
	static ScoreManager* instance_;	
	
	// コンストラクタ
	ScoreManager();

	// デフォルトコンストラクタは使用不可
	ScoreManager(const ScoreManager&) = delete;

	// コピー代入演算子は使用不可
	ScoreManager& operator=(const ScoreManager&) = delete;

	//スコア
	std::vector<int> scores_;

};

