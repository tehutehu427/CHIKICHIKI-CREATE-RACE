#include "ItemManager.h"


ItemManager* ItemManager::instance_ = nullptr;

void ItemManager::Init(void)
{

}

void ItemManager::Update(void)
{
}

void ItemManager::Draw(void)
{
}

void ItemManager::AddItem(IntVector3 mapPos, Quaternion rot, ItemBase::ITEM_TYPE type)
{
	//ItemBase*

	//最後にアイテムのカウンタを増やす
	//itemNum_++;
}

void ItemManager::DeleteItem(VECTOR mapPos, int range)
{
	//最後にアイテムのカウンタを減らす
	itemNum_--;
}

void ItemManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ItemManager();
	}
	instance_->Init();
}

ItemManager& ItemManager::GetInstance(void)
{
	return *instance_;
}

ItemManager::ItemManager(void)
{
	itemNum_ = 0;
}
