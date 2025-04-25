#pragma once
#include <DxLib.h>
#include <vector>
#include <map>
#include <memory>
#include "../../Common/Quaternion.h"
#include "../../Common/IntVector3.h"
#include "../../Object/Item/ItemBase.h"
class ItemManager
{
public:

	void Init(void);
	void Update(void);
	void Draw(void);

	/// <summary>
	/// アイテム追加
	/// </summary>
	/// <param name="mapPos">生成させるマップ座標</param>
	/// <param name="rot">生成させる時の回転情報</param>
	/// <param name="type">アイテムの種類</param>
	void AddItem(IntVector3 mapPos,Quaternion rot,ItemBase::ITEM_TYPE type);

	/// <summary>
	/// アイテム消去
	/// </summary>
	/// <param name="mapPos">消去位置</param>
	/// <param name="range">消去範囲</param>
	void DeleteItem(VECTOR mapPos, int range);

	/// <summary>
	/// ダミーアイテム追加
	/// </summary>
	/// <param name="mapPos">生成させるマップ座標</param>
	/// <param name="rot">生成させる時の回転情報</param>
	/// <param name="type">アイテムの種類</param>
	void CreateDummyItem(IntVector3 mapPos, Quaternion rot, ItemBase::ITEM_TYPE type, int playerNum);

	ItemBase::Status GetDummyItemStatus(int playerNum);
	IntVector3 GetDummyObjectSize(int playerNum);
	/// <summary>
	/// ダミーアイテムをアイテムに追加
	/// </summary>
	/// <param name="playerNum"></param>
	void DummyItemAddItems(int playerNum);

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static ItemManager& GetInstance(void);
protected:

private:
	static ItemManager* instance_;

	//アイテム[アイテムを置いた順番][置いたアイテム]
	std::vector<std::shared_ptr<ItemBase>> items_;
	//配置中のアイテム[プレイヤー番号][アイテムの種類]
	std::map<int,std::shared_ptr<ItemBase>> dummyItems_;
	ItemManager(void);
	//コピーコンストラクタ及び代入演算の禁止
	ItemManager(const ItemManager& instance_) = delete;
	void operator= (const ItemManager& instance_) = delete;
	~ItemManager(void) = default;
};

