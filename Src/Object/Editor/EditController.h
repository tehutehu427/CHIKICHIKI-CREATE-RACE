#pragma once
#include <DxLib.h>
#include <functional>
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
	static constexpr int ERROR_PAD_VIBE_POW = 400;		//エラー時のパッドバイブの強さ
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
	static constexpr VECTOR UP_CAMERA_POS = { MapEditer::MAP_SIZE.x / 2.0f * MapEditer::GRID_SIZE,3500.0f,MapEditer::MAP_SIZE.z / 2.0f * MapEditer::GRID_SIZE };	//上固定カメラの位置

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

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_playerNum">プレイヤー番号</param>
	EditController(int _playerNum);
	/// <summary>
	/// デストラクタ
	/// </summary>
	~EditController(void);

	/// <summary>
	/// 初期化処理
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
	/// UI描画処理
	/// </summary>
	/// <param name=""></param>
	void DrawUI(void);	

	/// <summary>
	/// リセットする
	/// </summary>
	/// <param name=""></param>
	void Reset(void);	
	/// <summary>
	/// モードを変更する
	/// </summary>
	/// <param name="_mode">変更するモード</param>
	void ChangeMode(MODE _mode);
	/// <summary>
	/// アイテム設定
	/// </summary>
	/// <param name="_itemType">アイテムの種類</param>
	void SetItemType(ItemBase::ITEM_TYPE _itemType);

	/// <summary>
	/// カーソル位置取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>カーソル位置</returns>
	Vector2 GetCursorPos(void) const { return cursorPos_; }	

	/// <summary>
	/// マルチ時にアイテムを置き終わったか
	/// </summary>
	/// <param name=""></param>
	/// <returns>置き終わっていたらtrue</returns>
	bool GetReady(void) const;	

	/// <summary>
	/// パッド番号取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>パッド番号</returns>
	KeyConfig::JOYPAD_NO GetPadNum(void) const { return padNum_; }

	/// <summary>
	/// スクリーンサイズ取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>スクリーンの大きさを取得</returns>
	Vector2 GetScreenSize(void) const { return screenSize_; }

	/// <summary>
	/// マルチ時に準備完了の処理
	/// </summary>
	/// <param name=""></param>
	void SetReady(void);

	/// <summary>
	/// カーソル更新
	/// </summary>
	/// <param name=""></param>
	void UpdateCursor(void);

	/// <summary>
	/// エラー関係の更新
	/// </summary>
	/// <param name=""></param>
	void UpdateError(void);

	/// <summary>
	/// エラーかどうか
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	int IsError(void) const;

	/// <summary>
	/// エラーの種類を設定
	/// </summary>
	/// <param name="_errorType">エラーの種類</param>
	void SetError(int _errorType);

protected:

private:

	//マルチ時にアイテムを置き終わったか
	std::unique_ptr<EditItemReady> ready_;
	//エラーの種類
	ERROR_TYPE errorType_;
	//エラー文字列の表示時間
	float errorStringTime_;	
	//プレイヤー番号
	int playerNum_;
	//プレイヤーの最大数
	int playerMaxNum_;	
	//パッド番号
	KeyConfig::JOYPAD_NO padNum_;	

	//スクリーンサイズ
	Vector2 screenSize_;	

	//座標関係
	Vector2 mousePos_;	//2Dのマウス座標
	Vector2 cursorPos_;	//2Dのカーソル座標
	IntVector3 mapPos_;	//3Dのマップ座標
	IntVector3 initMapPos_;	//初期マップ座標

	//カメラのモード
	CAMERA_MODE cameraMode_;	

	//モード
	MODE mode_;	
	//アイテムの種類
	ItemBase::ITEM_TYPE itemType_;	
	//オブジェクトをクリックしたか
	bool isClickObject_;	
	//移動方向
	MOVE_DIR moveDir_;	
	//マウスがオブジェクトに当たった座標
	IntVector3 mapPosObject_; 

	//カメラ関係
	VECTOR cPos_;		//カメラの座標
	VECTOR cAngles_;	//カメラの回転
	VECTOR cTargetPos_;	//カメラの注視点

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

	int IsChangeMoveDir(void) const;	//移動方向を変えていいか -1 :NONE　0: 元のまま 1:変える
	void DebugUpdate(void);	//デバッグ用更新
	void DebugDraw(void) const;	//デバッグ用描画

	void RotateObject(void) const;	//オブジェクト回転

	void DeleteItems(IntVector3 _mapPos, IntVector3 _size, IntVector3 _hitSize, float _rotY) const;	//範囲内のアイテムを削除

	bool IsChangeVecDir(const VECTOR _vec1, const VECTOR _vec2) const;	//ベクトルの方向が変わったか　変わったら　true

	void DrawXArrow(VECTOR _worldPos, bool _isBig);	//X方向の矢印を描画
	void DrawYArrow(VECTOR _worldPos, bool _isBig);	//Y方向の矢印を描画
	void DrawZArrow(VECTOR _worldPos, bool _isBig);	//Z方向の矢印を描画

	void SetCameraPosToDummyObject(void) const;	//ダミーオブジェクトにカメラを近づける

	void ChangeCameraMode(void);	//カメラのモードを変える
	void ChangeCameraMode(CAMERA_MODE _mode);	//カメラのモードを変える
};

