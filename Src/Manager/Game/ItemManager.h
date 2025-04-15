#pragma once
#include <DxLib.h>
#include <vector>
#include "../../Common/Quaternion.h"
#include "../../Common/IntVector3.h"
#include "../../Object/Item/ItemBase.h"
class ItemManager
{
public:



	void Init(void);

	void AddItem(IntVector3 mapPos,Quaternion rot,ItemBase::ITEM_TYPE type);				//アイテム追加　ステータス
	void DeleteItem(VECTOR mapPos, int range);	//アイテム消去　消去の中心地, 消去の範囲

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static ItemManager& GetInstance(void);
protected:

private:
	static ItemManager* instance_;

	std::vector<ItemBase*> items_;


	ItemManager(void);
	ItemManager(const ItemManager& instance_) = default;
	~ItemManager(void) = default;
};

