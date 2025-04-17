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
				isMapPosItem_[i][j][k] = 0;
			}
		}
	}
	itemNom_ = 1;
}

void MapEditer::AddItem(STATUS status)
{
	ItemManager::GetInstance().AddItem(status.mapPos,status.rotate,status.type);
	isMapPosItem_[status.mapPos.x][status.mapPos.y][status.mapPos.z] = itemNom_++;

	itemNom_++;
}

void MapEditer::DeleteItem(IntVector3 mapPos)
{

}

MapEditer::MapEditer(void)
{
}
