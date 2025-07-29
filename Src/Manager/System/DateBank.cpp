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
	stageNo_ = 0;
	multiClearScore_ = DEFAULT_SCORE;
	isItemSetSkip_ = true;

	//–¼‘O‚ÌŠi”[
	itemNames_[static_cast<int>(ItemBase::ITEM_TYPE::NONE)] = "";
	itemNames_[static_cast<int>(ItemBase::ITEM_TYPE::BOMB_BIG)] = "BombBig";
	itemNames_[static_cast<int>(ItemBase::ITEM_TYPE::BOMB_SMALL)] = "BombSmall";
	itemNames_[static_cast<int>(ItemBase::ITEM_TYPE::CANNON)] = "Cannon";
	itemNames_[static_cast<int>(ItemBase::ITEM_TYPE::FENCE)] = "Fence";
	itemNames_[static_cast<int>(ItemBase::ITEM_TYPE::FLOOR)] = "Floor";
	itemNames_[static_cast<int>(ItemBase::ITEM_TYPE::GOAL)] = "Goal";
	itemNames_[static_cast<int>(ItemBase::ITEM_TYPE::MOVE_HORI_FLOOR)] = "LateralMoveFloor";
	itemNames_[static_cast<int>(ItemBase::ITEM_TYPE::MOVE_VER_FLOOR)] = "VerticalMoveFloor";
	itemNames_[static_cast<int>(ItemBase::ITEM_TYPE::SPIKY)] = "Spiky";
	itemNames_[static_cast<int>(ItemBase::ITEM_TYPE::START)] = "Start";
	itemNames_[static_cast<int>(ItemBase::ITEM_TYPE::FAN)] = "Fan";
	itemNames_[static_cast<int>(ItemBase::ITEM_TYPE::SLIME_FLOOR)] = "SlimFloor";
	itemNames_[static_cast<int>(ItemBase::ITEM_TYPE::SPRING)] = "Spring";
	itemNames_[static_cast<int>(ItemBase::ITEM_TYPE::COIN)] = "Coin";
}

void DateBank::Destroy(void)
{
	if (instance_ != nullptr)
	{
		delete instance_;
		instance_ = nullptr;
	}
}

bool DateBank::SetPlayerNum(int _num)
{
	//maxPlayerNum_ = GetJoypadNum();
	//if (num <= maxPlayerNum_)
	//{
		playerNum_ = _num;
		return true;
	//}
	return false;
}

int DateBank::GetPlayerNum(void)
{
	return playerNum_;
}

DateBank::TYPE DateBank::GetType(void)
{
	TYPE type;
	if (GetPlayerNum() == 1)
	{
		type = TYPE::KEY_BORD;
	}
	else if (GetPlayerNum() == 0)
	{
		type = TYPE::ERORE;
	}
	else
	{
		type = TYPE::CONTROLLER;
	}
	return type;
}

DateBank::DateBank(void)
{
	maxPlayerNum_ = 0;
	playerNum_ = 0;
	for (std::string& n : itemNames_) { n = ""; }
}

DateBank::~DateBank(void)
{
}
