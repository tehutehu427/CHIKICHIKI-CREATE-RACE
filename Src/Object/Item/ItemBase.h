#pragma once
#include "../ObjectBase.h"
class ItemBase : public ObjectBase
{
public:
	enum class ITEM_TYPE	//アイテムの種類
	{
		NONE,
		START,				//スタート
		GOAL,				//ゴール
		FLOOR,				//床
		MOVE_HORI_FLOOR,	//水平(横)方向移動床
		MOVE_VER_FLOOT,		//垂直(縦)方向移動床
		FENCE,				//柵
		CANNON,				//大砲
		SPIKY,				//とげ
		BOMB_SMALL,			//爆弾(小)
		BOMB_BIG,			//爆弾(大)
	};

	//コンストラクタ
	ItemBase();
	//デストラクタ
	virtual ~ItemBase()override;

	//読み込み
	virtual void Load(void)override = 0;
	//初期化
	virtual void Init(void)override = 0;
	//更新
	virtual void Update(void)override = 0;
	//描画
	virtual void Draw(void)override = 0;
	//解放
	virtual void Release(void)override = 0;

	//アイテムの種類取得
	inline const ITEM_TYPE GetItemType(void)const { return type_; }

protected:

	//アイテムの種類
	ITEM_TYPE type_;

private:
};

