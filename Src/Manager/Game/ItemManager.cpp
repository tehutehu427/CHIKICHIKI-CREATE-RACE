#include "../Object/Item/Fixed/StartFlag.h"
#include "../Object/Item/Fixed/GoalFlag.h"
#include "../Object/Item/Installation/Floor.h"
#include "../Object/Item/Installation/MoveHoriFloor.h"
#include "../Object/Item/Installation/MoveVerFloor.h"
#include "../Object/Item/Installation/Fence.h"
#include "../Object/Item/Installation/Cannon.h"
#include "../Object/Item/Installation/Spiky.h"
#include "../Object/Item/Installation/Fan.h"
#include "../Object/Item/Installation/SlimeFloor.h"
#include "../Object/Item/Installation/Spring.h"
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
		if (MapEditer::GetInstance().IsObjectAtMapPos(item.second->GetInitMapPos(), item.second->GetSize(),item.second->GetHitSize(),GetDummyItemRotY(item.first)) < 0)
		{
			item.second->ChangeModelColor(ItemManager::DUMMY_OVERLAP_COLOR);
			item.second->SetModelColor(DUMMY_OVERLAP_COLOR.r, DUMMY_OVERLAP_COLOR.g, DUMMY_OVERLAP_COLOR.b, DUMMY_OVERLAP_COLOR.a);
		}
		//重なっていない
		else
		{
			item.second->ChangeModelColor(ItemManager::DUMMY_DEFAULT_COLOR);
			item.second->SetModelColor(DUMMY_DEFAULT_COLOR.r, DUMMY_DEFAULT_COLOR.g, DUMMY_DEFAULT_COLOR.b, DUMMY_DEFAULT_COLOR.a);
		}


		//モデル描画
		item.second->Draw();

		//色をもとに戻す
		item.second->ChangeModelColor(ItemManager::DEFAULT_COLOR);
		item.second->SetModelColor(1.0f, 1.0f, 1.0f, 1.0f);
	}
}

void ItemManager::Destroy(void)
{
	AllDeleteItem();
	if (instance_ != nullptr)
	{
		delete instance_;
		instance_ = nullptr;
	}
}

void ItemManager::AddItem(IntVector3 mapPos, Quaternion rot, ItemBase::ITEM_TYPE type,float rotY)
{
	//アイテム
	std::shared_ptr<ItemBase> item;

	//アイテムを生成
	item = CreateItem(type, mapPos, rot);
	if (item == nullptr)
	{
		return;
	}
	item->SetRotY(rotY);
	//配列に追加
	items_[type].emplace_back(std::move(item));
}

void ItemManager::DeleteItem(VECTOR mapPos, int range)
{

}

void ItemManager::DeleteItem(IntVector3 _mapPos, ItemBase::ITEM_TYPE _type)
{
	for (auto& items : items_)
	{
		if (items.first != _type)
		{
			continue;
		}
		for (auto& item : items.second)
		{
			if (item == nullptr)
			{
				continue;
			}
			if (item->GetInitMapPos() == _mapPos)
			{
				item = nullptr;
			}
		}
	}

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

	if (item == nullptr)
	{
		return;
	}

	//配列に追加
	dummyItems_[playerNum] = item;
}

