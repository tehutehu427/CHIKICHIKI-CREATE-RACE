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
}

void EventManager::Update()
{
}

void EventManager::Draw()
{
}

void EventManager::SetEventType(const EVENT_TYPE _type)
{
	eventType_ = _type;
	eventFunc_[eventType_]();
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

	//プレイヤー全て強化させる
	for (auto& player : players)
	{
		player->Init();
	}

	//ポストエフェクトを戻す
	PostEffectManager::GetInstance().ChangeEffectType(PostEffectManager::TYPE::NONE);

	//イベントを戻す
	SetEventType(EVENT_TYPE::NONE);
}

void EventManager::RegisterSet(const EVENT_TYPE _type, std::function<void(void)> _func)
{
	eventFunc_[_type] = _func;
}

void EventManager::SetRespawn(void)
{

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

EventManager::EventManager(void)
{
	eventType_ = EVENT_TYPE::NONE;

	//処理の登録
	RegisterSet(EVENT_TYPE::NONE, [&]() { SetNone(); });
	RegisterSet(EVENT_TYPE::RESPAWN, [&]() { SetRespawn(); });
	RegisterSet(EVENT_TYPE::WIGGLE, [&]() { SetWiggle(); });
	RegisterSet(EVENT_TYPE::STATE_UP, [&]() { SetStateUp(); });
	RegisterSet(EVENT_TYPE::THREE_POINT, [&]() { SetThreePoint(); });
	RegisterSet(EVENT_TYPE::FISH_EYE, [&]() { SetFishEye(); });
	RegisterSet(EVENT_TYPE::SAND_STORM, [&]() { SetSandstorm(); });
}

EventManager::~EventManager(void)
{
}
