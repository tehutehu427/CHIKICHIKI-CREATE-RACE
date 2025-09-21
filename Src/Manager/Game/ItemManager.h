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

	//アイテムの色関連
	static constexpr float DUMMY_ITEM_OPACITY_RATE = 0.3f;	//ダミーアイテムの半透明度
	static constexpr float DEFAULT_OPACITY_RATE = 1.0f;		//デフォルトの半透明度
	static constexpr COLOR_F DUMMY_OVERLAP_COLOR = { 1.0f,0.1f,0.1f,DUMMY_ITEM_OPACITY_RATE };	//ダミーの重なっているときの色
	static constexpr COLOR_F DUMMY_DEFAULT_COLOR = { 0.1f,0.1f,1.0f,DUMMY_ITEM_OPACITY_RATE };	//ダミーのデフォルトの色
	static constexpr COLOR_F DEFAULT_COLOR = { 1.0f,1.0f,1.0f,DEFAULT_OPACITY_RATE };	//デフォルトの色

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name=""></param>
	void Init(void);
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name=""></param>
	void Update(void);
	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name=""></param>
	void Draw(void);
	/// <summary>
	/// インスタンスの削除
	/// </summary>
	/// <param name=""></param>
	void Destroy(void);

	/// <summary>
	/// アイテム追加
	/// </summary>
	/// <param name="_mapPos">生成させるマップ座標</param>
	/// <param name="_rot">生成させる時の回転情報</param>
	/// <param name="_type">アイテムの種類</param>
	/// <param name="_rotY">Y軸回転</param>
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
	void AllDeleteItem(void);

	/// <summary>
	/// ダミーアイテム追加
	/// </summary>
	/// <param name="_mapPos">生成させるマップ座標</param>
	/// <param name="_rot">生成させる時の回転情報</param>
	/// <param name="_type">アイテムの種類</param>
	/// <param name="_playerNum">プレイヤー番号</param>
	void CreateDummyItem(IntVector3 _mapPos, Quaternion _rot, ItemBase::ITEM_TYPE _type, int _playerNum);

	/// <summary>
	/// ダミーアイテムのステータスを取得
	/// </summary>
	/// <param name="_playerNum">プレイヤー番号</param>
	/// <returns>ダミーアイテムのステータス</returns>
	ItemBase::Status GetDummyItemStatus(int _playerNum);

	/// <summary>
	/// ダミーアイテムのマップ座標を取得
	/// </summary>
	/// <param name="_playerNum">プレイヤー番号</param>
	/// <returns>ダミーアイテムのマップ座標</returns>
	IntVector3 GetDummyItemMapPos(int _playerNum);

	/// <summary>
	/// ダミーアイテムの当たり判定サイズを取得
	/// </summary>
	/// <param name="_playerNum">プレイヤー番号</param>
	/// <returns>ダミーアイテムのサイズ</returns>
	IntVector3 GetDummyItemHitSize(int _playerNum);

	/// <summary>
	/// ダミーアイテムのサイズを取得
	/// </summary>
	/// <param name="_playerNum">プレイヤー番号</param>
	/// <returns>ダミーアイテムのサイズ</returns>
	IntVector3 GetDummyItemSize(int _playerNum);

	/// <summary>
	/// ダミーアイテムのTransformを取得
	/// </summary>
	/// <param name="_playerNum">プレイヤー番号</param>
	/// <returns>ダミーアイテムのTransform</returns>
	const Transform& GetDummyItemTransform(int _playerNum);

	/// <summary>
	/// ダミーアイテムの座標設定
	/// </summary>
	/// <param name="_mapPos">マップ座標</param>
	/// <param name="_playerNum">プレイヤー番号</param>
	void DummyItemSetMapPos(IntVector3 _mapPos, int _playerNum);

	/// <summary>
	/// ダミーアイテムの回転を設定
	/// </summary>
	/// <param name="_rot">回転量</param>
	/// <param name="_playerNum">プレイヤー番号</param>
	void DummyItemSetRotate(Quaternion _rot, int _playerNum);

	/// <summary>
	/// ダミーアイテムをアイテムに追加
	/// </summary>
	/// <param name="_playerNum">プレイヤー番号</param>
	void DummyItemAddItems(int _playerNum);

	/// <summary>
	/// 明示的にインステンスを生成する
	/// </summary>
	/// <param name=""></param>
	static void CreateInstance(void);

	/// <summary>
	/// 静的インスタンスの取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>インスタンスを返す</returns>
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
	/// <param name="_playerNum">プレイヤー番号</param>
	/// <returns>成功ならtrue 失敗ならfalse</returns>
	bool ItemsAddDummyItems(ItemBase::ITEM_TYPE _type, IntVector3 _mapPos , int _playerNum);

	/// <summary>
	/// 特定のプレイヤーのダミーアイテムを削除
	/// </summary>
	/// <param name="_playerNum">プレイヤー番号</param>
	void DeleteDummyItem(int _playerNum);

	/// <summary>
	/// ダミーアイテムが存在するか
	/// </summary>
	/// <param name="_playerNum">プレイヤー番号</param>
	/// <returns>存在するtrue 存在しないfalse</returns>
	bool IsDummyItem(int _playerNum);

	/// <summary>
	/// アイテムのtransformを取得
	/// </summary>
	/// <param name="_mapPos">マップ座標</param>
	/// <param name="_type">アイテムの種類</param>
	/// <returns>アイテムのtransform</returns>
	const Transform& GetItemTransform(IntVector3 _mapPos , ItemBase::ITEM_TYPE _type) const;

	/// <summary>
	/// アイテムの当たり判定大きさを取得
	/// </summary>
	/// <param name="_type">アイテムの種類</param>
	/// <returns>当たり判定の大きさ</returns>
	IntVector3 GetItemHitSize(ItemBase::ITEM_TYPE _type) const;

	/// <summary>
	/// アイテムの大きさを取得
	/// </summary>
	/// <param name="_type">アイテムの種類</param>
	/// <returns>大きさ</returns>
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
	/// プレイヤーのスタートワールド座標を返す
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

	/// <summary>
	/// アイテムの値リセット
	/// </summary>
	/// <param name=""></param>
	void ResetItemValue(void);

	/// <summary>
	/// 全アイテムのエフェクトを停止
	/// </summary>
	/// <param name=""></param>
	void ItemEffectStop(void);

	/// <summary>
	/// ダミーアイテムのY軸回転を取得
	/// </summary>
	/// <param name="_playerNum">プレイヤー番号</param>
	/// <returns>y軸回転量を返す</returns>
	float GetDummyItemRotY(int _playerNum);
	/// <summary>
	/// ダミーアイテムのY軸回転を設定
	/// </summary>
	/// <param name="_playerNum">プレイヤー番号</param>
	/// <param name="_rotY">設定する値</param>
	void SetDummyItemRotY(int _playerNum , float _rotY);

	/// <summary>
	/// スタートオブジェクトの位置
	/// </summary>
	/// <param name=""></param>
	/// <returns>スタートのワールド座標</returns>
	const VECTOR GetStartObjectPos(void);

	/// <summary>
	/// ゴールオブジェクトの位置
	/// </summary>
	/// <param name=""></param>
	/// <returns>ゴールのワールド座標</returns>
	const VECTOR GetGoalObjectPos(void);

protected:

private:

	//静的インスタンス
	static ItemManager* instance_;

	//種類ごとにアイテムを管理
	std::map<ItemBase::ITEM_TYPE, std::vector<std::shared_ptr<ItemBase>>> items_;
	//配置中のアイテム[プレイヤー番号][アイテムの種類]
	std::map<int,std::shared_ptr<ItemBase>> dummyItems_;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name=""></param>
	ItemManager(void);
	//コピーコンストラクタ及び代入演算の禁止
	ItemManager(const ItemManager& instance_) = delete;
	void operator= (const ItemManager& instance_) = delete;
	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~ItemManager(void);

	/// <summary>
	/// アイテムの生成
	/// </summary>
	/// <param name="_type">アイテムの種類</param>
	/// <param name="_mapPos">マップ座標</param>
	/// <param name="_rot">回転量</param>
	/// <returns>アイテムのポインター</returns>
	std::shared_ptr<ItemBase> CreateItem(ItemBase::ITEM_TYPE _type, IntVector3 _mapPos, Quaternion _rot);
};