ItemBase::Status ItemManager::GetDummyItemStatus(int playerNum)
{
	ItemBase::Status status{};
	if (dummyItems_.find(playerNum) != dummyItems_.end())
	{
		if (dummyItems_[playerNum] == nullptr)
		{
			return status;
		}
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

IntVector3 ItemManager::GetDummyItemHitSize(int playerNum)
{
	//アイテムの大きさ
	IntVector3 size;
	if (dummyItems_.find(playerNum) == dummyItems_.end() || dummyItems_[playerNum] == nullptr)
	{
		return { -1,-1,-1 };
	}
	size = dummyItems_[playerNum]->GetHitSize();
	return size;
}

IntVector3 ItemManager::GetDummyItemSize(int playerNum)
{
	//アイテムの大きさ
	IntVector3 size;
	if (dummyItems_.find(playerNum) == dummyItems_.end() || dummyItems_[playerNum] == nullptr)
	{
		return { -1,-1,-1 };
	}
	size = dummyItems_[playerNum]->GetSize();
	return size;
}

const Transform ItemManager::GetDummyItemTransform(int playerNum)
{
	//モデル情報
	Transform transform;

	//指定プレイヤーのダミーアイテムがあるか
	if (dummyItems_.find(playerNum) != dummyItems_.end())
	{
		if (dummyItems_[playerNum] == nullptr)
		{
			return Transform();
		}
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
		if (dummyItems_[playerNum] == nullptr)
		{
			return;
		}
		//モデルの回転情報は保存
		Transform transform = dummyItems_[playerNum]->GetTransform();
		
		//新たに指定されたアイテムを生成
		std::shared_ptr<ItemBase> dummy;
		dummy = CreateItem(type, mapPos, transform.quaRot);
		dummy->SetRotY(GetDummyItemRotY(playerNum));
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
		if (dummyItems_[playerNum] == nullptr)
		{
			return;
		}
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
		if (dummyItems_[playerNum] == nullptr)
		{
			return;
		}
		AddItem(dummyItems_[playerNum]->GetInitMapPos(), dummyItems_[playerNum]->GetTransform().quaRot, dummyItems_[playerNum]->GetStatus().itemType,dummyItems_[playerNum]->GetRotY());
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

bool ItemManager::ItemsAddDummyItems(ItemBase::ITEM_TYPE _type, IntVector3 _mapPos,int playerNum)
{
	//アイテムが存在するか
	auto it = items_.find(_type);
	//アイテムが存在しない場合　falseを返す
	if (it == items_.end())
	{
		return false;
	}

	//指定アイテム
	for (auto& item : it->second)
	{
		//アイテムが存在しない場合　次のアイテムへ
		if (item == nullptr)
		{
			//飛ばす
			continue;
		}
		//アイテムのサイズ分ループ
		for (int i = 0; i < item->GetSize().x; i++)
		{
			for (int j = 0; j < item->GetSize().y; j++)
			{
				for (int k = 0; k < item->GetSize().z; k++)
				{
					//初期位置からサイズ分
					IntVector3 mapPos = { item->GetInitMapPos().x + i,item->GetInitMapPos().y + j,item->GetInitMapPos().z + k };
					//アイテムのマップ座標と指定されたマップ座標が同じ場合
					if (mapPos != _mapPos)
					{
						//飛ばす
						continue;
					}
					dummyItems_[playerNum] = CreateItem(_type, _mapPos, item->GetTransform().quaRot);
					dummyItems_[playerNum]->SetRotY(item->GetRotY());
					//元情報を削除
					item = nullptr;
					return true;
				}
			}
		}
	}
	return false;
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

bool ItemManager::IsDummyItem(int playerNum)
{
	if (dummyItems_.find(playerNum) != dummyItems_.end())
	{
		if (dummyItems_[playerNum] == nullptr)
		{
			return false;
		}
		return true;
	}
	return false;
}

Transform ItemManager::GetItemTransform(IntVector3 _mapPos, ItemBase::ITEM_TYPE _type) const
{
	for (auto& items : items_)
	{
		if (items.first != _type)
		{
			continue;
		}
		for (auto& item : items.second)
		{
			if (item == nullptr)
			{
				continue;
			}
			if (item->GetInitMapPos() == _mapPos)
			{
				return item->GetTransform();
			}
		}
	}

	return Transform{};
}

IntVector3 ItemManager::GetItemHitSize(ItemBase::ITEM_TYPE _type) const
{
	//アイテムのサイズ
	IntVector3 size = { -1,-1,-1 };
	auto item = GetInstance().CreateItem(_type, { -1,-1,-1 }, {});
	size = item->GetHitSize();
	return size;
}

IntVector3 ItemManager::GetItemSize(ItemBase::ITEM_TYPE _type) const
{
	//アイテムのサイズ
	IntVector3 size = { -1,-1,-1 };
	auto item = GetInstance().CreateItem(_type, { -1,-1,-1 }, {});
	size =item->GetSize();
	return size;
}

ItemBase::Status ItemManager::GetItemStatus(IntVector3 _mapPos, ItemBase::ITEM_TYPE _type) const
{
	for (auto& items : items_)
	{
		if (items.first != _type)
		{
			continue;
		}
		for (auto& item : items.second)
		{
			if (item == nullptr)
			{
				continue;
			}
			if (item->GetInitMapPos() == _mapPos)
			{
				return item->GetStatus();
			}
		}
	}
	return ItemBase::Status{};
}

float ItemManager::GetItemRotY(ItemBase::ITEM_TYPE _type, IntVector3 _mapPos)
{
	for (auto& items : items_)
	{
		if (items.first != _type)
		{
			continue;
		}
		for (auto& item : items.second)
		{
			if (item == nullptr)
			{
				continue;
			}
			if (item->GetInitMapPos() == _mapPos)
			{
				return item->GetRotY();
			}
		}
	}
	return 0.0f;
}

VECTOR ItemManager::GetStartWorldPos(void) const
{
	VECTOR startPos = { -1.0f,-1.0f,-1.0f };
	auto it = items_.find(ItemBase::ITEM_TYPE::START);
	if (it != items_.end())
	{
		for (auto& item : it->second)
		{
			if (item == nullptr)
			{
				continue;
			}
			IntVector3 mapPos = item->GetInitMapPos();
			//マップ座標からワールド座標に変換
			IntVector3 startMapSize = item->GetSize();
			startPos = MapEditer::GetInstance().MapToWorldPos(mapPos + IntVector3{ startMapSize.x / 2,startMapSize.y,startMapSize.z / 2 });
			startPos.x += startMapSize.x % 2 == 0 ? 0.0f : MapEditer::GRID_SIZE / 2.0f;
			startPos.y += startMapSize.y % 2 == 0 ? 0.0f : MapEditer::GRID_SIZE / 2.0f;
			startPos.z += startMapSize.z % 2 == 0 ? 0.0f : MapEditer::GRID_SIZE / 2.0f;
		}
	}
	return startPos;
}

bool ItemManager::AllDummyItemAddItems(void)
{
	bool isClear = true;	//アイテムをすべて移せたか
	std::vector<int> playerNum;
	//各ダミーアイテム
	for (auto& item : dummyItems_)
	{
		//まだ設置してない
		if (item.second == nullptr)
		{
			//飛ばす
			continue;
		}

		//他オブジェクトと重なっているか
		if (MapEditer::GetInstance().IsObjectAtMapPos(item.second->GetInitMapPos(), item.second->GetSize(),item.second->GetHitSize(),GetDummyItemRotY(item.first)) < 0)
		{
			isClear = false;
		}
		//重なっていない
		else
		{
			playerNum.push_back(item.first);
		}
	}
	for (auto& pNum : playerNum)
	{
		MapEditer::STATUS status;
		status.mapPos = GetDummyItemMapPos(pNum);
		status.rotate = GetDummyItemTransform(pNum).quaRot;
		status.type = dummyItems_[pNum]->GetStatus().itemType;
		MapEditer::GetInstance().AddItem(status, GetDummyItemSize(pNum),GetDummyItemHitSize(pNum), GetDummyItemRotY(pNum));
		DummyItemAddItems(pNum);
	}
	return isClear;
}

void ItemManager::ResetItemValue(void)
{
	//全アイテムタイプ
	for (auto& itemType : items_)
	{
		//全アイテム
		for (auto& item : itemType.second)
		{
			if (item == nullptr)continue;

			//値初期化
			item->ResetValue();
		}
	}
}

float ItemManager::GetDummyItemRotY(int playerNum)
{
	if (dummyItems_.find(playerNum) != dummyItems_.end())
	{
		if (dummyItems_[playerNum] == nullptr)
		{
			return 0.0f;
		}
		return dummyItems_[playerNum]->GetRotY();
	}
	return 0.0f;
}

void ItemManager::SetDummyItemRotY(int playerNum , float rotY)
{
	if (dummyItems_.find(playerNum) != dummyItems_.end())
	{
		if (dummyItems_[playerNum] == nullptr)
		{
			return ;
		}
		dummyItems_[playerNum]->SetRotY(rotY);
	}
	return;
}

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
	case ItemBase::ITEM_TYPE::NONE:
		break;
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
	case ItemBase::ITEM_TYPE::FAN:
		item = std::make_shared<Fan>();
		break;
	case ItemBase::ITEM_TYPE::SLIME_FLOOR:
		item = std::make_shared<SlimeFloor>();
		break;
	case ItemBase::ITEM_TYPE::SPRING:
		item = std::make_shared<Spring>();
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
