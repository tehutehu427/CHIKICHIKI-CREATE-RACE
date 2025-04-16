#include "ItemManager.h"


ItemManager* ItemManager::instance_ = nullptr;

void ItemManager::Init(void)
{

}

void ItemManager::AddItem(IntVector3 mapPos, Quaternion rot, ItemBase::ITEM_TYPE type, int itemNom)
{
}

void ItemManager::DeleteItem(VECTOR mapPos, int range)
{
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
}
