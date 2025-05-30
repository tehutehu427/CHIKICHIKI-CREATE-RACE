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
				leaderMapPos_[i][j][k] = { -1,-1,-1 };
			}
		}
	}
}


void MapEditer::Destroy(void)
{
	DeleteAllItem();
	if (instance_ != nullptr)
	{
		delete instance_;
		instance_ = nullptr;
	}
}

bool MapEditer::IsObjectAtMapPos(IntVector3 mapPos)
{
	return GetItemType(mapPos) != ItemBase::ITEM_TYPE::NONE;
}

bool MapEditer::IsObjectAtMapPos(IntVector3 mapPos, IntVector3 size)
{
	for (int x = 0;x < size.x;x++)
	{
		for (int y = 0;y < size.y;y++)
		{
			for (int z = 0;z < size.z;z++)
			{
				IntVector3 sizeLoop = { x,y,z };
				if (IsObjectAtMapPos(mapPos + sizeLoop))
				{
					return true;
				}
			}
		}
	}
	return false;
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
				leaderMapPos_[mapPos.x][mapPos.y][mapPos.z] = status.mapPos;
			}
		}
	}
}

void MapEditer::DeleteItem(ItemBase::ITEM_TYPE _type, IntVector3 _mapPos ,IntVector3 _size)
{
	//アイテムの消去
	for (int i = 0; i < _size.x; i++)
	{
		for (int j = 0; j < _size.y; j++)
		{
			for (int k = 0; k < _size.z; k++)
			{
				IntVector3 mapPos = { _mapPos.x + i,_mapPos.y + j,_mapPos.z + k };
				isMapPosItem_[mapPos.x][mapPos.y][mapPos.z] = ItemBase::ITEM_TYPE::NONE;
				leaderMapPos_[mapPos.x][mapPos.y][mapPos.z] = { -1,-1,-1 };
			}
		}
	}
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

void MapEditer::DeleteAllItem(void)
{
	for (int i = 0; i < MAP_SIZE.x; i++)
	{
		for (int j = 0; j < MAP_SIZE.y; j++)
		{
			for (int k = 0; k < MAP_SIZE.z; k++)
			{
				isMapPosItem_[i][j][k] = ItemBase::ITEM_TYPE::NONE;
				leaderMapPos_[i][j][k] = { -1,-1,-1 };
			}
		}
	}
}

MapEditer::MapEditer(void)
{
	isMapPosItem_[(MAP_SIZE.x)][(MAP_SIZE.y)][(MAP_SIZE.z)] = {};
}
