#include "../Object/Item/Floor.h"
#include "../Object/Item/MoveHoriFloor.h"
#include "../Object/Item/MoveVerFloor.h"
#include "../Object/Item/Fence.h"
#include "../Object/Item/Cannon.h"
#include "../Object/Item/SlimeFloor.h"
#include "MapEditer.h"
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
		if (item.second == nullptr)
		{
			continue;
		}
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
		MV1SetOpacityRate(item.second->GetTransform().modelId, DUMMY_ITEM_OPACITY_RATE);
		item.second->Draw();
		MV1SetOpacityRate(item.second->GetTransform().modelId, DEFAULT_OPACITY_RATE);
	}
}

void ItemManager::AddItem(IntVector3 mapPos, Quaternion rot, ItemBase::ITEM_TYPE type)
{
	//アイテム
	std::shared_ptr<ItemBase> item;
	item = CreateItem(type, mapPos, rot);

	//配列に追加
	items_[type].emplace_back(std::move(item));
}

void ItemManager::DeleteItem(VECTOR mapPos, int range)
{

}

void ItemManager::AllDeleteItem()
{
	items_.clear();
}

void ItemManager::CreateDummyItem(IntVector3 mapPos, Quaternion rot, ItemBase::ITEM_TYPE type,int playerNum)
{
	//アイテム
	std::shared_ptr<ItemBase> item;

	item = CreateItem(type, mapPos, rot);
	//配列に追加
	dummyItems_[playerNum] = item;
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

Transform ItemManager::GetDummyItemTransform(int playerNum)
{
	Transform transform;
	if (dummyItems_.find(playerNum) != dummyItems_.end())
	{
		transform = dummyItems_[playerNum]->GetTransform();
	}
	else
	{
		transform = Transform();
	}
	return transform;
}

void ItemManager::ResetDummyItem(int playerNum, ItemBase::ITEM_TYPE type,IntVector3 mapPos)
{
	if (dummyItems_.find(playerNum) != dummyItems_.end())
	{
		Transform transform = dummyItems_[playerNum]->GetTransform();
		std::shared_ptr<ItemBase> dummy;
		dummy = CreateItem(type, mapPos, transform.quaRot);
		//dummy->Init(mapPos, transform.quaRot, dummyItems_[playerNum]->GetStatus().itemType);
		dummyItems_.erase(playerNum);
		dummyItems_[playerNum] = dummy;
	}
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

void ItemManager::DummyItemSetRotate(Quaternion rot, int playerNum)
{
	if (dummyItems_.find(playerNum) != dummyItems_.end())
	{
		dummyItems_[playerNum]->SetRotate(rot);
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
		AddItem(dummyItems_[playerNum]->GetInitMapPos(), dummyItems_[playerNum]->GetTransform().quaRot, dummyItems_[playerNum]->GetStatus().itemType);
		//items_[dummyItems_[playerNum]->GetStatus().itemType].emplace_back(dummyItems_[playerNum]);
		dummyItems_.erase(playerNum);
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

const std::vector<std::shared_ptr<ItemBase>>* ItemManager::GetItems(const ItemBase::ITEM_TYPE _type) const
{
	auto it = items_.find(_type);
	if (it != items_.end()) 
	{
		return &it->second;
	}
	return nullptr;
}

void ItemManager::ItemsAddDummyItems(ItemBase::ITEM_TYPE _type, IntVector3 _mapPos,int playerNum)
{
	auto it = items_.find(_type);
	if (it == items_.end())
	{
		return;
	}
	for (auto& item : it->second)
	{
		if (item == nullptr)
		{
			continue;
		}
		for (int i = 0; i < item->GetSize().x; i++)
		{
			for (int j = 0; j < item->GetSize().y; j++)
			{
				for (int k = 0; k < item->GetSize().z; k++)
				{
					IntVector3 mapPos = { item->GetInitMapPos().x + i,item->GetInitMapPos().y + j,item->GetInitMapPos().z + k };
					if (mapPos != _mapPos)
					{
						continue;
					}
					if (dummyItems_.find(playerNum) != dummyItems_.end())
					{
						DummyItemAddItems(playerNum);
					}
					dummyItems_[playerNum] = CreateItem(_type, _mapPos, item->GetTransform().quaRot);
					item = nullptr;
					return;
				}
			}
		}
	}
}

ItemManager::ItemManager(void)
{

}

std::shared_ptr<ItemBase> ItemManager::CreateItem(ItemBase::ITEM_TYPE type, IntVector3 mapPos, Quaternion rot)
{
	std::shared_ptr<ItemBase> item = nullptr;
	switch (type)
	{
	case ItemBase::ITEM_TYPE::NONE:
		break;
	case ItemBase::ITEM_TYPE::START:
		break;
	case ItemBase::ITEM_TYPE::GOAL:
		break;
	case ItemBase::ITEM_TYPE::FLOOR:
		item = std::make_shared<Floor>();
		break;
	case ItemBase::ITEM_TYPE::MOVE_HORI_FLOOR:
		item = std::make_shared<MoveHoriFloor>();
		break;
	case ItemBase::ITEM_TYPE::MOVE_VER_FLOOR:
		item = std::make_shared<MoveVerFloor>();
		break;
	case ItemBase::ITEM_TYPE::FENCE:
		item = std::make_shared<Fence>();
		break;
	case ItemBase::ITEM_TYPE::CANNON:
		item = std::make_shared<Cannon>();
		break;
	case ItemBase::ITEM_TYPE::SPIKY:
		break;
	case ItemBase::ITEM_TYPE::BOMB_SMALL:
		break;
	case ItemBase::ITEM_TYPE::BOMB_BIG:
		break;
	case ItemBase::ITEM_TYPE::MAX:
		break;
	default:
		break;
	}
	if (item == nullptr)
	{
		return nullptr;
	}
	item->Init(mapPos, rot, type);
	return item;
}
