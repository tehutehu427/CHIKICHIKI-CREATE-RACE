#include "EventManager.h"
#include "PlayerManager.h"

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

void EventManager::RegisterSet(const EVENT_TYPE _type, std::function<void(void)> _func)
{
	eventFunc_[_type] = _func;
}

void EventManager::SetNone(void)
{
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

EventManager::EventManager(void)
{
	eventType_ = EVENT_TYPE::NONE;

	//処理の登録
	RegisterSet(EVENT_TYPE::NONE, [&]() { SetNone(); });
	RegisterSet(EVENT_TYPE::RESPAWN, [&]() { SetRespawn(); });
	RegisterSet(EVENT_TYPE::WIGGLE, [&]() { SetNone(); });
	RegisterSet(EVENT_TYPE::STATE_UP, [&]() { SetStateUp(); });
	RegisterSet(EVENT_TYPE::THREE_POINT, [&]() { SetThreePoint(); });
}

EventManager::~EventManager(void)
{
}
