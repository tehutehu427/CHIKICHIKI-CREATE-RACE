#pragma once
#include<DxLib.h>
#include"ItemManager.h"
#include"../../Common/Quaternion.h"
#include"../../Common/IntVector3.h"
class MapEditer
{
public:

	static constexpr IntVector3 MAP_SIZE = { 50,5,40 };
	static constexpr int GRID_SIZE = 100;

	struct STATUS
	{
		IntVector3 mapPos;
		Quaternion rotate;
		ItemBase::ITEM_TYPE type;
	};
	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static MapEditer& GetInstance(void);

	void Init(void);

	void AddItem(STATUS status);
	void DeleteItem(IntVector3 mapPos);
	IntVector3 WorldToMapPos(VECTOR worldPos);
protected:

private:
	static MapEditer* instance_;

	 ItemBase::ITEM_TYPE isMapPosItem_[(MAP_SIZE.x)][(MAP_SIZE.y)][(MAP_SIZE.z)];	//アイテム番号を入力 0はアイテムなし

	MapEditer(void);
	MapEditer(const MapEditer& instance_) = default;
	~MapEditer(void) = default;
};

