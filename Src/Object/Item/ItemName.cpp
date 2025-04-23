#include "ItemName.h"

ItemName::ItemName()
{
	for (std::string& n : names_) { n = ""; }
}

void ItemName::Init()
{
	//ñºëOÇÃäiî[
	names_[static_cast<int>(ItemBase::ITEM_TYPE::NONE)] = "";
	names_[static_cast<int>(ItemBase::ITEM_TYPE::BOMB_BIG)] = "îöíeëÂ";
	names_[static_cast<int>(ItemBase::ITEM_TYPE::BOMB_SMALL)] = "îöíeè¨";
	names_[static_cast<int>(ItemBase::ITEM_TYPE::CANNON)] = "ëÂñC";
	names_[static_cast<int>(ItemBase::ITEM_TYPE::FENCE)] = "çÚ";
	names_[static_cast<int>(ItemBase::ITEM_TYPE::FLOOR)] = "è∞";
	names_[static_cast<int>(ItemBase::ITEM_TYPE::GOAL)] = "ÉSÅ[Éã";
	names_[static_cast<int>(ItemBase::ITEM_TYPE::MOVE_HORI_FLOOR)] = "â°à⁄ìÆè∞";
	names_[static_cast<int>(ItemBase::ITEM_TYPE::MOVE_VER_FLOOR)] = "ècà⁄ìÆè∞";
	names_[static_cast<int>(ItemBase::ITEM_TYPE::SPIKY)] = "Ç∆Ç∞è∞";
	names_[static_cast<int>(ItemBase::ITEM_TYPE::START)] = "ÉXÉ^Å[Ég";
}
