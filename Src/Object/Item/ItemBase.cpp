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

void ItemBase::Init(IntVector3 _mapPos)
{
	//transform_.pos = _mapPos;
	Init();
}

void ItemBase::Init()
{
}
