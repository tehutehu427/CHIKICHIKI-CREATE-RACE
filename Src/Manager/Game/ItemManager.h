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
	static constexpr float DUMMY_ITEM_OPACITY_RATE = 0.3f;	//ダミーアイテムの半透明度
	static constexpr float DEFAULT_OPACITY_RATE = 1.0f;		//デフォルトの半透明度
	static constexpr COLOR_F DUMMY_OVERLAP_COLOR = { 1.0f,0.1f,0.1f,DUMMY_ITEM_OPACITY_RATE };	//ダミーの重なっているときの色
	static constexpr COLOR_F DUMMY_DEFAULT_COLOR = { 0.1f,0.1f,1.0f,DUMMY_ITEM_OPACITY_RATE };	//ダミーのデフォルトの色
	static constexpr COLOR_F DEFAULT_COLOR = { 1.0f,1.0f,1.0f,DEFAULT_OPACITY_RATE };	//デフォルトの色

	//初期化
	void Init(void);
	//更新
	void Update(void);
	//描画
	void Draw(void);
	//インスタンスの削除
	void Destroy(void);

	/// <summary>
	/// アイテム追加
	/// </summary>
	/// <param name="mapPos">生成させるマップ座標</param>
	/// <param name="rot">生成させる時の回転情報</param>
	/// <param name="type">アイテムの種類</param>
	void AddItem(IntVector3 _mapPos,Quaternion _rot,ItemBase::ITEM_TYPE _type,float _rotY);

	/// <summary>
	/// マップ座標を参照してアイテム消去
	/// </summary>
	/// <param name="_mapPos">マップ座標</param>
	/// <param name="_type">アイテムの種類</param>
	void DeleteItem(IntVector3 _mapPos, ItemBase::ITEM_TYPE _type);

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

	/// <summary>
	/// ダミーアイテムのステータスを取得
	/// </summary>
	/// <param name="playerNum">プレイヤー番号</param>
	/// <returns>ダミーアイテムのステータス</returns>
	ItemBase::Status GetDummyItemStatus(int playerNum);

	/// <summary>
	/// ダミーアイテムのマップ座標を取得
	/// </summary>
	/// <param name="playerNum">プレイヤー番号</param>
	/// <returns>ダミーアイテムのマップ座標</returns>
	IntVector3 GetDummyItemMapPos(int playerNum);

	/// <summary>
	/// ダミーアイテムの当たり判定サイズを取得
	/// </summary>
	/// <param name="playerNum">プレイヤー番号</param>
	/// <returns>ダミーアイテムのサイズ</returns>
	IntVector3 GetDummyItemHitSize(int playerNum);

	/// <summary>
	/// ダミーアイテムのサイズを取得
	/// </summary>
	/// <param name="playerNum">プレイヤー番号</param>
	/// <returns>ダミーアイテムのサイズ</returns>
	IntVector3 GetDummyItemSize(int playerNum);

	/// <summary>
	/// ダミーアイテムのTransformを取得
	/// </summary>
	/// <param name="playerNum">プレイヤー番号</param>
	/// <returns>ダミーアイテムのTransform</returns>
	const Transform& GetDummyItemTransform(int playerNum);

	/// <summary>
	/// ダミーアイテムの座標設定
	/// </summary>
	/// <param name="mapPos">マップ座標</param>
	/// <param name="playerNum">プレイヤー番号</param>
	void DummyItemSetMapPos(IntVector3 mapPos, int playerNum);

	/// <summary>
	/// ダミーアイテムの回転を設定
	/// </summary>
	/// <param name="rot">回転量</param>
	/// <param name="playerNum">プレイヤー番号</param>
	void DummyItemSetRotate(Quaternion rot, int playerNum);

	/// <summary>
	/// ダミーアイテムをアイテムに追加
	/// </summary>
	/// <param name="playerNum">プレイヤー番号</param>
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
	
	/// <summary>
	/// itemからdummyItemに置き換える
	/// </summary>
	/// <param name="_type">置き換えるアイテムの種類</param>
	/// <param name="_mapPos">マップ座標</param>
	/// <param name="playerNum">プレイヤー番号</param>
	/// <returns>成功ならtrue 失敗ならfalse</returns>
	bool ItemsAddDummyItems(ItemBase::ITEM_TYPE _type, IntVector3 _mapPos , int playerNum);

	/// <summary>
	/// 特定のプレイヤーのダミーアイテムを削除
	/// </summary>
	/// <param name="playerNum">プレイヤー番号</param>
	void DeleteDummyItem(int playerNum);

	/// <summary>
	/// ダミーアイテムが存在するか
	/// </summary>
	/// <param name="playerNum">プレイヤー番号</param>
	/// <returns>存在するtrue 存在しないfalse</returns>
	bool IsDummyItem(int playerNum);

	/// <summary>
	/// アイテムのtransformを取得
	/// </summary>
	/// <param name="_mapPos">マップ座標</param>
	/// <param name="_type">アイテムの種類</param>
	/// <returns></returns>
	const Transform& GetItemTransform(IntVector3 _mapPos , ItemBase::ITEM_TYPE _type) const;

	/// <summary>
	/// アイテムの当たり判定大きさを取得
	/// </summary>
	/// <param name="_type">アイテムの種類</param>
	/// <returns></returns>
	IntVector3 GetItemHitSize(ItemBase::ITEM_TYPE _type) const;

	/// <summary>
	/// アイテムの大きさを取得
	/// </summary>
	/// <param name="_type">アイテムの種類</param>
	/// <returns></returns>
	IntVector3 GetItemSize(ItemBase::ITEM_TYPE _type) const;

	/// <summary>
	/// アイテムのステータスを取得
	/// </summary>
	/// <param name="_mapPos">マップ座標</param>
	/// <param name="_type">アイテムの種類</param>
	/// <returns>ステータス</returns>
	ItemBase::Status GetItemStatus(IntVector3 _mapPos, ItemBase::ITEM_TYPE _type) const;

	/// <summary>
	/// アイテムのY軸回転を取得
	/// </summary>
	/// <param name="_type">アイテムの種類</param>
	/// <param name="_mapPos">マップ座標</param>
	/// <returns>Y軸回転</returns>
	float GetItemRotY(ItemBase::ITEM_TYPE _type, IntVector3 _mapPos);

	/// <summary>
	/// スタートのワールド座標を返す
	/// </summary>
	/// <param name=""></param>
	/// <returns>スタートの座標</returns>
	VECTOR GetStartWorldPos(void) const;

	/// <summary>
	/// すべてのプレイヤーのダミーアイテムをアイテムに移す
	/// </summary>
	/// <param name=""></param>
	/// <returns>すべて移動でtrue</returns>
	bool AllDummyItemAddItems(void);

	//アイテムの値リセット
	void ResetItemValue(void);

	//全アイテムのエフェクトを停止
	void ItemEffectStop(void);

	/// <summary>
	/// ダミーアイテムのY軸回転を取得
	/// </summary>
	/// <param name="playerNum">プレイヤー番号</param>
	/// <returns></returns>
	float GetDummyItemRotY(int playerNum);
	/// <summary>
	/// ダミーアイテムのY軸回転を設定
	/// </summary>
	/// <param name="playerNum">プレイヤー番号</param>
	/// <param name="rotY">設定する値</param>
	void SetDummyItemRotY(int playerNum , float rotY);

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
	~ItemManager(void);
	//アイテムの生成
	std::shared_ptr<ItemBase> CreateItem(ItemBase::ITEM_TYPE type, IntVector3 mapPos, Quaternion rot);
};

