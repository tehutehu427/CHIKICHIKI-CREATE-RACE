#include <cassert>
#include "../Utility/Utility.h"
#include "../Object/Editor/EditController.h"
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
	assert(instance_ && "MapEditer::GetInstance() called before CreateInstance()");
	return *instance_;
}

void MapEditer::Init(void)
{
	//アイテムの初期化
	DeleteAllItem();
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

bool MapEditer::IsObjectAtMapPos(IntVector3 mapPos) const
{
	//オブジェクトがあるかを判定して返す
	return GetItemType(mapPos) != ItemBase::ITEM_TYPE::NONE;
}

int MapEditer::IsObjectAtMapPos(IntVector3 _mapPos, IntVector3 _size,IntVector3 _hitSize,float _rotY)
{
	//_rotYが-値にならないように調整する
	_rotY += ONE_LAP_DEG;
	int rot = Utility::Round(_rotY);
	//回転を適用する
	ApplyRotation(_mapPos, _size, _hitSize, rot);

	//当たり判定の範囲内を確認する
	for (int x = 0;x < _hitSize.x;x++)
	{
		for (int y = 0;y < _hitSize.y;y++)
		{
			for (int z = 0;z < _hitSize.z;z++)
			{
				IntVector3 sizeLoop = { x,y,z };
				IntVector3 mapPos = _mapPos + sizeLoop;
				if (!IsMapPosInRange(mapPos))
				{
					//アイテムが範囲外に出ている
					return -static_cast<int>(EditController::ERROR_TYPE::ITEM_RANGE_OUT);
				}
				if (IsObjectAtMapPos(mapPos))
				{
					//アイテムが重なっている
					return -static_cast<int>(EditController::ERROR_TYPE::ITEM_OVER_LAP);
				}
			}
		}
	}
	return -static_cast<int>(EditController::ERROR_TYPE::NONE);
}

void MapEditer::AddItem(STATUS _status, IntVector3 _size ,IntVector3 _hitSize, float _rotY)
{
	//初期値を格納する
	IntVector3 mapPos = _status.mapPos;
	//_rotYが-値にならないように調整する
	_rotY += ONE_LAP_DEG;
	int rot = Utility::Round(_rotY);
	//回転を適用する
	ApplyRotation(_status.mapPos, _size, _hitSize, rot);

	//アイテムの配置
	for (int i = 0; i < _hitSize.x; i++)
	{
		for (int j = 0; j < _hitSize.y; j++)
		{
			for (int k = 0; k < _hitSize.z; k++)
			{
				//配置するマップ座標
				IntVector3 mPos = { _status.mapPos.x + i,_status.mapPos.y + j,_status.mapPos.z + k };
				isMapPosItem_[mPos.x][mPos.y][mPos.z] = _status.type;
				leaderMapPos_[mPos.x][mPos.y][mPos.z] = mapPos;
			}
		}
	}
}

void MapEditer::DeleteItem(IntVector3 _mapPos ,float _rotY ,IntVector3 _size ,IntVector3 _hitSize)
{
	//_rotYが-値にならないように調整する
	_rotY += ONE_LAP_DEG;
	int rot = Utility::Round(_rotY);
	//回転を適用する
	ApplyRotation(_mapPos, _size, _hitSize, rot);

	//アイテムの消去
	for (int i = 0; i < _hitSize.x; i++)
	{
		for (int j = 0; j < _hitSize.y; j++)
		{
			for (int k = 0; k < _hitSize.z; k++)
			{
				//消去するマップ座標
				IntVector3 mapPos = { _mapPos.x + i,_mapPos.y + j,_mapPos.z + k };
				isMapPosItem_[mapPos.x][mapPos.y][mapPos.z] = ItemBase::ITEM_TYPE::NONE;
				leaderMapPos_[mapPos.x][mapPos.y][mapPos.z] = EditController::ERROR_POS;

			}
		}
	}
}

IntVector3 MapEditer::WorldToMapPos(VECTOR worldPos)
{
	IntVector3 mapPos = {};
	mapPos.x = static_cast<int>(worldPos.x / GRID_SIZE);
	mapPos.y = static_cast<int>(worldPos.y / GRID_SIZE);
	mapPos.z = static_cast<int>(worldPos.z / GRID_SIZE);
	return mapPos;
}

VECTOR MapEditer::MapToWorldPos(IntVector3 mapPos)
{
	VECTOR worldPos = {};
	worldPos.x = static_cast<float>(mapPos.x * GRID_SIZE);
	worldPos.y = static_cast<float>(mapPos.y * GRID_SIZE);
	worldPos.z = static_cast<float>(mapPos.z * GRID_SIZE);
	return worldPos;
}

void MapEditer::ApplyRotation(IntVector3& _mapPos, IntVector3& _size, IntVector3& _hitSize, int _rotY)
{
	int rot = static_cast<int>(_rotY) % static_cast<int>(ONE_LAP_DEG);
	switch (rot)
	{
	case 0:
		break;
	case QUATER_ONE_LAP_DEG:
		std::swap(_hitSize.x, _hitSize.z);
		_mapPos.z -= _hitSize.z - _size.z;
		break;
	case HALF_ONE_LAP_DEG:
		_mapPos.x -= _hitSize.x - _size.x;
		break;
	case static_cast<int>(ONE_LAP_DEG) - QUATER_ONE_LAP_DEG:
		std::swap(_hitSize.x, _hitSize.z);
		break;
	default: 
		break;
	}
}


void MapEditer::DeleteAllItem(void)
{
	for (int x = 0; x < MAP_SIZE.x; x++)
	{
		for (int y = 0; y < MAP_SIZE.y; y++)
		{
			for (int z = 0; z < MAP_SIZE.z; z++)
			{
				//マップ座標のアイテムを削除
				isMapPosItem_[x][y][z] = ItemBase::ITEM_TYPE::NONE;
				leaderMapPos_[x][y][z] = EditController::ERROR_POS;
			}
		}
	}
}

bool MapEditer::IsMapPosInRange(IntVector3 mapPos) const
{
	//マップ座標が範囲内かを判定
	if (mapPos.x < 0 || mapPos.x >= MAP_SIZE.x ||
		mapPos.y < 0 || mapPos.y >= MAP_SIZE.y ||
		mapPos.z < 0 || mapPos.z >= MAP_SIZE.z)
	{
		return false;	//範囲外
	}
	return true;	//範囲内
}

MapEditer::MapEditer(void)
{
	//マップの初期化
	for (int x = 0; x < MAP_SIZE.x; x++)
	{
		for (int y = 0; y < MAP_SIZE.y; y++)
		{
			for (int z = 0; z < MAP_SIZE.z; z++)
			{
				//マップ座標のアイテムを削除
				isMapPosItem_[x][y][z] = ItemBase::ITEM_TYPE::NONE;
				leaderMapPos_[x][y][z] = EditController::ERROR_POS;
			}
		}
	}

}

MapEditer::~MapEditer(void)
{
}

