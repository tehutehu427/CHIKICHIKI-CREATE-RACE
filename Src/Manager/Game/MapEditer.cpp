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

void MapEditer::AddItem(const ItemBase::ITEM_TYPE& _type, const IntVector3& mapPos)
{
	//À•W‚ð’Ç‰Á
	itemsPos_[_type].push_back(mapPos);
}

void MapEditer::DeleteItem(const ItemBase::ITEM_TYPE& _type, const IntVector3& mapPos)
{
	//À•W‚ðíœ
}

MapEditer::MapEditer(void)
{
	itemNom_ = -1;
	itemsPos_.clear();
	isMapPosItem_[(MAP_SIZE.x)][(MAP_SIZE.y)][(MAP_SIZE.z)] = {};
}
