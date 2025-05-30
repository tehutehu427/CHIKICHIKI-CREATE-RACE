#pragma once
#include<DxLib.h>
#include"ItemManager.h"
#include"../../Common/Quaternion.h"
#include"../../Common/IntVector3.h"
class MapEditer
{
public:

	static constexpr IntVector3 MAP_SIZE = { 40,5,30 };
	static constexpr int GRID_SIZE = 100;

	struct STATUS
	{
		IntVector3 mapPos = {-1,-1,-1};
		Quaternion rotate = {};
		ItemBase::ITEM_TYPE type = ItemBase::ITEM_TYPE::NONE;
	};
	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static MapEditer& GetInstance(void);

	void Init(void);
	/// <summary>
	/// 指定のマップ座標のアイテムの種類を判別する
	/// </summary>
	/// <param name="mapPos"></param>
	/// <returns></returns>
	ItemBase::ITEM_TYPE GetItemType(IntVector3 mapPos) { return isMapPosItem_[mapPos.x][mapPos.y][mapPos.z]; }
	/// <summary>
	/// 支点を返す
	/// </summary>
	/// <param name="mapPos">マップ座標</param>
	/// <returns>支点座標</returns>
	IntVector3 GetLeaderMapPos(IntVector3 mapPos) const { return leaderMapPos_[mapPos.x][mapPos.y][mapPos.z]; }

	void Destroy(void);

	/// <summary>
	/// 指定のマップ座標にオブジェクトがあるか
	/// </summary>
	/// <param name="mapPos"></param>
	/// <returns></returns>
	bool IsObjectAtMapPos(IntVector3 mapPos);
	/// <summary>
	/// 指定のマップ座標にオブジェクトがあるか（サイズ内）
	/// </summary>
	/// <param name="mapPos"></param>
	/// <param name="size"></param>
	/// <returns></returns>
	bool IsObjectAtMapPos(IntVector3 mapPos, IntVector3 size);

	void AddItem(STATUS status, IntVector3 size);
	void DeleteItem(ItemBase::ITEM_TYPE _type, IntVector3 _mapPos, IntVector3 _size);
	/// <summary>
	/// ワールド座標をマップ座標に変換する
	/// </summary>
	/// <param name="worldPos">ワールド座標</param>
	/// <returns>マップ座標</returns>
	IntVector3 WorldToMapPos(VECTOR worldPos);
	VECTOR MapToWorldPos(IntVector3 mapPos);

	void DeleteAllItem(void);
protected:

private:
	static MapEditer* instance_;

	ItemBase::ITEM_TYPE isMapPosItem_[(MAP_SIZE.x)][(MAP_SIZE.y)][(MAP_SIZE.z)];	//アイテムタイプを入力 
	IntVector3 leaderMapPos_[(MAP_SIZE.x)][(MAP_SIZE.y)][(MAP_SIZE.z)];				//アイテムの支点を入力　アイテム無しは{ -1,-1,-1 }

	MapEditer(void);
	MapEditer(const MapEditer& instance_) = default;
	~MapEditer(void) = default;
};