#pragma once
#include <string>
#include "ItemBase.h"

class ItemName
{
public:

	ItemName();
	~ItemName() = default;

	void Init();

	//–¼‘O‚ð•Ô‚·
	inline const std::string GetItemName(const ItemBase::ITEM_TYPE _type)const { return names_[static_cast<int>(_type)]; }

private:

	//Ží—Þ‚²‚Æ‚Ì–¼‘O
	std::string names_[static_cast<int>(ItemBase::ITEM_TYPE::MAX)];

};

