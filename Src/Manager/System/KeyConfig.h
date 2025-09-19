#pragma once
#include <vector>
#include <map>
#include <memory>
#include "../Common/Vector2.h"

class InputManager;

class KeyConfig
{
public:

	// ゲームコントローラーの認識番号
	// DxLib定数、DX_INPUT_PAD1等に対応
	enum class JOYPAD_NO
	{
		KEY_PAD1,			// キー入力とパッド１入力
		PAD1,				// パッド１入力
		PAD2,				// パッド２入力
		PAD3,				// パッド３入力
		PAD4,				// パッド４入力
		INPUT_KEY = 4096	// キー入力
	};

	enum class TYPE
	{
		KEYBORD_MOUSE,
		PAD,
		ALL,
	};


	// ゲームコントローラーボタン
	enum class JOYPAD_BTN
	{
		RIGHTBUTTON_LEFT = 0,	// X
		RIGHTBUTTON_RIGHT,		// B
		RIGHTBUTTON_TOP,		// Y
		RIGHTBUTTON_DOWN,		// A
		R_TRIGGER,				// R_TRIGGER
		L_TRIGGER,				// L_TRIGGER
		R_BUTTON,				// R_BUTTON
		L_BUTTON,				// L_BUTTON
		START_BUTTON,			// START_BUTTON
		SELECT_BUTTON,			// SELECT_BUTTON
		LEFTBUTTON_TOP,			// 上
		LEFTBUTTON_DOWN,		// 下
		LEFTBUTTON_LEFT,		// 左
		LEFTBUTTON_RIGHT,		// 右
		LEFT_STICK,				// 左スティック押し込み
		RIGHT_STICK,			// 右スティック押し込み
		MAX
	};

	enum class JOYPAD_STICK
	{
		L_STICK_UP,		//左スティック上
		L_STICK_DOWN,	//左スティック下
		L_STICK_LEFT,	//左スティック左
		L_STICK_RIGHT,	//左スティック右
		R_STICK_UP,		//右スティック上
		R_STICK_DOWN,	//右スティック下
		R_STICK_LEFT,	//右スティック左
		R_STICK_RIGHT,	//右スティック右
		MAX
	};

	enum class MOUSE
	{
		CLICK_RIGHT,		//右クリック
		CLICK_LEFT,			//左クリック
		MOVE_LEFT,			//左移動
		MOVE_RIGHT,			//右移動
		MOVE_UP,			//上移動
		MOVE_DOWN,			//下移動
		WHEEL_FRONT,		//ホイール前(奥)回転
		WHEEL_BACK,			//ホイール後ろ(手前)回転
		MAX
	};

	enum class CONTROL_TYPE //操作の種類	
	{
		ENTER,					//決定
		DECISION_KEY_AND_PAD,	//決定(PAD or KEYBOARDD onry)
		DECISION_MOUSE,			//決定(MOUSE)
		CANCEL,					//キャンセル

		PHASE_CHANGE,			//フェーズ変更
		PHASE_CHANGE_CHECK,		//フェーズ変更

		IMPORT_FILE,			//ファイル入力
		IMPORT_FILE_CLICK,		//ファイル入力クリック
		EXPORT_FILE,			//ファイル出力
		EXPORT_FILE_CLICK,		//ファイル出力クリック

		EDIT_ESCAPE,			//エディットをやめる
		EDIT_ESCAPE_CLICK,		//エディットをやめる時のクリック用

		SELECT_UP,				//選択　上
		SELECT_DOWN,			//選択　下
		SELECT_LEFT,			//選択　左
		SELECT_RIGHT,			//選択　右

		MANUAL,					//マニュアルを開く
		MANUAL_ICON_CLICK,		//マニュアルをクリックで開く

		PALETTE_CURSOR_UP,		//パレット時のカーソル上移動
		PALETTE_CURSOR_DOWN,	//パレット時のカーソル下移動
		PALETTE_CURSOR_LEFT,	//パレット時のカーソル左移動
		PALETTE_CURSOR_RIGHT,	//パレット時のカーソル右移動
		PALETTE_CURSOR_SELECT,	//パレット時のカーソル選択

