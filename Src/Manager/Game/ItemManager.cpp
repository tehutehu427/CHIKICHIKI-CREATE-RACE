#include "../Object/Item/Floor.h"
#include "../Object/Item/MoveHoriFloor.h"
#include "ItemManager.h"


ItemManager* ItemManager::instance_ = nullptr;

void ItemManager::Init(void)
{

}

void ItemManager::Update(void)
{
	for (int i = 0; i < itemNum_; i++)
	{
		items_[i]->Update();
	}
}

void ItemManager::Draw(void)
{
	for (int i = 0; i < itemNum_; i++)
	{
		items_[i]->Draw();
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
	case ItemBase::ITEM_TYPE::MOVE_VER_FLOOT:
		break;
	case ItemBase::ITEM_TYPE::FENCE:
		break;
	case ItemBase::ITEM_TYPE::CANNON:
		break;
	case ItemBase::ITEM_TYPE::SPIKY:
		break;
	case ItemBase::ITEM_TYPE::BOMB_SMALL:
		break;
	case ItemBase::ITEM_TYPE::BOMB_BIG:
		break;
	}

	//初期化
	item->Init(mapPos,rot);

	//配列に追加
	items_.emplace(itemNum_, std::move(item));

	//最後にアイテムのカウンタを増やす
	itemNum_++;
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
