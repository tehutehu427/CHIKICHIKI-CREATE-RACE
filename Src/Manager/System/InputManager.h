#pragma once
#include <map>
#include <vector>
#include <Dxlib.h>
#include "../../Common/Vector2.h"

class InputManager
{

public:

	static constexpr int STICK_THRESHOLD = 300;	//スティックの猶予
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

	// ゲームコントローラータイプ
	// DxLib定数、DX_OTHER等に対応
	enum class JOYPAD_TYPE
	{
		OTHER = 0,
		XBOX_360,
		XBOX_ONE,
		DUAL_SHOCK_4,
		DUAL_SENSE,
		SWITCH_JOY_CON_L,
		SWITCH_JOY_CON_R,
		SWITCH_PRO_CTRL,
		MAX
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

	// ゲームコントローラーの入力情報
	struct JOYPAD_IN_STATE
	{
		unsigned char ButtonsOld[static_cast<int>(JOYPAD_BTN::MAX)];
		unsigned char ButtonsNew[static_cast<int>(JOYPAD_BTN::MAX)];
		bool IsOld[static_cast<int>(JOYPAD_BTN::MAX)];
		bool IsNew[static_cast<int>(JOYPAD_BTN::MAX)];
		bool IsTrgDown[static_cast<int>(JOYPAD_BTN::MAX)];
		bool IsTrgUp[static_cast<int>(JOYPAD_BTN::MAX)];
		int AKeyLX;
		int AKeyLY;
		int AKeyRX;
		int AKeyRY;
	};

	// インスタンスを明示的に生成
	static void CreateInstance(void);

	// インスタンスの取得
	static InputManager& GetInstance(void);

	void Init(void);
	void Update(void);

	// リソースの破棄
	void Destroy(void);

	// 判定を行うキーを追加
	void Add(int key);

	// 判定を行うキーをクリア
	void Clear(void);

	// キーの押下判定
	bool IsNew(int key) const;

	// キーの押下判定(押しっぱなしはNG)
	bool IsTrgDown(int key) const;

	// キーを離した時の判定
	bool IsTrgUp(int key) const;

	// マウス座標の取得
	Vector2 GetMousePos(void) const;

	// マウスのクリック状態を取得(MOUSE_INPUT_LEFT、RIGHT)
	int GetMouse(void) const;

	// コントローラの入力情報を取得する
	JOYPAD_IN_STATE GetJPadInputState(JOYPAD_NO no);

	// ボタンが押された
	bool IsPadBtnNew(JOYPAD_NO no, JOYPAD_BTN btn) const;
	bool IsPadBtnTrgDown(JOYPAD_NO no, JOYPAD_BTN btn) const;
	bool IsPadBtnTrgUp(JOYPAD_NO no, JOYPAD_BTN btn) const;

	// スティックが倒されたか
	bool IsStickNew(JOYPAD_NO no, JOYPAD_STICK stick) const;
	bool IsStickDown(JOYPAD_NO no, JOYPAD_STICK stick) const;
	bool IsStickUp(JOYPAD_NO no, JOYPAD_STICK stick) const;

	bool IsMouseNew(MOUSE mouse);
	bool IsMouseTrgUp(MOUSE mouse);
	bool IsMouseTrgDown(MOUSE mouse);
private:

	// キー情報
	struct Info
	{
		int key;			// キーID
		bool keyOld;		// 1フレーム前の押下状態
		bool keyNew;		// 現フレームの押下状態
		bool keyTrgDown;	// 現フレームでボタンが押されたか
		bool keyTrgUp;		// 現フレームでボタンが離されたか
	};

	// マウス
	struct MouseInfo
	{
		bool keyOld = false;		// 1フレーム前の押下状態
		bool keyNew = false;		// 現フレームの押下状態
		bool keyTrgDown = false;	// 現フレームでボタンが押されたか
		bool keyTrgUp = false;		// 現フレームでボタンが離されたか
	};

	struct StickInfo
	{
		JOYPAD_STICK key;
		bool keyOld = false;
		bool keyNew = false;
		bool keyTrgDown = false;
		bool keyTrgUp = false;
	};

	// コントローラ情報
	DINPUT_JOYSTATE joyDInState_;

	// コントローラ情報(XBOX)
	XINPUT_STATE joyXInState_;

	// シングルトン用インスタンス
	static InputManager* instance_;

	// キー情報
	std::map<int, InputManager::Info> keyInfos_;
	InputManager::Info infoEmpty_;

	// マウス情報
	std::map<MOUSE, InputManager::MouseInfo> mouseInfos_;
	InputManager::MouseInfo mouseInfoEmpty_;

	// スティック情報
	std::map<JOYPAD_NO, std::vector<InputManager::StickInfo>> stickInfos_;

	// マウスカーソルの位置
	Vector2 mousePrePos_;
	Vector2 mousePos_;
	
	//マウスホイール回転量
	int wheelRot_;

	// マウスボタンの入力状態
	int mouseInput_;

	// パッド情報
	JOYPAD_IN_STATE padInfos_[5];

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	InputManager(void);
	InputManager(const InputManager& manager) = default;
	~InputManager(void);

	// 配列の中からキー情報を取得する
	const InputManager::Info& Find(int key) const;

	// 配列の中からマウス情報を取得する
	const InputManager::MouseInfo& FindMouse(MOUSE key) const;

	// 接続されたコントローラの種別を取得する
	JOYPAD_TYPE GetJPadType(JOYPAD_NO no);

	// コントローラの入力情報を取得する
	DINPUT_JOYSTATE GetJPadDInputState(JOYPAD_NO no);

	// コントローラ(XBOX)の入力情報を取得する
	XINPUT_STATE GetJPadXInputState(JOYPAD_NO no);

	// コントローラの入力情報を更新する
	void SetJPadInState(JOYPAD_NO jpNo);

	//指定の方向に倒れた度合い0から1000
	int PadStickOverSize(JOYPAD_NO no, JOYPAD_STICK stick)const;
};