#pragma once
#include "../ObjectBase.h"
class ItemBase : public ObjectBase
{
public:

	//アイテムの種類
	enum class ITEM_TYPE	
	{
		NONE,

		//固定オブジェ
		START,				//スタート
		GOAL,				//ゴール

		//設置オブジェ
		FLOOR,				//床
		MOVE_HORI_FLOOR,	//水平(横)方向移動床
		MOVE_VER_FLOOR,		//垂直(縦)方向移動床
		FENCE,				//柵
		CANNON,				//大砲
		SPIKY,				//とげ
		FAN,				//扇風機
		FERRIS_WHEEL,		//観覧車
		SLIME_FLOOR,		//スライム床
		SPRING,				//ばね(ジャンプ台)

		//破壊オブジェ
		BOMB_SMALL,			//爆弾(小)
		BOMB_BIG,			//爆弾(大)

		MAX,				//最大
	};

	static constexpr int ITEM_NUM_MAX = static_cast<int>(ITEM_TYPE::MAX) - 1;	//アイテムの種類数

	//アイテムの効果
	enum class EFFECT_TYPE
	{
		FIXED,			//固定
		INSTALLATION,	//設置物
		DESTROYER,		//破壊物
	};

	//ステータス
	struct Status
	{
		bool isBreak;				//破壊可能か(true:破壊可能)
		bool isGravity;				//重力影響を受けるか(true:受ける)
		ITEM_TYPE itemType;			//アイテムの種類
		EFFECT_TYPE effType;		//アイテムの効果
	};

	//コンストラクタ
	ItemBase();
	//デストラクタ
	virtual ~ItemBase()override;

	//アイテムの個別設定
	virtual void SetParam(void) = 0;

	//読み込み
	void Load(void)override;
	//初期化
	virtual void Init(IntVector3 _mapPos ,Quaternion _quaRot, ITEM_TYPE _itemType);
	virtual void Init()override;
	//更新
	virtual void Update(void)override = 0;
	//描画
	virtual void Draw(void)override;

	//アイテムのステータス取得
	inline const Status GetStatus(void)const { return status_; }

	//座標の設定
	void SetPos(IntVector3 mapPos);
	void SetRotate(Quaternion rot);
	
	//初期マップ座標の取得
	IntVector3 GetInitMapPos(void)const { return InitMapPos_; }	

protected:

	//共通変数
	Status status_;			//ステータス
	IntVector3 InitMapPos_;	//初期マップ座標

private:
};

