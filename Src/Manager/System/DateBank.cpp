#include "InputManager.h"
#include "DateBank.h"
#include "../../Object/Item/ItemName.h"

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

	//ñºëOÇÃäiî[
	itemNames_[static_cast<int>(ItemBase::ITEM_TYPE::NONE)] = "";
	itemNames_[static_cast<int>(ItemBase::ITEM_TYPE::BOMB_BIG)] = "îöíeëÂ";
	itemNames_[static_cast<int>(ItemBase::ITEM_TYPE::BOMB_SMALL)] = "îöíeè¨";
	itemNames_[static_cast<int>(ItemBase::ITEM_TYPE::CANNON)] = "ëÂñC";
	itemNames_[static_cast<int>(ItemBase::ITEM_TYPE::FENCE)] = "çÚ";
	itemNames_[static_cast<int>(ItemBase::ITEM_TYPE::FLOOR)] = "è∞";
	itemNames_[static_cast<int>(ItemBase::ITEM_TYPE::GOAL)] = "ÉSÅ[Éã";
	itemNames_[static_cast<int>(ItemBase::ITEM_TYPE::MOVE_HORI_FLOOR)] = "â°à⁄ìÆè∞";
	itemNames_[static_cast<int>(ItemBase::ITEM_TYPE::MOVE_VER_FLOOR)] = "ècà⁄ìÆè∞";
	itemNames_[static_cast<int>(ItemBase::ITEM_TYPE::SPIKY)] = "Ç∆Ç∞è∞";
	itemNames_[static_cast<int>(ItemBase::ITEM_TYPE::START)] = "ÉXÉ^Å[Ég";
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

DateBank::TYPE DateBank::GetType(void)
{
	TYPE type;
	if (GetPlayerNum() == 1)
	{
		type = TYPE::KEY_BORD;
	}
	if (GetPlayerNum() == 0)
	{
		type = TYPE::ERORE;
	}
	type = TYPE::CONTROLLER;

	return type;
}

DateBank::DateBank(void)
{
	maxPlayerNum_ = 0;
	playerNum_ = 0;
	for (std::string& n : itemNames_) { n = ""; }
}
