#include "../Object/Item/Fixed/StartFlag.h"
#include "../Object/Item/Fixed/GoalFlag.h"
#include "../Object/Item/Installation/Floor.h"
#include "../Object/Item/Installation/MoveHoriFloor.h"
#include "../Object/Item/Installation/MoveVerFloor.h"
#include "../Object/Item/Installation/Fence.h"
#include "../Object/Item/Installation/Cannon.h"
#include "../Object/Item/Installation/SlimeFloor.h"
#include "../Object/Item/Installation/Fan.h"
#include "../Object/Item/Installation/Spiky.h"
#include "../Object/Item/Destroyer/SmallBomb.h"
#include "../Object/Item/Destroyer/BigBomb.h"
#include "MapEditer.h"
#include "ItemManager.h"


ItemManager* ItemManager::instance_ = nullptr;

void ItemManager::Init(void)
{

}

void ItemManager::Update(void)
{
	//各アイテムの更新
	for (auto& [type, itemList] : items_) {
		for (auto& item : itemList) {
			if (item) {
				item->Update(); 
			}
		}
	}
	//各ダミーアイテムの更新
	for (auto& item : dummyItems_)
	{
		//まだ設置してない
		if (item.second == nullptr)
		{
			//飛ばす
			continue;
		}
		item.second->Update();
	}
}

void ItemManager::Draw(void)
{
	//各アイテムの描画
	for (auto& [type, itemList] : items_) {
		for (auto& item : itemList) {
			if (item) {
				item->Draw();
			}
		}
	}

	//各ダミーアイテムの描画
	for (auto& item : dummyItems_)
	{
		//まだ設置してない
		if (item.second == nullptr)
		{
			//飛ばす
			continue;
		}

		//他オブジェクトと重なっているか
		if (MapEditer::GetInstance().IsObjectAtMapPos(item.second->GetInitMapPos(), item.second->GetSize()))
		{
			//設置禁止表示
			MV1SetDifColorScale(item.second->GetTransform().modelId, ItemManager::DUMMY_OVERLAP_COLOR);
		}
		//重なっていない
		else
		{
			//設置可能表示
			MV1SetDifColorScale(item.second->GetTransform().modelId, ItemManager::DUMMY_DEFAULT_COLOR);
		}
		//モデル描画
		item.second->Draw();

		//元の色に戻す
		MV1SetDifColorScale(item.second->GetTransform().modelId, ItemManager::DEFAULT_COLOR);
	}
}

void ItemManager::AddItem(IntVector3 mapPos, Quaternion rot, ItemBase::ITEM_TYPE type)
{
	//アイテム
	std::shared_ptr<ItemBase> item;

	//アイテムを生成
	item = CreateItem(type, mapPos, rot);

	//配列に追加
	items_[type].emplace_back(std::move(item));
}

void ItemManager::DeleteItem(VECTOR mapPos, int range)
{

}

void ItemManager::AllDeleteItem()
{
	//アイテム全削除
	items_.clear();
}

void ItemManager::CreateDummyItem(IntVector3 mapPos, Quaternion rot, ItemBase::ITEM_TYPE type,int playerNum)
{
	//アイテム
	std::shared_ptr<ItemBase> item;

	//アイテム作成
	item = CreateItem(type, mapPos, rot);
	
	//配列に追加
	dummyItems_[playerNum] = item;
}

ItemBase::Status ItemManager::GetDummyItemStatus(int playerNum)
{
	//ステータス
	ItemBase::Status status;

	//指定プレイヤーのダミーアイテムがあるか
	if (dummyItems_.find(playerNum) != dummyItems_.end())
	{
		//見つかった
		status = dummyItems_[playerNum]->GetStatus();
	}
	else
	{
		//見つからなかった
		status = ItemBase::Status();
	}

	//ステータスを返す
	return status;
}

IntVector3 ItemManager::GetDummyItemMapPos(int playerNum)
{
	//マップ座標
	IntVector3 mapPos;

	//指定プレイヤーのダミーアイテムがあるか
	if (dummyItems_.find(playerNum) != dummyItems_.end())
	{
		//見つかった
		mapPos = dummyItems_[playerNum]->GetInitMapPos();
	}
	else
	{
		//見つからなかった
		mapPos = { -1,-1,-1 };
	}

	//マップ座標を返す
	return mapPos;
}

IntVector3 ItemManager::GetDummyItemSize(int playerNum)
{
	//アイテムの大きさ
	IntVector3 size;

	//指定プレイヤーのダミーアイテムがあるか
	if (dummyItems_.find(playerNum) != dummyItems_.end())
	{
		//見つかった
		size = dummyItems_[playerNum]->GetSize();
	}
	else
	{
		//見つからなかった
		size = {-1,-1,-1};
	}

	//大きさを返す
	return size;
}

Transform ItemManager::GetDummyItemTransform(int playerNum)
{
	//モデル情報
	Transform transform;

	//指定プレイヤーのダミーアイテムがあるか
	if (dummyItems_.find(playerNum) != dummyItems_.end())
	{
		//見つかった
		transform = dummyItems_[playerNum]->GetTransform();
	}
	else
	{
		//見つからなかった
		transform = Transform();
	}

	//モデル情報を返す
	return transform;
}

