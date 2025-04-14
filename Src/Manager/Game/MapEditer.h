#pragma once
#include<DxLib.h>
#include"ItemManager.h"
class MapEditer
{
public:

	static constexpr VECTOR MAP_SIZE = { 30,15,30 };


	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static MapEditer& GetInstance(void);

	void Init(void);
protected:

private:
	static MapEditer* instance_;

	 int isMapPosObject_[static_cast<int>(MAP_SIZE.x)][static_cast<int>(MAP_SIZE.y)][static_cast<int>(MAP_SIZE.z)];	//オブジェクト番号を入力 0はオブジェクトなし
	 int objectNom_;

	MapEditer(void);
	MapEditer(const MapEditer& instance_) = default;
	~MapEditer(void) = default;
};

