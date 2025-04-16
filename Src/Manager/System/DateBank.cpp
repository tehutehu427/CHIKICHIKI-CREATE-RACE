#include "InputManager.h"
#include "DateBank.h"

DateBank* DateBank::instance_ = nullptr;

void DateBank::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new DateBank();
	}
	instance_->Init();
}

DateBank& DateBank::GetInstance(void)
{
    return *instance_;
}

void DateBank::Init(void)
{
	playerNum_ = 0;
	maxPlayerNum_ = GetJoypadNum();
}

bool DateBank::SetPlayerNum(int num)
{
	maxPlayerNum_ = GetJoypadNum();
	if (num > maxPlayerNum_)
	{
		playerNum_ = num;
		return true;
	}
	return false;
}

int DateBank::GetPlayerNum(void)
{
	return playerNum_;
}

DateBank::DateBank(void)
{
}
