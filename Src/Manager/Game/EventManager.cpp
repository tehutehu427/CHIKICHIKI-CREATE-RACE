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

void EventManager::Reset()
{
	if (eventType_ == EVENT_TYPE::NONE)
	{
		return;
	}

	//プレイヤーのステータスを戻す


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
