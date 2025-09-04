#include "EventManager.h"
#include "PlayerManager.h"
#include "PostEffectManager.h"

EventManager* EventManager::instance_ = nullptr;

void EventManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new EventManager();
	}
	instance_->Init();
}

EventManager& EventManager::GetInstance(void)
{
	return *instance_;
}

void EventManager::Destroy(void)
{
	//自身のインスタンス削除
	delete instance_;
	instance_ = nullptr;
}

void EventManager::Init()
{
	SetEventType(EVENT_TYPE::NONE);
	isStateUp_ = false;
}

void EventManager::Update()
{
	eventUpdateMap_[eventType_]();
}

void EventManager::Draw()
{
	if (eventType_ == EVENT_TYPE::NONE)
	{
		return;
	}

	DrawString(20, 20, "イベント発生！！", 0xff0000);
}

void EventManager::RegisterSet(const EVENT_TYPE _type, std::function<void(void)> _setter, std::function<void(void)> _update)
{
	eventSetterMap_[_type] = _setter;
	eventUpdateMap_[_type] = _update;
}

void EventManager::SetEventType(const EVENT_TYPE _type)
{
	eventType_ = _type;
	eventSetterMap_[eventType_]();
}

void EventManager::SetRandomEventByRound()
{
	//ランダムで値取得
	int index = GetRand(EVENT_PER);

	//特定値以外はイベントを実行しない
	if (index != 0)
	{
		return;
	}

	//ランダムで値取得
	index = GetRand(static_cast<int>(EVENT_TYPE::MAX) - 1);

	//設定
	SetEventType(static_cast<EVENT_TYPE>(index));
}

void EventManager::Reset()
{
	if (eventType_ == EVENT_TYPE::NONE)
	{
		return;
	}

	//プレイヤーのステータスを戻す
	auto& players = PlayerManager::GetInstance().GetPlayers();

	//プレイヤー戻す
	for (auto& player : players)
	{
		player->SetRespawnCnt(0);
	}

	//教科用判定を戻す
	isStateUp_ = false;

	//ポストエフェクトを戻す
	PostEffectManager::GetInstance().ChangeEffectType(PostEffectManager::TYPE::NONE);

	//イベントを戻す
	SetEventType(EVENT_TYPE::NONE);
}

void EventManager::SetRespawn(void)
{
	auto& players = PlayerManager::GetInstance().GetPlayers();

	//リスポーン値を設定する
	for (auto& player : players)
	{
		player->SetRespawnCnt(Player::RESPAWN_CNT);
	}
}

void EventManager::SetThreePoint(void)
{
}

void EventManager::SetStateUp(void)
{
	auto& players = PlayerManager::GetInstance().GetPlayers();

	//プレイヤー全て強化させる
	for (auto& player : players)
	{
		player->SetPunchPow(Player::BUFF_KNOCKBACK_CNT_MAX, Player::BUFF_KNOCKBACK_SPEED_MAX);
		player->SetJumpDecelMax(Player::BUFF_JUMP_POW_MAX);
		player->SetSpeed(Player::BUFF_MOVE_SPEED, Player::BUFF_DASH_SPEED);
	}

}

void EventManager::SetSandstorm(void)
{
	PostEffectManager::GetInstance().ChangeEffectType(PostEffectManager::TYPE::SANDSTORM);
}

void EventManager::SetWiggle(void)
{
	PostEffectManager::GetInstance().ChangeEffectType(PostEffectManager::TYPE::WIGGLE);
}

void EventManager::SetFishEye(void)
{
	PostEffectManager::GetInstance().ChangeEffectType(PostEffectManager::TYPE::FISH_EYE);
}

void EventManager::UpdateStateUp(void)
{
	if (!isStateUp_)
	{
		auto& players = PlayerManager::GetInstance().GetPlayers();

		//プレイヤー全て強化させる
		for (auto& player : players)
		{
			player->SetPunchPow(Player::BUFF_KNOCKBACK_CNT_MAX, Player::BUFF_KNOCKBACK_SPEED_MAX);
			player->SetJumpDecelMax(Player::BUFF_JUMP_POW_MAX);
			player->SetSpeed(Player::BUFF_MOVE_SPEED, Player::BUFF_DASH_SPEED);
		}

		isStateUp_ = true;
	}
}

void EventManager::UpdateSandstorm(void)
{
}

EventManager::EventManager(void)
{
	eventType_ = EVENT_TYPE::NONE;

	//処理の登録
	RegisterSet(EVENT_TYPE::NONE, [&]() { SetNone(); }, [&]() { UpdateNone(); });
	RegisterSet(EVENT_TYPE::RESPAWN, [&]() { SetRespawn(); }, [&]() { UpdateNone(); });
	RegisterSet(EVENT_TYPE::WIGGLE, [&]() { SetWiggle(); }, [&]() { UpdateNone(); });
	RegisterSet(EVENT_TYPE::STATE_UP, [&]() { SetStateUp(); }, [&]() { UpdateStateUp(); });
	RegisterSet(EVENT_TYPE::THREE_POINT, [&]() { SetThreePoint(); }, [&]() { UpdateNone(); });
	RegisterSet(EVENT_TYPE::FISH_EYE, [&]() { SetFishEye(); }, [&]() { UpdateNone(); });
	RegisterSet(EVENT_TYPE::SAND_STORM, [&]() { SetSandstorm(); }, [&]() { UpdateSandstorm(); });
}

EventManager::~EventManager(void)
{
}