		EDIT_CAMERA_ROT_UP,		//エディット時のカメラ回転上
		EDIT_CAMERA_ROT_DOWN,	//エディット時のカメラ回転下
		EDIT_CAMERA_ROT_RIGHT,	//エディット時のカメラ回転右
		EDIT_CAMERA_ROT_LEFT,	//エディット時のカメラ回転左
		EDIT_CAMERA_MOVE_FRONT,	//エディット時のカメラ移動前
		EDIT_CAMERA_MOVE_BACK,	//エディット時のカメラ移動後ろ
		EDIT_CAMERA_MOVE_LEFT,	//エディット時のカメラ移動左
		EDIT_CAMERA_MOVE_RIGHT,	//エディット時のカメラ移動右
		EDIT_CAMERA_MOVE_UP,	//エディット時のカメラ移動上
		EDIT_CAMERA_MOVE_DOWN,	//エディット時のカメラ移動下
		EDIT_CAMERA_CHANGE,		//エディット時のカメラを変更する

		EDIT_ITEM_SELECT,		//エディット時のアイテム選択
		EDIT_ITEM_ROTATE,		//エディット時のアイテム回転
		EDIT_ITEM_DELETE,		//エディット時のアイテム削除

		EDIT_GRID_ON_OFF,		//エディット時のグリッドのオンオフ

		CURSOR_SPEED_UP,		//カーソルの移動速度アップ

		PLAYER_JUMP,			//プレイヤーのジャンプ
		PLAYER_PUNCH,			//プレイヤーのパンチ
		PLAYER_DASH,
		PLAYER_MOVE_FRONT,		//プレイヤーの移動前
		PLAYER_MOVE_BACK,		//プレイヤーの移動後ろ
		PLAYER_MOVE_RIGHT,		//プレイヤーの移動右
		PLAYER_MOVE_LEFT,		//プレイヤーの移動左

		PLAY_CAMERA_MOVE_UP,	//プレイのカメラ移動上
		PLAY_CAMERA_MOVE_DOWN,	//プレイのカメラ移動下
		PLAY_CAMERA_MOVE_RIGHT,	//プレイのカメラ移動右
		PLAY_CAMERA_MOVE_LEFT,	//プレイのカメラ移動左
		PLAY_CAMERA_ZOOM_IN,	//プレイのカメラズームイン
		PLAY_CAMERA_ZOOM_OUT,	//プレイのカメラズームアウト

		DEMO_TO_TITLE_BACK,		//デモからタイトルに戻る

		INPUT_CHECK,			//マルチ時の入力確認

		DEBUG_CHANGE_TITLE,		//デバッグ用タイトル画面に戻る
		DEBUG_CHANGE_CLEAR,		//デバッグ用クリア画面に戻る
		DEBUG_CHANGE_INPUT,			//デバッグ用入力デバイス切り替え
		DATA_INPUT,				//データを入力
		DATA_EXPORT,			//データを出力
		SELECT_SKIP,
		MAX,
	};

	/// <summary>
	/// インスタンスを明示的に生成
	/// </summary>
	/// <param name=""></param>
	static void CreateInstance(void);

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>インスタンス</returns>
	static KeyConfig& GetInstance(void);

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
	/// 操作の種類別にキーの状態を取得
	/// </summary>
	/// <param name="_cType">操作の種類</param>
	/// <param name="_no">パッド番号</param>
	/// <param name="_type">入力デバイスの選択</param>
	/// <returns>押されていたらtrue</returns>
	bool IsNew(CONTROL_TYPE _cType, KeyConfig::JOYPAD_NO _no ,TYPE _type = TYPE::ALL);
	/// <summary>
	/// 操作の種類別にキーの状態を取得
	/// </summary>
	/// <param name="_cType">操作の種類</param>
	/// <param name="_no">パッド番号</param>
	/// <param name="_type">入力デバイスの選択</param>
	/// <returns>押された瞬間ならtrue</returns>
	bool IsTrgDown(CONTROL_TYPE _cType, KeyConfig::JOYPAD_NO _no,TYPE _type = TYPE::ALL);
	/// <summary>
	/// 操作の種類別にキーの状態を取得
	/// </summary>
	/// <param name="_cType">操作の種類</param>
	/// <param name="_no">パッド番号</param>
	/// <param name="_type">入力デバイスの選択</param>
	/// <returns>離された瞬間ならtrue</returns>
	bool IsTrgUp(CONTROL_TYPE _cType, KeyConfig::JOYPAD_NO _no, TYPE _type = TYPE::ALL);

