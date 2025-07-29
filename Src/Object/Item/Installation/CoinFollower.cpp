#include "Coin.h"
#include "../../Player/Player.h"
#include "CoinFollower.h"

CoinFollower::CoinFollower(Coin& _coin, const Player& _follower) : coin_(_coin), follower_(_follower)
{
	isEnd_ = false;
}

CoinFollower::~CoinFollower(void)
{
}

void CoinFollower::Update(void)
{
	//所持者が死んだなら
	if (follower_.GetIsDeathMoment())
	{
		//フォロワーを外す
		isEnd_ = true;
	}

	//所持者がゴールしたなら
	if (follower_.GetIsGoalMoment())
	{
		//コインの役割終了
		coin_.End();
	}
}
