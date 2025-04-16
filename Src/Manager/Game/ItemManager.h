#pragma once
#include <DxLib.h>
#include <vector>
#include <map>
#include "../../Common/Quaternion.h"
#include "../../Common/IntVector3.h"
#include "../../Object/Item/ItemBase.h"
class ItemManager
{
public:



	void Init(void);

	void AddItem(IntVector3 mapPos,Quaternion rot,ItemBase::ITEM_TYPE type,int itemNom);				//アイテム追加　ステータス
	void DeleteItem(VECTOR mapPos, int range);	//アイテム消去　消去の中心地, 消去の範囲

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static ItemManager& GetInstance(void);
protected:

private:
	static ItemManager* instance_;

	std::map<int/*アイテムを置いた順番*/, ItemBase*/*置いたアイテム*/> items_;


	ItemManager(void);
	ItemManager(const ItemManager& instance_) = default;
	~ItemManager(void) = default;
};

