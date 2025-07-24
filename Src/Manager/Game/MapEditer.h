#pragma once
#include<DxLib.h>
#include"../Object/Item/ItemBase.h"
#include"../../Common/Quaternion.h"
#include"../../Common/IntVector3.h"
class MapEditer
{
public:

	// マップのサイズ
	static constexpr IntVector3 MAP_SIZE = { 40,5,30 };
	// グリッドのサイズ
	static constexpr int GRID_SIZE = 100;

	static constexpr float ONE_LAP_DEG = 360.0f;	//1周の角度

	static constexpr int HALF_ONE_LAP_DEG = 180;	//1/2周の角度

	static constexpr int QUATER_ONE_LAP_DEG = 90;	//1/4周の角度	


	struct STATUS
	{
		IntVector3 mapPos = { -1,-1,-1 };	//マップ座標
		Quaternion rotate = {};		//回転情報
		ItemBase::ITEM_TYPE type = ItemBase::ITEM_TYPE::NONE;	//アイテムの種類
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
	/// <returns>0の場合　重なっていない　-1の場合　範囲外に出ている -2の場合　重なっている</returns>
	int IsObjectAtMapPos(IntVector3 _mapPos, IntVector3 _size, IntVector3 _hitSize,float _rotY);

	void AddItem(STATUS _status, IntVector3 _size,IntVector3 _hitSize, float _rotY);
	void DeleteItem(IntVector3 _mapPos, float _rotY , IntVector3 _size,IntVector3 _hitSize);
	/// <summary>
	/// ワールド座標をマップ座標に変換する
	/// </summary>
	/// <param name="worldPos">ワールド座標</param>
	/// <returns>マップ座標</returns>
	IntVector3 WorldToMapPos(VECTOR _worldPos);
	VECTOR MapToWorldPos(IntVector3 _mapPos);

	/// <summary>
	/// 回転時にマップ座標とサイズを調整する
	/// </summary>
	/// <param name="_mapPos"></param>
	/// <param name="_size"></param>
	/// <param name="_hitSize"></param>
	/// <param name="_rotY"></param>
	void ApplyRotation(IntVector3& _mapPos, IntVector3& _size, IntVector3& _hitSize, int _rotY);

	void DeleteAllItem(void);
protected:

private:
	static MapEditer* instance_;

	ItemBase::ITEM_TYPE isMapPosItem_[(MAP_SIZE.x)][(MAP_SIZE.y)][(MAP_SIZE.z)];	//アイテムタイプを入力 
	IntVector3 leaderMapPos_[(MAP_SIZE.x)][(MAP_SIZE.y)][(MAP_SIZE.z)];				//アイテムの支点を入力　アイテム無しは{ -1,-1,-1 }

	MapEditer(void);
	MapEditer(const MapEditer& instance_) = default;
	~MapEditer(void);
};