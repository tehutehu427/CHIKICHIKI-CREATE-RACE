#include "MapEditer.h"

MapEditer* MapEditer::instance_ = nullptr;

void MapEditer::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new MapEditer();
	}
	instance_->Init();
}

MapEditer& MapEditer::GetInstance(void)
{
	return *instance_;
}

void MapEditer::Init(void)
{
	for (int i = 0;i < MAP_SIZE.x;i++)
	{
		for (int j = 0;j < MAP_SIZE.y;j++)
		{
			for (int k = 0;k < MAP_SIZE.z;k++)
			{
				isMapPosItem_[i][j][k] = ItemBase::ITEM_TYPE::NONE;
			}
		}
	}
}


bool MapEditer::IsObjectAtMapPos(IntVector3 mapPos)
{
	return GetItemType(mapPos) != ItemBase::ITEM_TYPE::NONE;
}

void MapEditer::AddItem(STATUS status, IntVector3 size)
{
	//アイテムの配置
	for (int i = 0; i < size.x; i++)
	{
		for (int j = 0; j < size.y; j++)
		{
			for (int k = 0; k < size.z; k++)
			{
				IntVector3 mapPos = { status.mapPos.x + i,status.mapPos.y + j,status.mapPos.z + k };
				isMapPosItem_[mapPos.x][mapPos.y][mapPos.z] = status.type;
				itemsPos_[status.type].emplace_back(mapPos);
			}
		}
	}
}

void MapEditer::DeleteItem(const ItemBase::ITEM_TYPE& _type, const IntVector3& mapPos)
{
	
}

IntVector3 MapEditer::WorldToMapPos(VECTOR worldPos)
{
	IntVector3 mapPos;
	mapPos.x = static_cast<int>(worldPos.x / GRID_SIZE);
	mapPos.y = static_cast<int>(worldPos.y / GRID_SIZE);
	mapPos.z = static_cast<int>(worldPos.z / GRID_SIZE);
	return mapPos;
}

VECTOR MapEditer::MapToWorldPos(IntVector3 mapPos)
{
	VECTOR worldPos;
	worldPos.x = static_cast<float>(mapPos.x * GRID_SIZE);
	worldPos.y = static_cast<float>(mapPos.y * GRID_SIZE);
	worldPos.z = static_cast<float>(mapPos.z * GRID_SIZE);
	return worldPos;
}

MapEditer::MapEditer(void)
{
	itemsPos_.clear();
	isMapPosItem_[(MAP_SIZE.x)][(MAP_SIZE.y)][(MAP_SIZE.z)] = {};
}
