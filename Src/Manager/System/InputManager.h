#pragma once
#include <map>
#include <vector>
#include <Dxlib.h>
#include "../../Common/Vector2.h"
#include "KeyConfig.h"

class InputManager
{

public:

	static constexpr int STICK_THRESHOLD = 300;	//スティックの猶予

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
	// ゲームコントローラーの入力情報
	struct JOYPAD_IN_STATE
	{
		unsigned char ButtonsOld[static_cast<int>(KeyConfig::JOYPAD_BTN::MAX)];
		unsigned char ButtonsNew[static_cast<int>(KeyConfig::JOYPAD_BTN::MAX)];
		bool IsOld[static_cast<int>(KeyConfig::JOYPAD_BTN::MAX)];
		bool IsNew[static_cast<int>(KeyConfig::JOYPAD_BTN::MAX)];
		bool IsTrgDown[static_cast<int>(KeyConfig::JOYPAD_BTN::MAX)];
		bool IsTrgUp[static_cast<int>(KeyConfig::JOYPAD_BTN::MAX)];
		int AKeyLX;
		int AKeyLY;
		int AKeyRX;
		int AKeyRY;
	};
	InputManager(void);
	InputManager(const InputManager& manager) = default;
	~InputManager(void);

	void Init(void);
	void Update(void);
	void Release(void);

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

	//マウスの移動距離を取得
	Vector2 GetMousePosDistance(void)const;

	//マウスの座標を設定
	void SetMousePosScreen(void);

	void SetMousePos(const Vector2& pos);

	// マウスのクリック状態を取得(MOUSE_INPUT_LEFT、RIGHT)
	int GetMouse(void) const;

	// コントローラの入力情報を取得する
	JOYPAD_IN_STATE GetJPadInputState(KeyConfig::JOYPAD_NO no);

	// ボタンが押された
	bool IsPadBtnNew(KeyConfig::JOYPAD_NO no, KeyConfig::JOYPAD_BTN btn) const;
	bool IsPadBtnTrgDown(KeyConfig::JOYPAD_NO no, KeyConfig::JOYPAD_BTN btn) const;
	bool IsPadBtnTrgUp(KeyConfig::JOYPAD_NO no, KeyConfig::JOYPAD_BTN btn) const;

	// スティックが倒されたか
	bool IsStickNew(KeyConfig::JOYPAD_NO no, KeyConfig::JOYPAD_STICK stick) const;
	bool IsStickDown(KeyConfig::JOYPAD_NO no, KeyConfig::JOYPAD_STICK stick) const;
	bool IsStickUp(KeyConfig::JOYPAD_NO no, KeyConfig::JOYPAD_STICK stick) const;

	bool IsMouseNew(KeyConfig::MOUSE mouse);
	bool IsMouseTrgUp(KeyConfig::MOUSE mouse);
	bool IsMouseTrgDown(KeyConfig::MOUSE mouse);
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
		KeyConfig::JOYPAD_STICK key;
		bool keyOld = false;
		bool keyNew = false;
		bool keyTrgDown = false;
		bool keyTrgUp = false;
	};

	// コントローラ情報
	DINPUT_JOYSTATE joyDInState_;

	// コントローラ情報(XBOX)
	XINPUT_STATE joyXInState_;

	// キー情報
	std::map<int, InputManager::Info> keyInfos_;
	InputManager::Info infoEmpty_;

	// マウス情報
	std::map<KeyConfig::MOUSE, InputManager::MouseInfo> mouseInfos_;
	InputManager::MouseInfo mouseInfoEmpty_;

	// スティック情報
	std::map<KeyConfig::JOYPAD_NO, std::vector<InputManager::StickInfo>> stickInfos_;

	// マウスカーソルの位置
	Vector2 mousePrePos_;
	Vector2 mousePos_;
	
	//マウスホイール回転量
	int wheelRot_;

	// マウスボタンの入力状態
	int mouseInput_;

	// パッド情報
	JOYPAD_IN_STATE padInfos_[5];

	// 配列の中からキー情報を取得する
	const InputManager::Info& Find(int key) const;

	// 配列の中からマウス情報を取得する
	const InputManager::MouseInfo& FindMouse(KeyConfig::MOUSE key) const;

	// 接続されたコントローラの種別を取得する
	JOYPAD_TYPE GetJPadType(KeyConfig::JOYPAD_NO no);

	// コントローラの入力情報を取得する
	DINPUT_JOYSTATE GetJPadDInputState(KeyConfig::JOYPAD_NO no);

	// コントローラ(XBOX)の入力情報を取得する
	XINPUT_STATE GetJPadXInputState(KeyConfig::JOYPAD_NO no);

	// コントローラの入力情報を更新する
	void SetJPadInState(KeyConfig::JOYPAD_NO jpNo);

};