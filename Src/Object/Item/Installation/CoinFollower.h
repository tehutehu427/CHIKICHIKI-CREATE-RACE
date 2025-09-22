#pragma once

class Player;
class Coin;

class CoinFollower
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_coin">コイン</param>
	/// <param name="_follower">追従者</param>
	CoinFollower(Coin& _coin, const Player& _follower);
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~CoinFollower(void);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name=""></param>
	void Update(void);

	/// <summary>
	/// 終了判定の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>終了判定</returns>
	const bool IsEnd(void)const { return isEnd_; }

private:

	//追従関係
	Coin& coin_;				//コイン
	const Player& follower_;	//所持者
	bool isEnd_;				//終了判定
};