void ItemManager::ResetDummyItem(int playerNum, ItemBase::ITEM_TYPE type,IntVector3 mapPos)
{
	//指定プレイヤーのダミーアイテムがあるか
	if (dummyItems_.find(playerNum) != dummyItems_.end())
	{
		//モデルの回転情報は保存
		Transform transform = dummyItems_[playerNum]->GetTransform();
		
		//新たに指定されたアイテムを生成
		std::shared_ptr<ItemBase> dummy;
		dummy = CreateItem(type, mapPos, transform.quaRot);

		//元あったアイテムの削除
		dummyItems_.erase(playerNum);

		//置き換え
		dummyItems_[playerNum] = dummy;
	}
}

void ItemManager::DummyItemSetMapPos(IntVector3 mapPos, int playerNum)
{
	//指定プレイヤーのダミーアイテムがあるか
	if (dummyItems_.find(playerNum) != dummyItems_.end())
	{
		//見つかった
		dummyItems_[playerNum]->SetPos(mapPos);
	}
	else
	{
		//見つからなかった
		return;
	}
}

void ItemManager::DummyItemSetRotate(Quaternion rot, int playerNum)
{
	//指定プレイヤーのダミーアイテムがあるか
	if (dummyItems_.find(playerNum) != dummyItems_.end())
	{
		//見つかった
		dummyItems_[playerNum]->SetRotate(rot);
	}
	else
	{
		//見つからなかった
		return;
	}
}

void ItemManager::DummyItemAddItems(int playerNum)
{
	//指定プレイヤーのダミーアイテムがあるか
	if (dummyItems_.find(playerNum) != dummyItems_.end())
	{
		//ダミーからアイテムに追加
		AddItem(dummyItems_[playerNum]->GetInitMapPos(), dummyItems_[playerNum]->GetTransform().quaRot, dummyItems_[playerNum]->GetStatus().itemType);
		//items_[dummyItems_[playerNum]->GetStatus().itemType].emplace_back(dummyItems_[playerNum]);
		
		//ダミー内の要素を消す
		dummyItems_.erase(playerNum);
	}
}

void ItemManager::CreateInstance(void)
{
	//インスタンスがないなら
	if (instance_ == nullptr)
	{
		//インスタンス生成
		instance_ = new ItemManager();
	}

	//初期化
	instance_->Init();
}

ItemManager& ItemManager::GetInstance(void)
{
	return *instance_;
}

const std::vector<std::shared_ptr<ItemBase>>* ItemManager::GetItems(const ItemBase::ITEM_TYPE _type) const
{
	//指定アイテムが存在するか
	auto it = items_.find(_type);
	if (it != items_.end()) 
	{
		//存在した
		return &it->second;
	}

	//存在しなかった
	return nullptr;
}

void ItemManager::ItemsAddDummyItems(ItemBase::ITEM_TYPE _type, IntVector3 _mapPos,int playerNum)
{
	//アイテムが存在するか
	auto it = items_.find(_type);
	if (it == items_.end())
	{
		//存在しなかった
		return;
	}

	//指定アイテム
	for (auto& item : it->second)
	{
		//指定アイテムが存在するか
		if (item == nullptr)
		{
			//飛ばす
			continue;
		}

		//大きさ分の全マスと判定
		for (int i = 0; i < item->GetSize().x; i++)
		{
			for (int j = 0; j < item->GetSize().y; j++)
			{
				for (int k = 0; k < item->GetSize().z; k++)
				{
					//初期位置からサイズ分
					IntVector3 mapPos = { item->GetInitMapPos().x + i,item->GetInitMapPos().y + j,item->GetInitMapPos().z + k };
					
					//重なっていない
					if (mapPos != _mapPos)
					{
						//飛ばす
						continue;
					}

					//アイテムが存在するか
					if (dummyItems_.find(playerNum) != dummyItems_.end())
					{
						//ダミーからアイテムに移行
						DummyItemAddItems(playerNum);
					}

					//新たに生成
					dummyItems_[playerNum] = CreateItem(_type, _mapPos, item->GetTransform().quaRot);
					
					//元情報を削除
					item = nullptr;
					return;
				}
			}
		}
	}
}

void ItemManager::DeleteDummyItem(int playerNum)
{
	//アイテムが存在するか
	if (dummyItems_.find(playerNum) != dummyItems_.end())
	{
		//削除
		dummyItems_[playerNum] = nullptr;
	}
	else
	{
		return;
	}
}

//void ItemManager::MoveSubItemOwner(const ItemBase::ITEM_TYPE _type, std::shared_ptr<ItemBase> _subItem)
//{
//	//初期化処理
//	_subItem->Init();
//
//	//所有権明け渡し
//	items_[_type].emplace_back(_subItem);
//}

ItemManager::ItemManager(void)
{

}

ItemManager::~ItemManager(void)
{
}

std::shared_ptr<ItemBase> ItemManager::CreateItem(ItemBase::ITEM_TYPE type, IntVector3 mapPos, Quaternion rot)
{
	//アイテム
	std::shared_ptr<ItemBase> item = nullptr;
	
	//種類ごとの生成
	switch (type)
	{
	case ItemBase::ITEM_TYPE::START:
		item = std::make_shared<StartFlag>();
		break;
	case ItemBase::ITEM_TYPE::GOAL:
		item = std::make_shared<GoalFlag>();
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
		item = std::make_shared<Spiky>();
		break;
	case ItemBase::ITEM_TYPE::BOMB_SMALL:
		item = std::make_shared<SmallBomb>();
		break;
	case ItemBase::ITEM_TYPE::BOMB_BIG:
		item = std::make_shared<BigBomb>();
		break;
	default:
		break;
	}

	//アイテムが生成できていない
	if (item == nullptr)
	{
		//空アイテムを返す
		return nullptr;
	}

	//初期化
	item->Init(mapPos, rot, type);
	return item;
}
