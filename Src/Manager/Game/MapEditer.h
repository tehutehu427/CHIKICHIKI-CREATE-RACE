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

	//アイテムナンバーを返す
	inline int GetItemNom(void) { return itemNom_; }

	//アイテムを追加
	void AddItem(const ItemBase::ITEM_TYPE &_type,const IntVector3 & mapPos);

	//アイテムを削除
	void DeleteItem(const ItemBase::ITEM_TYPE &_type,const IntVector3 & mapPos);

	//種類ごとの配置情報を返す
	inline std::vector<IntVector3> GetItemsPos(const ItemBase::ITEM_TYPE _type) { return itemsPos_[_type]; }

protected:

private:
	static MapEditer* instance_;

	 int isMapPosItem_[(MAP_SIZE.x)][(MAP_SIZE.y)][(MAP_SIZE.z)];	//アイテム番号を入力 0はアイテムなし
	 int itemNom_;

	 //配置したアイテムの座標を管理
	 std::map<ItemBase::ITEM_TYPE ,std::vector<IntVector3> > itemsPos_;

	MapEditer(void);
	MapEditer(const MapEditer& instance_) = default;
	~MapEditer(void) = default;
};