#pragma once
#include <string>
#include "../Object/Item/ItemBase.h"

class DateBank
{
public:
	enum class TYPE
	{
		KEY_BORD,	//キーボード操作		ソロ用
		CONTROLLER,	//コントローラー操作	マルチ用
		ERORE		//プレイヤー人数がerore
	};


	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static DateBank& GetInstance(void);

	void Init(void);

	void Destroy(void);

	bool SetPlayerNum(int num);

	int GetPlayerNum(void);

	TYPE GetType(void);

	//名前を返す
	inline const std::string GetItemName(const ItemBase::ITEM_TYPE _type)const { return itemNames_[static_cast<int>(_type)]; }

protected:

private:
	static DateBank* instance_;
	DateBank(void);
	DateBank(const DateBank& instance_) = default;
	~DateBank(void) = default;
	int playerNum_;		//プレイヤーの人数
	int maxPlayerNum_;	//コントローラー数よりプレイヤー人数が多くならないようにする

	//種類ごとの名前
	std::string itemNames_[static_cast<int>(ItemBase::ITEM_TYPE::MAX)];
};

