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
		ItemBase::ITEM_TYPE type = ItemBase::ITEM_TYPE::NONE;	//アイテムの種類
	};
	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static MapEditer& GetInstance(void);

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name=""></param>
	void Init(void);

	/// <summary>
	/// 指定のマップ座標のアイテムの種類を判別する
	/// </summary>
	/// <param name="mapPos">マップ座標</param>
	/// <returns>アイテムの種類</returns>
	ItemBase::ITEM_TYPE GetItemType(IntVector3 mapPos) const { return isMapPosItem_[mapPos.x][mapPos.y][mapPos.z]; }

	/// <summary>
	/// 支点を返す
	/// </summary>
	/// <param name="mapPos">マップ座標</param>
	/// <returns>支点座標</returns>
	IntVector3 GetLeaderMapPos(IntVector3 mapPos) const { return leaderMapPos_[mapPos.x][mapPos.y][mapPos.z]; }

	/// <summary>
	/// インスタンスの削除
	/// </summary>
	/// <param name=""></param>
	void Destroy(void);

	/// <summary>
	/// 指定のマップ座標にオブジェクトがあるか
	/// </summary>
	/// <param name="mapPos"></param>
	/// <returns></returns>
	bool IsObjectAtMapPos(IntVector3 mapPos) const;

	/// <summary>
	/// 指定のマップ座標にオブジェクトがあるか（サイズ内）
	/// </summary>
	/// <param name="mapPos"></param>
	/// <param name="size"></param>
	/// <returns>0の場合　重なっていない　-1の場合　範囲外に出ている -2の場合　重なっている</returns>
	int IsObjectAtMapPos(IntVector3 _mapPos, IntVector3 _size, IntVector3 _hitSize,float _rotY);

	/// <summary>
	/// アイテムを追加する
	/// </summary>
	/// <param name="_status">マップ座標とアイテムの種類</param>
	/// <param name="_size">モデルの大きさ</param>
	/// <param name="_hitSize">モデルの移動範囲</param>
	/// <param name="_rotY">Y軸回転(Degree)</param>
	void AddItem(STATUS _status, IntVector3 _size,IntVector3 _hitSize, float _rotY);

	/// <summary>
	/// アイテムを削除する
	/// </summary>
	/// <param name="_mapPos">マップ座標</param>
	/// <param name="_rotY">Y軸回転(Degree)</param>
	/// <param name="_size">モデルの大きさ</param>
	/// <param name="_hitSize">モデルの移動範囲</param>
	void DeleteItem(IntVector3 _mapPos, float _rotY , IntVector3 _size,IntVector3 _hitSize);

	/// <summary>
	/// ワールド座標をマップ座標に変換する
	/// </summary>
	/// <param name="worldPos">ワールド座標</param>
	/// <returns>マップ座標</returns>
	IntVector3 WorldToMapPos(VECTOR _worldPos);

	/// <summary>
	/// マップ座標をワールド座標に変換する
	/// </summary>
	/// <param name="_mapPos">マップ座標</param>
	/// <returns>ワールド座標</returns>
	VECTOR MapToWorldPos(IntVector3 _mapPos);

	/// <summary>
	/// 回転時にマップ座標とサイズを調整する
	/// </summary>
	/// <param name="_mapPos">マップ座標</param>
	/// <param name="_size">モデルの大きさ</param>
	/// <param name="_hitSize">モデルの移動範囲</param>
	/// <param name="_rotY">Y軸回転(Degree)</param>
	void ApplyRotation(IntVector3& _mapPos, IntVector3& _size, IntVector3& _hitSize, int _rotY);

	/// <summary>
	/// 全てのアイテムを削除する
	/// </summary>
	/// <param name=""></param>
	void DeleteAllItem(void);

	/// <summary>
	/// 受け取ったマップ座標がマップの範囲内かを判定する
	/// </summary>
	/// <param name="mapPos">マップ座標</param>
	/// <returns>true 範囲内　false 範囲外</returns>
	bool IsMapPosInRange(IntVector3 mapPos) const;
protected:

private:
	static MapEditer* instance_;

	ItemBase::ITEM_TYPE isMapPosItem_[(MAP_SIZE.x)][(MAP_SIZE.y)][(MAP_SIZE.z)];	//アイテムタイプを入力 
	IntVector3 leaderMapPos_[(MAP_SIZE.x)][(MAP_SIZE.y)][(MAP_SIZE.z)];				//アイテムの支点を入力　アイテム無しは{ -1,-1,-1 }

	MapEditer(void);
	MapEditer(const MapEditer& instance_) = default;
	~MapEditer(void);
};