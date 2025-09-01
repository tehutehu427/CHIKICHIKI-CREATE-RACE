#pragma once
#include<DxLib.h>
#include<functional>
#include "../../Common/Vector2.h"
#include "../../Common/IntVector3.h"
#include "../../Object/Item/ItemBase.h"
#include "../../Manager/System/KeyConfig.h"
#include "../../Manager/Game/MapEditer.h"

class EditItemReady;

class EditController
{
public:
	static constexpr float PAD_STICK_RATE = 0.0125f;	//パッドスティックの感度
	static constexpr float PAD_STICK_RATE_UP = 2.0f;	//パッドスティックの感度倍率
	static constexpr int ERROR_PAD_VIBE_POW = 400.0f;		//エラー時のパッドバイブの強さ
	static constexpr float MOVE_ARROW_LENGTH = 75.0f;	//移動矢印の長さ
	static constexpr float MOVE_ARROW_RADIUS = 15.0f;		//移動矢印の先端の半径
	static constexpr float MOVE_ARROW_SIZE = 60.0f;		//移動矢印の先端の長さ

	static constexpr float MOVED_ARROW_LENGTH = 100.0f;	//移動時の矢印の長さ
	static constexpr float MOVED_ARROW_RADIUS = 30.0f;		//移動時の矢印の先端の半径
	static constexpr float MOVED_ARROW_SIZE = 90.0f;		//移動時の矢印の先端の長さ

	static constexpr int MOVE_ARROW_VARTEXNUM = 32;	//移動矢印の先端の頂点数
	static constexpr int DELAY_MOVE_ARROW = 30;			//移動矢印の先端からの猶予座標

	static constexpr int CURSOR_LOCAL_POS_X = 16;	//カーソルのローカル座標X
	static constexpr int CURSOR_LOCAL_POS_Y = 16;	//カーソルのローカル座標Y
	static constexpr int CURSOR_POINT = 2;			//カーソルのポイントサイズ
	static constexpr IntVector3 ERROR_POS = { -1,-1,-1 };	//生成不可の座標
	static constexpr IntVector3 PLAYER1_INIT_EDIT_POS = { 5,0,0 };	//プレイヤー1のエディター初期座標
	static constexpr IntVector3 PLAYER2_INIT_EDIT_POS = { 10,0,0 };	//プレイヤー2のエディター初期座標
	static constexpr IntVector3 PLAYER3_INIT_EDIT_POS = { 15,0,0 };	//プレイヤー3のエディター初期座標
	static constexpr IntVector3 PLAYER4_INIT_EDIT_POS = { 20,0,0 };	//プレイヤー4のエディター初期座標

	static constexpr VECTOR UP_CAMERA_TARGET_POS = { 0.0f,-5000.0f,0.0f };	//上固定カメラの位置
	static constexpr VECTOR UP_CAMERA_POS = { MapEditer::MAP_SIZE.x / 2 * MapEditer::GRID_SIZE,3500,MapEditer::MAP_SIZE.z / 2 * MapEditer::GRID_SIZE };	//上固定カメラの位置

	static constexpr float GO_DUMMY_DISTANCE = 700.0f;

	static constexpr float ERROR_STRING_TIME = 1.0f;	//エラー文字列の表示時間

	static constexpr int CENTER_BOX_SIZE = 3;	//中央のボックスのサイズ
	static constexpr int CENTER_CROSS_THICKNESS = 3;
	static constexpr int CENTER_CROSS_OFFSET = 10;

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

	enum class ERROR_TYPE
	{
		NONE,			//エラーなし
		ITEM_RANGE_OUT,	//アイテムが範囲外に出ている
		ITEM_OVER_LAP,	//アイテムが重なっている
		ITEM_NOT_SET,	//アイテムが設置できない場所
	};

