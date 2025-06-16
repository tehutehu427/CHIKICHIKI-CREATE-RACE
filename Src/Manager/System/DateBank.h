#pragma once
#include <string>
#include "../Object/Item/ItemBase.h"

class DateBank
{
public:

	/// <summary>
	/// 操作種類
	/// </summary>
	enum class TYPE
	{
		KEY_BORD,	//キーボード操作		ソロ用
		CONTROLLER,	//コントローラー操作	マルチ用
		ERORE		//プレイヤー人数がerore
	};

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
	static DateBank& GetInstance(void);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name=""></param>
	void Init(void);

	/// <summary>
	/// 解放処理
	/// </summary>
	/// <param name=""></param>
	void Destroy(void);

	/// <summary>
	/// プレイヤー人数を設定
	/// </summary>
	/// <param name="num">プレイヤー人数</param>
	/// <returns>正しく設定できればtrue,問題が起きればfalse</returns>
	bool SetPlayerNum(int _num);

	/// <summary>
	/// プレイヤー人数を返す
	/// </summary>
	/// <param name=""></param>
	/// <returns>プレイヤー人数</returns>
	int GetPlayerNum(void);

	/// <summary>
	/// 操作種類を返す
	/// </summary>
	/// <param name=""></param>
	/// <returns>操作種類</returns>
	TYPE GetType(void);

	/// <summary>
	/// ステージの番号を設定
	/// </summary>
	/// <param name="_stageNo">ステージ番号</param>
	/// <returns></returns>
	inline const void SetStageNo(const int _stageNo) { stageNo_ = _stageNo; }

	/// <summary>
	/// ステージの番号を返す
	/// </summary>
	/// <returns>ステージ番号</returns>
	inline const int GetStageNo()const { return stageNo_; }

	/// <summary>
	/// アイテムの名前を返す
	/// </summary>
	/// <param name="_type">アイテムの種類</param>
	/// <returns>アイテムネーム</returns>
	inline const std::string GetItemName(const ItemBase::ITEM_TYPE _type)const { return itemNames_[static_cast<int>(_type)]; }

private:
	static DateBank* instance_;
	DateBank(void);
	DateBank(const DateBank& instance_) = default;
	~DateBank(void);
	int playerNum_;		//プレイヤーの人数
	int maxPlayerNum_;	//コントローラー数よりプレイヤー人数が多くならないようにする
	int stageNo_;		//ステージ番号

	//種類ごとの名前
	std::string itemNames_[static_cast<int>(ItemBase::ITEM_TYPE::MAX)];
};

