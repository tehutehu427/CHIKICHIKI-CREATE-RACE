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

	static constexpr float DUMMY_ITEM_OPACITY_RATE = 0.5f;	//ダミーアイテムの半透明度
	static constexpr float DEFAULT_OPACITY_RATE = 1.0f;		//デフォルトの半透明度
	static constexpr COLOR_F DUMMY_OVERLAP_COLOR = { 1.0f,0.0f,0.0f,DUMMY_ITEM_OPACITY_RATE };	//ダミーの重なっているときの色
	static constexpr COLOR_F DUMMY_DEFAULT_COLOR = { 0.0f,0.0f,1.0f,DUMMY_ITEM_OPACITY_RATE };	//ダミーのデフォルトの色
	static constexpr COLOR_F DEFAULT_COLOR = { 1.0f,1.0f,1.0f,DEFAULT_OPACITY_RATE };	//デフォルトの色
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
	/// すべてのアイテムを削除
	/// </summary>
	void AllDeleteItem();

	/// <summary>
	/// ダミーアイテム追加
	/// </summary>
	/// <param name="mapPos">生成させるマップ座標</param>
	/// <param name="rot">生成させる時の回転情報</param>
	/// <param name="type">アイテムの種類</param>
	void CreateDummyItem(IntVector3 mapPos, Quaternion rot, ItemBase::ITEM_TYPE type, int playerNum);

	ItemBase::Status GetDummyItemStatus(int playerNum);
	IntVector3 GetDummyItemMapPos(int playerNum);
	IntVector3 GetDummyObjectSize(int playerNum);
	Transform GetDummyItemTransform(int playerNum);
	/// <summary>
	/// ダミーアイテムを置き換える
	/// </summary>
	/// <param name="playerNum"></param>
	/// <param name="type"></param>
	/// <param name="mapPos"></param>
	void ResetDummyItem(int playerNum,ItemBase::ITEM_TYPE type,IntVector3 mapPos);

	/// <summary>
	/// ダミーアイテムの座標設定
	/// </summary>
	/// <param name="mapPos"></param>
	/// <param name="playerNum"></param>
	void DummyItemSetMapPos(IntVector3 mapPos, int playerNum);
	/// <summary>
	/// ダミーアイテムの回転を設定
	/// </summary>
	/// <param name="rot"></param>
	/// <param name="playerNum"></param>
	void DummyItemSetRotate(Quaternion rot, int playerNum);
	/// <summary>
	/// ダミーアイテムをアイテムに追加
	/// </summary>
	/// <param name="playerNum"></param>
	void DummyItemAddItems(int playerNum);

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static ItemManager& GetInstance(void);


	/// <summary>
	/// 指定した種類のアイテム配列を返す
	/// </summary>
	/// <param name="_type">種類</param>
	/// <returns>指定したアイテム配列のポインタ。存在しなければ nullptr</returns>
	const std::vector<std::shared_ptr<ItemBase>>* GetItems(const ItemBase::ITEM_TYPE _type)const;
	
	void ItemsAddDummyItems(ItemBase::ITEM_TYPE _type, IntVector3 _mapPos , int playerNum);

	void DeleteDummyItem(int playerNum);

	/// <summary>
	/// サブアイテムの所有権獲得
	/// </summary>
	/// <param name="_type">所有権を渡したいアイテムの種類</param>
	/// <param name="_subItem">所有権を渡したいアイテムのポインタ(std::moveして渡す)</param>
	void MoveSubItemOwner(const ItemBase::ITEM_TYPE _type, std::shared_ptr<ItemBase> _subItem);

protected:

private:
	static ItemManager* instance_;

	//種類ごとにアイテムを管理
	std::map<ItemBase::ITEM_TYPE, std::vector<std::shared_ptr<ItemBase>>> items_;
	//配置中のアイテム[プレイヤー番号][アイテムの種類]
	std::map<int,std::shared_ptr<ItemBase>> dummyItems_;
	ItemManager(void);
	//コピーコンストラクタ及び代入演算の禁止
	ItemManager(const ItemManager& instance_) = delete;
	void operator= (const ItemManager& instance_) = delete;
	~ItemManager(void) = default;
	//アイテムの生成
	std::shared_ptr<ItemBase> CreateItem(ItemBase::ITEM_TYPE type, IntVector3 mapPos, Quaternion rot);
};

