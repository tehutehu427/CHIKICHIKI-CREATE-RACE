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

void MapEditer::AddItem(IntVector3 mapPos)
{

}

void MapEditer::DeleteItem(IntVector3 mapPos)
{

}

MapEditer::MapEditer(void)
{
}
