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

bool MapEditer::IsObjectAtMapPos(IntVector3 _mapPos, IntVector3 _size,IntVector3 _hitSize,float _rotY)
{

	int rot = static_cast<int>(_rotY) % 360;
	switch (rot)
	{
	case 0:
		break;
	case 90:
		std::swap(_hitSize.x, _hitSize.z);
		_mapPos.z -= _hitSize.z - _size.z;
		break;
	case 180:
		_mapPos.x -= _hitSize.x - _size.x;
		break;
	case 270:
		std::swap(_hitSize.x, _hitSize.z);
		break;
	default:
		break;
	}


	for (int x = 0;x < _hitSize.x;x++)
	{
		for (int y = 0;y < _hitSize.y;y++)
		{
			for (int z = 0;z < _hitSize.z;z++)
			{
				IntVector3 sizeLoop = { x,y,z };
				IntVector3 mapPos = _mapPos + sizeLoop;
				if (mapPos.x < 0 || mapPos.x > MAP_SIZE.x ||
					mapPos.y < 0 || mapPos.y >MAP_SIZE.y ||
					mapPos.z <0 || mapPos.z > MAP_SIZE.z)
				{
					return true;
				}
				if (IsObjectAtMapPos(mapPos))
				{
					return true;
				}
			}
		}
	}
	return false;
}

void MapEditer::AddItem(STATUS _status, IntVector3 _size ,IntVector3 _hitSize, float _rotY)
{
	IntVector3 mapPos = _status.mapPos;
	int rot = static_cast<int>(_rotY) % 360;
	switch (rot)
	{
	case 0:
		break;
	case 90:
		std::swap(_hitSize.x, _hitSize.z);
		_status.mapPos.z -= _hitSize.z - _size.z;
		break;
	case 180:
		_status.mapPos.x -= _hitSize.x - _size.x;
		break;
	case 270:
		std::swap(_hitSize.x, _hitSize.z);
		break;
	default:
		break;
	}


	//アイテムの配置
	for (int i = 0; i < _hitSize.x; i++)
	{
		for (int j = 0; j < _hitSize.y; j++)
		{
			for (int k = 0; k < _hitSize.z; k++)
			{
				IntVector3 mPos = { _status.mapPos.x + i,_status.mapPos.y + j,_status.mapPos.z + k };
				isMapPosItem_[mPos.x][mPos.y][mPos.z] = _status.type;
				leaderMapPos_[mPos.x][mPos.y][mPos.z] = mapPos;
			}
		}
	}
}

void MapEditer::DeleteItem(ItemBase::ITEM_TYPE _type, IntVector3 _mapPos ,float _rotY ,IntVector3 _size ,IntVector3 _hitSize)
{
	int rot = static_cast<int>(_rotY) % 360;
	switch (rot)
	{
	case 0:
		break;
	case 90:
		std::swap(_hitSize.x, _hitSize.z);
		_mapPos.z -= _hitSize.z - _size.z;
		break;
	case 180:
		_mapPos.x -= _hitSize.x - _size.x;
		break;
	case 270:
		std::swap(_hitSize.x, _hitSize.z);
		break;
	default:
		break;
	}

	//アイテムの消去
	for (int i = 0; i < _hitSize.x; i++)
	{
		for (int j = 0; j < _hitSize.y; j++)
		{
			for (int k = 0; k < _hitSize.z; k++)
			{
				IntVector3 mapPos = { _mapPos.x + i,_mapPos.y + j,_mapPos.z + k };
				if (!IsObjectAtMapPos(mapPos)) 
				{
					int a = 0;
				}
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

MapEditer::~MapEditer(void)
{
}
