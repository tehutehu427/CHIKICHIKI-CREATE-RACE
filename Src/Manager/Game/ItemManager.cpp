#include "../Object/Item/Floor.h"
#include "../Object/Item/MoveHoriFloor.h"
#include "../Object/Item/MoveVerFloor.h"
#include "../Object/Item/Fence.h"
#include "../Object/Item/Cannon.h"
#include "ItemManager.h"


ItemManager* ItemManager::instance_ = nullptr;

void ItemManager::Init(void)
{

}

void ItemManager::Update(void)
{
	for (auto& [type, itemList] : items_) {
		for (auto& item : itemList) {
			if (item) {
				item->Update(); 
			}
		}
	}
	for (auto& item : dummyItems_)
	{
		item.second->Update();
	}
}

void ItemManager::Draw(void)
{
	for (auto& [type, itemList] : items_) {
		for (auto& item : itemList) {
			if (item) {
				item->Draw();
			}
		}
	}
	for (auto& item : dummyItems_)
	{
		if (item.second == nullptr)
		{
			continue;
		}
		item.second->Draw();
	}
}

void ItemManager::AddItem(IntVector3 mapPos, Quaternion rot, ItemBase::ITEM_TYPE type)
{
	//アイテム
	std::unique_ptr<ItemBase> item;

	//生成
	switch (type)
	{
	case ItemBase::ITEM_TYPE::START:
		break;
	case ItemBase::ITEM_TYPE::GOAL:
		break;
	case ItemBase::ITEM_TYPE::FLOOR:
		item = std::make_unique<Floor>();
		break;
	case ItemBase::ITEM_TYPE::MOVE_HORI_FLOOR:
		item = std::make_unique<MoveHoriFloor>();
		break;
	case ItemBase::ITEM_TYPE::MOVE_VER_FLOOR:
		item = std::make_unique<MoveVerFloor>();
		break;
	case ItemBase::ITEM_TYPE::FENCE:
		item = std::make_unique<Fence>();
		break;
	case ItemBase::ITEM_TYPE::CANNON:
		item = std::make_unique<Cannon>();
		break;
	case ItemBase::ITEM_TYPE::SPIKY:
		break;
	case ItemBase::ITEM_TYPE::BOMB_SMALL:
		break;
	case ItemBase::ITEM_TYPE::BOMB_BIG:
		break;
	}

	//初期化
	item->Init(mapPos,rot, type);

	//配列に追加
	items_[type].emplace_back(std::move(item));
}

void ItemManager::DeleteItem(VECTOR mapPos, int range)
{

}

void ItemManager::CreateDummyItem(IntVector3 mapPos, Quaternion rot, ItemBase::ITEM_TYPE type,int playerNum)
{
	//アイテム
	std::unique_ptr<ItemBase> item;

	//生成
	switch (type)
	{
	case ItemBase::ITEM_TYPE::START:
		break;
	case ItemBase::ITEM_TYPE::GOAL:
		break;
	case ItemBase::ITEM_TYPE::FLOOR:
		item = std::make_unique<Floor>();
		break;
	case ItemBase::ITEM_TYPE::MOVE_HORI_FLOOR:
		item = std::make_unique<MoveHoriFloor>();
		break;
	case ItemBase::ITEM_TYPE::MOVE_VER_FLOOR:
		item = std::make_unique<MoveVerFloor>();
		break;
	case ItemBase::ITEM_TYPE::FENCE:
		item = std::make_unique<Fence>();
		break;
	case ItemBase::ITEM_TYPE::CANNON:
		item = std::make_unique<Cannon>();
		break;
	case ItemBase::ITEM_TYPE::SPIKY:
		break;
	case ItemBase::ITEM_TYPE::BOMB_SMALL:
		break;
	case ItemBase::ITEM_TYPE::BOMB_BIG:
		break;
	}

	//初期化
	item->Init(mapPos, rot, type);

	//配列に追加
	dummyItems_[playerNum] = std::move(item);
}

ItemBase::Status ItemManager::GetDummyItemStatus(int playerNum)
{
	ItemBase::Status status;
	if (dummyItems_.find(playerNum) != dummyItems_.end())
	{
		status = dummyItems_[playerNum]->GetStatus();
	}
	else
	{
		status = ItemBase::Status();
	}
	return status;
}

IntVector3 ItemManager::GetDummyObjectSize(int playerNum)
{
	IntVector3 size;
	if (dummyItems_.find(playerNum) != dummyItems_.end())
	{
		size = dummyItems_[playerNum]->GetSize();
	}
	else
	{
		size = {-1,-1,-1};
	}
	return size;
}

void ItemManager::DummyItemSetMapPos(IntVector3 mapPos, int playerNum)
{
	if (dummyItems_.find(playerNum) != dummyItems_.end())
	{
		dummyItems_[playerNum]->SetPos(mapPos);
	}
	else
	{
		return;
	}
}

void ItemManager::DummyItemAddItems(int playerNum)
{
	if (dummyItems_.find(playerNum) != dummyItems_.end())
	{
		items_.emplace_back(std::move(dummyItems_[playerNum]));
		dummyItems_.erase(playerNum);
	}
	else
	{
		return;
	}
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

const std::vector<std::unique_ptr<ItemBase>>* ItemManager::GetItems(const ItemBase::ITEM_TYPE _type) const
{
	auto it = items_.find(_type);
	if (it != items_.end()) 
	{
		return &it->second;
	}
	return nullptr;
}

ItemManager::ItemManager(void)
{

}
