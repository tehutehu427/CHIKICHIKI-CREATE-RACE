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
		for (int j = 0;i < MAP_SIZE.y;i++)
		{
			for (int k = 0;i < MAP_SIZE.z;i++)
			{
				isMapPosItem_[i][j][k] = ItemBase::ITEM_TYPE::NONE;
			}
		}
	}
}

void MapEditer::AddItem(STATUS status)
{
	ItemManager::GetInstance().AddItem(status.mapPos,status.rotate,status.type);
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

MapEditer::MapEditer(void)
{
	itemNom_ = -1;
	itemsPos_.clear();
	isMapPosItem_[(MAP_SIZE.x)][(MAP_SIZE.y)][(MAP_SIZE.z)] = {};
}
