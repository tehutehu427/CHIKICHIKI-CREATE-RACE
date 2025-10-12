#pragma once
#include <Dxlib.h>
#include "../../Common/Vector2.h"
#include "KeyConfig.h"

class InputManager
{

public:

	//スティックの猶予
	static constexpr int STICK_THRESHOLD = 300;

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

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name=""></param>
	InputManager(void);

	/// <summary>
	/// コピーコンストラクタ
	/// </summary>
	/// <param name="_manager"></param>
	InputManager(const InputManager& _manager) = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~InputManager(void);

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
	/// 解放処理
	/// </summary>
	/// <param name=""></param>
	void Release(void);

	/// <summary>
	/// 判定を行うキーボードのキーを追加
	/// </summary>
	/// <param name="_key"></param>
	void Add(int _key);

	/// <summary>
	/// 判定を行うキーをクリア
	/// </summary>
	/// <param name=""></param>
	void Clear(void);

	/// <summary>
	/// キーの押下判定
	/// </summary>
	/// <param name="_key">キー</param>
	/// <returns>押されているならtrue</returns>
	bool IsNew(int _key) const;

	/// <summary>
	/// キーの押下判定(押しっぱなしはNG)
	/// </summary>
	/// <param name="_key">キー</param>
	/// <returns>押された瞬間ならtrue</returns>
	bool IsTrgDown(int _key) const;

	/// <summary>
	/// キーを離した時の判定
	/// </summary>
	/// <param name="_key">キー</param>
	/// <returns>離された瞬間ならture</returns>
	bool IsTrgUp(int _key) const;

	/// <summary>
	/// マウス座標の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>マウス座標</returns>
	Vector2 GetMousePos(void) const;

	/// <summary>
	/// マウスの移動距離を取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>マウスの移動量</returns>
	Vector2 GetMousePosDistance(void)const;

	/// <summary>
	/// マウスの座標をスクリーン中央に設定
	/// </summary>
	/// <param name=""></param>
	void SetMousePosScreen(void);
	/// <summary>
	/// マウスの座標を設定
	/// </summary>
	/// <param name="_pos">設定する座標</param>
	void SetMousePos(const Vector2& _pos);

	/// <summary>
	/// マウスのクリック状態を取得(MOUSE_INPUT_LEFT、RIGHT)
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	int GetMouse(void) const;

	/// <summary>
	/// コントローラの入力情報を取得する
	/// </summary>
	/// <param name="_no">パッド番号</param>
	/// <returns>入力情報</returns>
	JOYPAD_IN_STATE GetJPadInputState(KeyConfig::JOYPAD_NO _no);

	/// <summary>
	/// ボタンが押された
	/// </summary>
	/// <param name="_no">パッド番号</param>
	/// <param name="_btn">ボタンの種類</param>
	/// <returns>押されているならtrue</returns>
	bool IsPadBtnNew(KeyConfig::JOYPAD_NO _no, KeyConfig::JOYPAD_BTN _btn) const;
	/// <summary>
	/// ボタンが押された瞬間か
	/// </summary>
	/// <param name="_no">パッド番号</param>
	/// <param name="_btn">ボタンの種類</param>
	/// <returns>押された瞬間ならtrue</returns>
	bool IsPadBtnTrgDown(KeyConfig::JOYPAD_NO _no, KeyConfig::JOYPAD_BTN _btn) const;
	/// <summary>
	/// ボタンが離れた瞬間か
	/// </summary>
	/// <param name="_no">パッド番号</param>
	/// <param name="_btn">ボタンの種類</param>
	/// <returns>離れた瞬間ならtrue</returns>
	bool IsPadBtnTrgUp(KeyConfig::JOYPAD_NO _no, KeyConfig::JOYPAD_BTN _btn) const;

	/// <summary>
	///  スティックが閾値以上倒されたか
	/// </summary>
	/// <param name="_no">パッド番号</param>
	/// <param name="_stick">スティックの方向</param>
	/// <returns>押されているならtrue</returns>
	bool IsStickNew(KeyConfig::JOYPAD_NO _no, KeyConfig::JOYPAD_STICK _stick) const;
	/// <summary>
	///  スティックが閾値以上倒されたか
	/// </summary>
	/// <param name="_no">パッド番号</param>
	/// <param name="_stick">スティックの方向</param>
	/// <returns>押された瞬間ならtrue</returns>
	bool IsStickDown(KeyConfig::JOYPAD_NO _no, KeyConfig::JOYPAD_STICK _stick) const;
	/// <summary>
	///  スティックが閾値以上倒されたか
	/// </summary>
	/// <param name="_no">パッド番号</param>
	/// <param name="_stick">スティックの方向</param>
	/// <returns>離された瞬間ならtrue</returns>
	bool IsStickUp(KeyConfig::JOYPAD_NO _no, KeyConfig::JOYPAD_STICK _stick) const;

	/// <summary>
	/// マウスが押されたか
	/// </summary>
	/// <param name="_mouse">ボタンの種類</param>
	/// <returns>押されていたらtrue</returns>
	bool IsMouseNew(KeyConfig::MOUSE _mouse);
	/// <summary>
	/// マウスが押されたか
	/// </summary>
	/// <param name="_mouse">ボタンの種類</param>
	/// <returns>押された瞬間ならtrue</returns>
	bool IsMouseTrgUp(KeyConfig::MOUSE _mouse);
	/// <summary>
	/// マウスが押されたか
	/// </summary>
	/// <param name="_mouse">ボタンの種類</param>
	/// <returns>離された瞬間ならtrue</returns>
	bool IsMouseTrgDown(KeyConfig::MOUSE _mouse);

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
	const InputManager::Info& Find(int _key) const;

	// 配列の中からマウス情報を取得する
	const InputManager::MouseInfo& FindMouse(KeyConfig::MOUSE _key) const;

	// 接続されたコントローラの種別を取得する
	JOYPAD_TYPE GetJPadType(KeyConfig::JOYPAD_NO _no);

	// コントローラの入力情報を取得する
	DINPUT_JOYSTATE GetJPadDInputState(KeyConfig::JOYPAD_NO _no);

	// コントローラ(XBOX)の入力情報を取得する
	XINPUT_STATE GetJPadXInputState(KeyConfig::JOYPAD_NO _no);

	// コントローラの入力情報を更新する
	void SetJPadInState(KeyConfig::JOYPAD_NO _jpNo);

};