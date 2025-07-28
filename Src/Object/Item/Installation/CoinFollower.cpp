#include "Coin.h"
#include "../../Player/Player.h"
#include "CoinFollower.h"

CoinFollower::CoinFollower(Coin& _coin, const Player& _follower) : coin_(_coin), follower_(_follower)
{
}

CoinFollower::~CoinFollower(void)
{
}

void CoinFollower::Update(void)
{
	//所持者が死んだなら
	if(follower_.)

	//所持者がゴールしたなら
	if (follower_.IsGoal())
	{
		//コインの役割終了
		coin_.End();
	}
}
