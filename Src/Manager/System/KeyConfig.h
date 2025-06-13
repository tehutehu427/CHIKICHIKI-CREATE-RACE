#pragma once
#include <vector>
#include "InputManager.h"
class KeyConfig
{
public:

	enum class CONTROL_TYPE //操作の種類	
	{
		ENTER,					//決定
		CANCEL,					//キャンセル
		SELECT_UP,				//選択　上
		SELECT_DOWN,			//選択　下
		SELECT_LEFT,			//選択　左
		SELECT_RIGHT,			//選択　右
		PALETTE_CURSOR_UP,		//パレット時のカーソル上移動
		PALETTE_CURSOR_DOWN,	//パレット時のカーソル下移動
		PALETTE_CURSOR_LEFT,	//パレット時のカーソル左移動
		PALETTE_CURSOR_RIGHT,	//パレット時のカーソル右移動
		EDIT_CAMERA_ROT_UP,		//エディット時のカメラ回転上
		EDIT_CAMERA_ROT_DOWN,	//エディット時のカメラ回転下
		EDIT_CAMERA_ROT_RIGHT,	//エディット時のカメラ回転右
		EDIT_CAMERA_ROT_LEFT,	//エディット時のカメラ回転左
		EDIT_CAMERA_MOVE_FRONT,	//エディット時のカメラ移動前
		EDIT_CAMERA_MOVE_BACK,	//エディット時のカメラ移動後ろ
		PLAYER_JUMP,			//プレイヤーのジャンプ
		PLAYER_PUNCH,			//プレイヤーのパンチ
		PLAYER_MOVE_FRONT,		//プレイヤーの移動前
		PLAYER_MOVE_BACK,		//プレイヤーの移動後ろ
		PLAYER_MOVE_RIGHT,		//プレイヤーの移動右
		PLAYER_MOVE_LEFT,		//プレイヤーの移動左
		PLAY_CAMERA_MOVE_UP,	//プレイのカメラ移動上
		PLAY_CAMERA_MOVE_DOWN,	//プレイのカメラ移動下
		PLAY_CAMERA_MOVE_RIGHT,	//プレイのカメラ移動右
		PLAY_CAMERA_MOVE_LEFT,	//プレイのカメラ移動左
		MAX,
	};

	// インスタンスを明示的に生成
	static void CreateInstance(void);

	// インスタンスの取得
	static KeyConfig& GetInstance(void);

	void Init(void);
	void Update(void);

	//操作の種類別にキーの状態を取得
	bool IsNew(CONTROL_TYPE cType, InputManager::JOYPAD_NO no);
	bool IsTrgDown(CONTROL_TYPE cType, InputManager::JOYPAD_NO no);
	bool IsTrgUp(CONTROL_TYPE cType, InputManager::JOYPAD_NO no);

	//操作の種類別にキーを追加	
	void Add(int key , CONTROL_TYPE type);
	void Add(InputManager::JOYPAD_BTN key, CONTROL_TYPE type);
	void Add(InputManager::JOYPAD_STICK key, CONTROL_TYPE type);

	// リソースの破棄
	void Destroy(void);

private:
	
	std::map<CONTROL_TYPE, std::vector<int>>keyInput_;	//操作の種類とキーの種類でキーボードの状態を格納
	std::map<CONTROL_TYPE, std::vector<InputManager::JOYPAD_BTN>>conInput_;	//操作の種類とボタンの種類でコントローラーの状態を格納
	std::map<CONTROL_TYPE, std::vector<InputManager::JOYPAD_STICK>>stickInput_;	//操作の種類とスティックの種類でコントローラーの状態を格納

	// シングルトン用インスタンス
	static KeyConfig* instance_;

	KeyConfig(void);
	KeyConfig(const KeyConfig& manager);
	~KeyConfig(void) = default;
};

