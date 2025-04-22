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
	for (auto& item : items_)
	{
		item->Update();
	}
}

void ItemManager::Draw(void)
{
	for (auto& item : items_)
	{
		item->Draw();
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
	items_.emplace_back(std::move(item));
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