	//操作の種類別にキーを追加
	/// <summary>
	/// キーボードのキーと操作の種類を追加
	/// </summary>
	/// <param name="_type">操作の種類</param>
	/// <param name="_key">キーボードのキー</param>
	void Add(CONTROL_TYPE _type, int _key);
	/// <summary>
	/// パッドのボタンと操作の種類を追加
	/// </summary>
	/// <param name="_type">操作の種類</param>
	/// <param name="_key">パッドのボタン</param>
	void Add(CONTROL_TYPE _type, JOYPAD_BTN _key);
	/// <summary>
	/// パッドのスティックと操作の種類を追加
	/// </summary>
	/// <param name="_type">操作の種類</param>
	/// <param name="_key">パッドのスティック</param>
	void Add(CONTROL_TYPE _type, JOYPAD_STICK _key);
	/// <summary>
	/// マウスと操作の種類を追加
	/// </summary>
	/// <param name="_type">操作の種類</param>
	/// <param name="_key">マウス</param>
	void Add(CONTROL_TYPE _type, MOUSE _key);

	/// <summary>
	/// マウス座標の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>マウス座標</returns>
	Vector2 GetMousePos(void) const;
	/// <summary>
	/// マウスの移動量を取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>マウスの移動量</returns>
	Vector2 GetMouseMove(void) const;

	/// <summary>
	/// マウスの座標を設定
	/// </summary>
	/// <param name=""></param>
	void SetMousePosScreen(void);

	/// <summary>
	/// マウスの座標を設定
	/// </summary>
	/// <param name="_pos">設定する座標</param>
	void SetMousePos(const Vector2& _pos);

	//上を0.0度として角度を渡す
	/// <summary>
	/// 左スティックの倒れた角度を取得
	/// </summary>
	/// <param name="_no">パッド番号</param>
	/// <returns>角度</returns>
	float GetLStickDeg(KeyConfig::JOYPAD_NO _no) const;
	/// <summary>
	/// 右スティックの倒れた角度を取得
	/// </summary>
	/// <param name="_no">パッド番号</param>
	/// <returns>角度</returns>
	float GetRStickDeg(KeyConfig::JOYPAD_NO _no) const;

	/// <summary>
	/// 左スティックの倒れた度合いを取得
	/// </summary>
	/// <param name="_no">パッド番号</param>
	/// <returns>倒れ具合を渡す</returns>
	Vector2 GetKnockLStickSize(KeyConfig::JOYPAD_NO _no) const;
	/// <summary>
	/// 右スティックの倒れた度合いを取得
	/// </summary>
	/// <param name="_no">パッド番号</param>
	/// <returns>倒れた具合を渡す</returns>
	Vector2 GetKnockRStickSize(KeyConfig::JOYPAD_NO _no) const;

	/// <summary>
	/// 指定の方向に倒れた度合い0から1000
	/// </summary>
	/// <param name="_no">パッド番号</param>
	/// <param name="_stick">スティック方向</param>
	/// <returns>倒れ度合い</returns>
	int PadStickOverSize(KeyConfig::JOYPAD_NO _no, KeyConfig::JOYPAD_STICK _stick)const;

	/// <summary>
	/// リソースの破棄
	/// </summary>
	/// <param name=""></param>
	void Destroy(void);
	/// <summary>
	/// パッドを振動する
	/// </summary>
	/// <param name="_no">振動させるPAD番号</param>
	/// <param name="_time">ミリ秒　,-1で無限に続けることができる(STOP 必須)</param>
	/// <param name="_pow">1～1000</param>
	void PadVibration(KeyConfig::JOYPAD_NO _no, int _time, int _pow);
	/// <summary>
	/// 振動を止める
	/// </summary>
	/// <param name="_no">パッド番号</param>
	void StopPadVibration(KeyConfig::JOYPAD_NO _no);

private:

	//入力管理クラスのインスタンス
	std::unique_ptr<InputManager> inputManager_;	

	// シングルトン用インスタンス
	static KeyConfig* instance_;

	// 操作の種類ごとにキーの状態を格納する配列
	std::map<CONTROL_TYPE, std::vector<int>>keyInput_;								//操作の種類とキーの種類でキーボードの状態を格納
	std::map<CONTROL_TYPE, std::vector<JOYPAD_BTN>>conInput_;			//操作の種類とボタンの種類でコントローラーの状態を格納
	std::map<CONTROL_TYPE, std::vector<JOYPAD_STICK>>stickInput_;		//操作の種類とスティックの種類でコントローラーの状態を格納
	std::map < CONTROL_TYPE, std::vector<MOUSE>>mouseInput_;			//操作の種類とマウスの種類でマウスの状態を格納



	KeyConfig(void);
	KeyConfig(const KeyConfig& _manager);
	~KeyConfig(void) = default;
};

