#pragma once
#include<DxLib.h>
#include<functional>
#include "../../Common/Vector2.h"
#include "../../Common/IntVector3.h"
#include "../../Object/Item/ItemBase.h"
#include "../../Manager/System/KeyConfig.h"

class EditItemReady;

class EditController
{
public:
	static constexpr float PAD_STICK_RATE = 0.0251f;	//パッドスティックの感度
	static constexpr float MOVE_ARROW_LENGTH = 75.0f;	//移動矢印の長さ
	static constexpr float MOVE_ARROW_SIZE = 10.0f;		//移動矢印の先端の大きさ
	static constexpr int DELAY_MOVE_ARROW = 20;			//移動矢印の先端からの猶予座標
	static constexpr IntVector3 ERROR_POS = { -1,-1,-1 };	//生成不可の座標
	static constexpr IntVector3 PLAYER1_INIT_EDIT_POS = { 5,0,0 };	//プレイヤー1のエディター初期座標
	static constexpr IntVector3 PLAYER2_INIT_EDIT_POS = { 10,0,0 };	//プレイヤー1のエディター初期座標
	static constexpr IntVector3 PLAYER3_INIT_EDIT_POS = { 15,0,0 };	//プレイヤー1のエディター初期座標
	static constexpr IntVector3 PLAYER4_INIT_EDIT_POS = { 20,0,0 };	//プレイヤー1のエディター初期座標

	enum class MODE
	{
		ITEM_SELECT,	//アイテム選択
		MOVE_ROTATE,	//オブジェクト移動,回転
		MAX
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
	EditController(int playerNum);
	//デストラクタ
	~EditController();

	void Init(void);		//初期化
	void Update(void);		//更新
	void Draw(void);		//描画

	void DrawUI(void);	//UI描画

	void Reset(void);	//リセット
	//モード変更
	void ChengeMode(MODE mode);
	//アイテム設定
	void SetItemType(ItemBase::ITEM_TYPE itemType);

	Vector2 GetCursorPos(void) const { return cursorPos_; }	//カーソル位置取得

	bool GetReady(void) const;	//マルチ時にアイテムを置き終わったか

	KeyConfig::JOYPAD_NO GetPadNum(void) const { return padNum_; }	//パッド番号取得

	Vector2 GetScreenSize(void) const { return screenSize_; }	//スクリーンサイズ取得
protected:

private:

	std::unique_ptr<EditItemReady> ready_;	//マルチ時にアイテムを置き終わったか

	int playerNum_;	//プレイヤー番号
	int playerMaxNum_;	//プレイヤーの最大数
	KeyConfig::JOYPAD_NO padNum_;	//パッド番号

	Vector2 screenSize_;	//スクリーンサイズ

	Vector2 mousePos_;	//2Dのマウス座標
	Vector2 cursorPos_;	//2Dのカーソル座標
	IntVector3 mapPos_;	//3Dのマップ座標
	IntVector3 initMapPos_;	//初期マップ座標

	MODE mode_;	//モード
	ItemBase::ITEM_TYPE itemType_;	//アイテムの種類
	bool isClickObject_;	//オブジェクトをクリックしたか
	MOVE_DIR moveDir_;	//移動方向
	IntVector3 mapPosObject_; //マウスがオブジェクトに当たった座標
	//モード管理(遷移時の初期処理)
	std::map<MODE, std::function<void(void)>> modeChanges_;

	//モード管理(更新ステップ)
	std::function<void(void)> modeUpdate_;

	//モード管理(更新ステップ)
	std::function<void(void)> modeDraw_;

	//モード別変更処理
	void ChengeModeItemSelect(void);
	void ChengeModeMove(void);

	//モード別更新処理
	void ItemSelectUpdate(void);		//アイテム選択
	void MoveRotateObjectUpdate(void);	//オブジェクト移動回転

	//モード別描画処理
	void ItemSelectDraw(void);			//アイテム選択
	void MoveRotateObjectDraw(void);	//オブジェクト移動回転

	//アイテム選択解除
	void ItemNotSelect(void);		
	
	//近くのオブジェクトの直前座標を取得
	IntVector3 NearObjectFrontPos(void);	
	
	//移動するアイテムの座標を取得
	void  MoveItem(void);			

	//移動方向を取得
	MOVE_DIR GetMoveDir(void) const;			

	void DebugUpdate(void);	//デバッグ用更新
	void DebugDraw(void);	//デバッグ用描画

	void RotateObject(void) const;	//オブジェクト回転

	void DeleteItems(IntVector3 _mapPos, IntVector3 _size, IntVector3 _hitSize, float _rotY);	//範囲内のアイテムを削除
};

