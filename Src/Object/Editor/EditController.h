#pragma once
#include<DxLib.h>
#include<functional>
#include "../../Common/Vector2.h"
#include "../../Common/IntVector3.h"
#include "../../Object/Item/ItemBase.h"

class EditController
{
public:
	static constexpr float MOVE_ARROW_LENGTH = 50.0f;	//移動矢印の長さ
	static constexpr float MOVE_ARROW_SIZE = 10.0f;		//移動矢印の先端の大きさ
	static constexpr int DELAY_MOVE_ARROW = 30;			//移動矢印の先端からの猶予座標

	enum class MODE
	{
		ITEM_SELECT,	//アイテム選択
		MOVE,			//オブジェクト移動
		ROTATE,			//オブジェクト回転
	};

	enum class MOVE_DIR
	{
		NONE,	//移動しない
		X,		//X軸
		Y,		//Y軸
		Z,		//Z軸
		XY,		//XY平面
		XZ,		//XZ平面
		YZ,		//YZ平面
	};

	//コンストラクタ
	EditController();

	//デストラクタ
	~EditController() = default;

	void Init(void);		//初期化
	void Update(void);		//更新
	void Draw(void);		//描画

	//モード変更
	void ChengeMode(MODE mode);
	void SetItemType(ItemBase::ITEM_TYPE itemType);
protected:

private:

	Vector2 mousePos_;	//2Dのマウス座標
	IntVector3 mapPos_;	//3Dのマップ座標
	MODE mode_;	//モード
	ItemBase::ITEM_TYPE itemType_;	//アイテムの種類
	bool isClickObject_;	//オブジェクトをクリックしたか
	MOVE_DIR moveDir_;	//移動方向

	//モード管理(遷移時の初期処理)
	std::map<MODE, std::function<void(void)>> modeChanges_;

	//モード管理(更新ステップ)
	std::function<void(void)> modeUpdate_;

	//モード管理(更新ステップ)
	std::function<void(void)> modeDraw_;

	//モード別変更処理
	void ChengeModeItemSelect(void);
	void ChengeModeMove(void);
	void ChengeModeRotate(void);

	//モード別更新処理
	void ItemSelectUpdate(void);	//アイテム選択
	void MoveObjectUpdate(void);	//オブジェクト移動
	void RotateObjectUpdate(void);	//オブジェクト回転

	//モード別描画処理
	void ItemSelectDraw(void);		//アイテム選択
	void MoveObjectDraw(void);		//オブジェクト移動
	void RotateObjectDraw(void);	//オブジェクト回転

	//アイテム選択解除
	void ItemNotSelect(void);		
	
	//近くのオブジェクトの座標を取得
	IntVector3 NearObjectPos(void);	
	
	//移動方向を取得
	MOVE_DIR GetMoveDir(void);			

};

