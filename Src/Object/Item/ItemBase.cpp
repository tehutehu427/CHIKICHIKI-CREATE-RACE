#include "ItemBase.h"

ItemBase::ItemBase()
{
	status_.isBreak = false;
	status_.isGravity = false;
	status_.itemType = ITEM_TYPE::NONE;
	status_.effType = EFFECT_TYPE::FIXED;
}

ItemBase::~ItemBase()
{
}