	enum class CAMERA_MODE	//エディット時のカメラ
	{
		FREE,				//自由移動
		GO_DUMMY,		//ダミー
		FIXED_UP,			//上固定
		MAX,
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
	void ChangeMode(MODE mode);
	//アイテム設定
	void SetItemType(ItemBase::ITEM_TYPE itemType);

	Vector2 GetCursorPos(void) const { return cursorPos_; }	//カーソル位置取得

	bool GetReady(void) const;	//マルチ時にアイテムを置き終わったか

	KeyConfig::JOYPAD_NO GetPadNum(void) const { return padNum_; }	//パッド番号取得

	Vector2 GetScreenSize(void) const { return screenSize_; }	//スクリーンサイズ取得

	void SetReady(void);	//マルチ時に準備完了の処理

	void UpdateCursor(void);	//カーソル更新

	void UpdateError(void);	//エラー関係の更新

	int IsError(void);	//エラーかどうか

	void SetError(int errorType);	//エラーの種類を設定
protected:

private:

	std::unique_ptr<EditItemReady> ready_;	//マルチ時にアイテムを置き終わったか
	ERROR_TYPE errorType_;	//エラーの種類
	float errorStringTime_;	//エラー文字列の表示時間
	int playerNum_;	//プレイヤー番号
	int playerMaxNum_;	//プレイヤーの最大数
	KeyConfig::JOYPAD_NO padNum_;	//パッド番号

	Vector2 screenSize_;	//スクリーンサイズ

	Vector2 mousePos_;	//2Dのマウス座標
	Vector2 cursorPos_;	//2Dのカーソル座標
	IntVector3 mapPos_;	//3Dのマップ座標
	IntVector3 initMapPos_;	//初期マップ座標

	CAMERA_MODE cameraMode_;	//カメラのモード

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
	void ChangeModeItemSelect(void);
	void ChangeModeMove(void);

	//モード別更新処理
	void ItemSelectUpdate(void);		//アイテム選択
	void MoveRotateObjectUpdate(void);	//オブジェクト移動回転

	//モード別描画処理
	void ItemSelectDraw(void);			//アイテム選択
	void MoveRotateObjectDraw(void);	//オブジェクト移動回転

	//アイテム選択解除
	void ItemNotSelect(void);		
	
	//近くのオブジェクトの直前座標を取得 スクリーン座標を渡す
	IntVector3 NearObjectFrontPos(Vector2 pos);	
	
	//移動するアイテムの座標を取得
	void  MoveItem(void);			

	//移動方向を取得
	MOVE_DIR GetMoveDir(void) ;		//クリックした方向の円錐をもとに算出

	MOVE_DIR GetMoveDirTwo(void);	//カメラの方向をもとに算出

	int IsChangeMoveDir(void);	//移動方向を変えていいか -1 :NONE　0: 元のまま 1:変える
	void DebugUpdate(void);	//デバッグ用更新
	void DebugDraw(void);	//デバッグ用描画

	void RotateObject(void) const;	//オブジェクト回転

	void DeleteItems(IntVector3 _mapPos, IntVector3 _size, IntVector3 _hitSize, float _rotY);	//範囲内のアイテムを削除

	bool IsChangeVecDir(const VECTOR vec1, const VECTOR vec2) const;	//ベクトルの方向が変わったか　変わったら　true

	void DrawXArrow(VECTOR worldPos, bool isBig);	//X方向の矢印を描画
	void DrawYArrow(VECTOR worldPos, bool isBig);	//Y方向の矢印を描画
	void DrawZArrow(VECTOR worldPos, bool isBig);	//Z方向の矢印を描画

	void SetCameraPosToDummyObject(void);	//ダミーオブジェクトにカメラを近づける

	void ChangeCameraMode(void);	//カメラのモードを変える
	void ChangeCameraMode(CAMERA_MODE mode);	//カメラのモードを変える
	VECTOR cPos_;		//カメラの座標
	VECTOR cAngles_;	//カメラの回転
	VECTOR cTargetPos_;	//カメラの中止店
};

