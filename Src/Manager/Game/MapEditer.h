#pragma once
#include<DxLib.h>
#include"ItemManager.h"
#include"../../Common/Quaternion.h"
class MapEditer
{
public:

	static constexpr VECTOR MAP_SIZE = { 30,15,30 };

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
	void AddItem(VECTOR mapPos);
	void DeleteItem(VECTOR mapPos);
protected:

private:
	static MapEditer* instance_;

	 int isMapPosItem_[static_cast<int>(MAP_SIZE.x)][static_cast<int>(MAP_SIZE.y)][static_cast<int>(MAP_SIZE.z)];	//アイテム番号を入力 0はアイテムなし
	 int itemNom_;

	MapEditer(void);
	MapEditer(const MapEditer& instance_) = default;
	~MapEditer(void) = default;
};

