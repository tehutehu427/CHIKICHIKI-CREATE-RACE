#pragma once

class Player;
class Coin;

class CoinFollower
{
public:

	//コンストラクタ
	CoinFollower(Coin& _coin, const Player& _follower);
	
	//デストラクタ
	~CoinFollower(void);

	//更新
	void Update(void);

private:

	Coin& coin_;				//コイン
	const Player& follower_;	//所持者
};

