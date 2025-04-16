#pragma once
#include<DxLib.h>
#include"ItemManager.h"
#include"../../Common/Quaternion.h"
#include"../../Common/IntVector3.h"
class MapEditer
{
public:

	static constexpr IntVector3 MAP_SIZE = { 50,5,40 };
	static constexpr int GRID_SIZE = 50;

	struct STATUS
	{
		VECTOR mapPos;
		Quaternion rotate;
		int itemNom;
	};
	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static MapEditer& GetInstance(void);

	void Init(void);

	int GetItemNom(void) { return itemNom_; }
	void AddItem(IntVector3 mapPos);
	void DeleteItem(IntVector3 mapPos);
protected:

private:
	static MapEditer* instance_;

	 int isMapPosItem_[(MAP_SIZE.x)][(MAP_SIZE.y)][(MAP_SIZE.z)];	//アイテム番号を入力 0はアイテムなし
	 int itemNom_;

	MapEditer(void);
	MapEditer(const MapEditer& instance_) = default;
	~MapEditer(void) = default;
};

