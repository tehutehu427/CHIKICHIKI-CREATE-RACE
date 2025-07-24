#include <cmath>
#include "KeyConfig.h"
#include "InputManager.h"
KeyConfig* KeyConfig::instance_ = nullptr;

void KeyConfig::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new KeyConfig();
	}
	instance_->Init();
}

KeyConfig& KeyConfig::GetInstance(void)
{
	if (instance_ == nullptr)
	{
		KeyConfig::CreateInstance();
	}
	return *instance_;
}

void KeyConfig::Init(void)
{
	inputManager_ = std::make_unique<InputManager>();
	inputManager_->Init();

	//決定処理
	Add(CONTROL_TYPE::ENTER, KEY_INPUT_SPACE);
	Add(CONTROL_TYPE::ENTER, KEY_INPUT_RETURN);
	Add(CONTROL_TYPE::ENTER, MOUSE::CLICK_LEFT);
	Add(CONTROL_TYPE::ENTER, JOYPAD_BTN::RIGHTBUTTON_RIGHT);

	//決定処理
	Add(CONTROL_TYPE::DECISION_KEY_AND_PAD, JOYPAD_BTN::RIGHTBUTTON_RIGHT);
	Add(CONTROL_TYPE::DECISION_KEY_AND_PAD, KEY_INPUT_SPACE);
	Add(CONTROL_TYPE::DECISION_MOUSE, MOUSE::CLICK_LEFT);


	//キャンセル
	Add(CONTROL_TYPE::CANCEL, KEY_INPUT_BACK);	//キャンセル
	Add(CONTROL_TYPE::CANCEL, KeyConfig::JOYPAD_BTN::RIGHTBUTTON_DOWN);


	//フェーズ遷移
	Add(CONTROL_TYPE::PHASE_CHANGE, KEY_INPUT_Z);
	Add(CONTROL_TYPE::PHASE_CHANGE, JOYPAD_BTN::START_BUTTON);
	Add(CONTROL_TYPE::PHASE_CHANGE_CHECK, MOUSE::CLICK_LEFT);
	Add(CONTROL_TYPE::PHASE_CHANGE_CHECK, KeyConfig::JOYPAD_BTN::RIGHTBUTTON_RIGHT);


	//ファイル読みこみ
	Add(CONTROL_TYPE::IMPORT_FILE, KEY_INPUT_N);
	Add(CONTROL_TYPE::IMPORT_FILE_CLICK, MOUSE::CLICK_LEFT);
	Add(CONTROL_TYPE::IMPORT_FILE_CLICK, KeyConfig::JOYPAD_BTN::RIGHTBUTTON_RIGHT);
	Add(CONTROL_TYPE::EXPORT_FILE, KEY_INPUT_M);
	Add(CONTROL_TYPE::EXPORT_FILE_CLICK, MOUSE::CLICK_LEFT);
	Add(CONTROL_TYPE::EXPORT_FILE_CLICK, KeyConfig::JOYPAD_BTN::RIGHTBUTTON_RIGHT);


	//パレットを開く
	Add(CONTROL_TYPE::PALETTE_CURSOR_SELECT, MOUSE::CLICK_LEFT);
	Add(CONTROL_TYPE::PALETTE_CURSOR_SELECT, KeyConfig::JOYPAD_BTN::RIGHTBUTTON_RIGHT);


	//エディットカメラ動作
	Add(CONTROL_TYPE::EDIT_CAMERA_ROT_DOWN, KEY_INPUT_S);
	Add(CONTROL_TYPE::EDIT_CAMERA_ROT_UP, KEY_INPUT_W);
	Add(CONTROL_TYPE::EDIT_CAMERA_ROT_LEFT, KEY_INPUT_Q);
	Add(CONTROL_TYPE::EDIT_CAMERA_ROT_RIGHT, KEY_INPUT_E);
	Add(CONTROL_TYPE::EDIT_CAMERA_MOVE_LEFT, KEY_INPUT_A);
	Add(CONTROL_TYPE::EDIT_CAMERA_MOVE_LEFT, JOYPAD_BTN::L_BUTTON);
	Add(CONTROL_TYPE::EDIT_CAMERA_MOVE_LEFT, JOYPAD_BTN::LEFTBUTTON_LEFT);
	Add(CONTROL_TYPE::EDIT_CAMERA_MOVE_RIGHT, KEY_INPUT_D);
	Add(CONTROL_TYPE::EDIT_CAMERA_MOVE_RIGHT, JOYPAD_BTN::R_BUTTON);
	Add(CONTROL_TYPE::EDIT_CAMERA_MOVE_RIGHT, JOYPAD_BTN::LEFTBUTTON_RIGHT);
	Add(CONTROL_TYPE::EDIT_CAMERA_MOVE_FRONT, MOUSE::WHEEL_FRONT);
	Add(CONTROL_TYPE::EDIT_CAMERA_MOVE_FRONT, JOYPAD_BTN::R_TRIGGER);
	Add(CONTROL_TYPE::EDIT_CAMERA_MOVE_UP, JOYPAD_BTN::LEFTBUTTON_TOP);
	Add(CONTROL_TYPE::EDIT_CAMERA_MOVE_BACK, MOUSE::WHEEL_BACK);
	Add(CONTROL_TYPE::EDIT_CAMERA_MOVE_BACK, JOYPAD_BTN::L_TRIGGER);
	Add(CONTROL_TYPE::EDIT_CAMERA_MOVE_DOWN, JOYPAD_BTN::LEFTBUTTON_DOWN);
	Add(CONTROL_TYPE::CURSOR_SPEED_UP, JOYPAD_BTN::LEFT_STICK);
	Add(CONTROL_TYPE::EDIT_CAMERA_CHANGE,KEY_INPUT_LCONTROL);
	Add(CONTROL_TYPE::EDIT_CAMERA_CHANGE,JOYPAD_BTN::RIGHT_STICK);


	//エディット中のアイテム選択処理
	Add(CONTROL_TYPE::EDIT_ITEM_SELECT, MOUSE::CLICK_LEFT);
	Add(CONTROL_TYPE::EDIT_ITEM_SELECT, JOYPAD_BTN::RIGHTBUTTON_RIGHT);
	Add(CONTROL_TYPE::EDIT_ITEM_ROTATE, KEY_INPUT_R);
	Add(CONTROL_TYPE::EDIT_ITEM_ROTATE, JOYPAD_BTN::RIGHTBUTTON_TOP);
	Add(CONTROL_TYPE::EDIT_ITEM_DELETE, MOUSE::CLICK_RIGHT);
	Add(CONTROL_TYPE::EDIT_ITEM_DELETE, JOYPAD_BTN::RIGHTBUTTON_DOWN);

	//エディット中のグリッドのオンオフ
	Add(CONTROL_TYPE::EDIT_GRID_ON_OFF, KEY_INPUT_G);
	Add(CONTROL_TYPE::EDIT_GRID_ON_OFF, JOYPAD_BTN::RIGHTBUTTON_LEFT);

	//プレイヤーの動作
	Add(CONTROL_TYPE::PLAYER_DASH, KEY_INPUT_LSHIFT);
	Add(CONTROL_TYPE::PLAYER_DASH, JOYPAD_BTN::RIGHTBUTTON_DOWN);
	Add(CONTROL_TYPE::PLAYER_DASH, JOYPAD_BTN::LEFT_STICK);
	Add(CONTROL_TYPE::PLAYER_DASH, JOYPAD_BTN::R_BUTTON);
	Add(CONTROL_TYPE::PLAYER_MOVE_FRONT, KEY_INPUT_W);
	Add(CONTROL_TYPE::PLAYER_MOVE_LEFT, KEY_INPUT_A);
	Add(CONTROL_TYPE::PLAYER_MOVE_BACK, KEY_INPUT_S);
	Add(CONTROL_TYPE::PLAYER_MOVE_RIGHT, KEY_INPUT_D);
	Add(CONTROL_TYPE::PLAYER_JUMP, KEY_INPUT_SPACE);
	Add(CONTROL_TYPE::PLAYER_JUMP, JOYPAD_BTN::RIGHTBUTTON_RIGHT);
	Add(CONTROL_TYPE::PLAYER_PUNCH, KEY_INPUT_RETURN);
	Add(CONTROL_TYPE::PLAYER_PUNCH, JOYPAD_BTN::RIGHTBUTTON_LEFT);

	//フリーエディットをやめる
	Add(CONTROL_TYPE::EDIT_ESCAPE, KEY_INPUT_BACK);
	Add(CONTROL_TYPE::EDIT_ESCAPE_CLICK, MOUSE::CLICK_LEFT);
	Add(CONTROL_TYPE::EDIT_ESCAPE_CLICK, JOYPAD_BTN::RIGHTBUTTON_RIGHT);


	//プレイ中のカメラ動作
	Add(CONTROL_TYPE::PLAY_CAMERA_MOVE_UP, KEY_INPUT_UP);
	//Add(CONTROL_TYPE::PLAY_CAMERA_MOVE_UP, MOUSE::MOVE_UP);
	Add(CONTROL_TYPE::PLAY_CAMERA_MOVE_DOWN, KEY_INPUT_DOWN);
	//Add(CONTROL_TYPE::PLAY_CAMERA_MOVE_DOWN, MOUSE::MOVE_DOWN);
	Add(CONTROL_TYPE::PLAY_CAMERA_MOVE_RIGHT, KEY_INPUT_RIGHT);
	//Add(CONTROL_TYPE::PLAY_CAMERA_MOVE_RIGHT, MOUSE::MOVE_RIGHT);
	Add(CONTROL_TYPE::PLAY_CAMERA_MOVE_LEFT, KEY_INPUT_LEFT);
	//Add(CONTROL_TYPE::PLAY_CAMERA_MOVE_LEFT, MOUSE::MOVE_LEFT);
	Add(CONTROL_TYPE::PLAY_CAMERA_ZOOM_IN, JOYPAD_BTN::R_TRIGGER);
	Add(CONTROL_TYPE::PLAY_CAMERA_ZOOM_IN, MOUSE::WHEEL_FRONT);
	Add(CONTROL_TYPE::PLAY_CAMERA_ZOOM_OUT, JOYPAD_BTN::L_TRIGGER);
	Add(CONTROL_TYPE::PLAY_CAMERA_ZOOM_OUT, MOUSE::WHEEL_BACK);

	//選択処理
	Add(CONTROL_TYPE::SELECT_UP, KEY_INPUT_UP);	//上
	Add(CONTROL_TYPE::SELECT_UP, KEY_INPUT_W);	//上
	Add(CONTROL_TYPE::SELECT_UP, KeyConfig::JOYPAD_STICK::L_STICK_UP);
	Add(CONTROL_TYPE::SELECT_DOWN, KEY_INPUT_DOWN);	//下
	Add(CONTROL_TYPE::SELECT_DOWN, KEY_INPUT_S);	//下
	Add(CONTROL_TYPE::SELECT_DOWN, KeyConfig::JOYPAD_STICK::L_STICK_DOWN);
	Add(CONTROL_TYPE::SELECT_RIGHT, KEY_INPUT_RIGHT);//右
	Add(CONTROL_TYPE::SELECT_RIGHT, KEY_INPUT_D);//右
	Add(CONTROL_TYPE::SELECT_RIGHT, KeyConfig::JOYPAD_STICK::L_STICK_RIGHT);
	Add(CONTROL_TYPE::SELECT_LEFT, KEY_INPUT_LEFT);	//左
	Add(CONTROL_TYPE::SELECT_LEFT, KEY_INPUT_A);	//左
	Add(CONTROL_TYPE::SELECT_LEFT, KeyConfig::JOYPAD_STICK::L_STICK_LEFT);

	//マニュアルを開く
	Add(CONTROL_TYPE::MANUAL, KEY_INPUT_TAB);
	Add(CONTROL_TYPE::MANUAL, JOYPAD_BTN::SELECT_BUTTON);
	Add(CONTROL_TYPE::MANUAL_ICON_CLICK, KeyConfig::MOUSE::CLICK_LEFT);
	Add(CONTROL_TYPE::MANUAL_ICON_CLICK, KeyConfig::JOYPAD_BTN::RIGHTBUTTON_RIGHT);

	//デモからタイトルに戻る
	Add(CONTROL_TYPE::DEMO_TO_TITLE_BACK, KeyConfig::JOYPAD_BTN::START_BUTTON);
	Add(CONTROL_TYPE::DEMO_TO_TITLE_BACK, KEY_INPUT_BACK);


	Add(CONTROL_TYPE::SELECT_SKIP, KeyConfig::JOYPAD_BTN::RIGHTBUTTON_TOP);


	Add(CONTROL_TYPE::INPUT_CHECK, KeyConfig::JOYPAD_BTN::RIGHTBUTTON_RIGHT);

#ifdef _DEBUG
	//デバッグ処理
	Add(CONTROL_TYPE::DEBUG_CHANGE_TITLE, KEY_INPUT_RSHIFT);	//タイトルシーン遷移
	Add(CONTROL_TYPE::DEBUG_CHANGE_CLEAR, KEY_INPUT_C);	//ゲームクリアへの遷移
	Add(CONTROL_TYPE::DEBUG_CHANGE_INPUT, KEY_INPUT_F1);
	Add(CONTROL_TYPE::DEBUG_CHANGE_INPUT, KeyConfig::JOYPAD_BTN::START_BUTTON);
#endif // _DEBUG


}

void KeyConfig::Update(void)
{
	inputManager_->Update();
}

bool KeyConfig::IsNew(CONTROL_TYPE cType, JOYPAD_NO no,TYPE type)
{
	if (type == TYPE::KEYBORD_MOUSE || type == TYPE::ALL)
	{
		for (auto& key : keyInput_)
		{
			if (key.first != cType)
			{
				continue;
			}
			for (auto keyI : key.second)
			{
				if (inputManager_->IsNew(keyI))
				{
					return true;
				}
			}
		}
		for (auto& mouse : mouseInput_)
		{
			if (mouse.first != cType)
			{
				continue;
			}
			for (auto mouseI : mouse.second)
			{
				if (inputManager_->IsMouseNew(mouseI))
				{
					return true;
				}
			}
		}
	}
	if (type == TYPE::PAD || type == TYPE::ALL)
	{
		for (auto& con : conInput_)
		{
			if (con.first != cType)
			{
				continue;
			}
			for (auto conI : con.second)
			{
				if (inputManager_->IsPadBtnNew(no, conI))
				{
					return true;
				}
			}
		}
		for (auto& stick : stickInput_)
		{
			if (stick.first != cType)
			{
				continue;
			}
			for (auto stickI : stick.second)
			{
				if (inputManager_->IsStickNew(no, stickI))
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool KeyConfig::IsTrgDown(CONTROL_TYPE cType, JOYPAD_NO no ,TYPE type)
{

	if (type == TYPE::KEYBORD_MOUSE || type == TYPE::ALL)
	{
		for (auto& key : keyInput_)
		{
			if (key.first != cType)
			{
				continue;
			}
			for (auto keyI : key.second)
			{
				if (inputManager_->IsTrgDown(keyI))
				{
					return true;
				}
			}
		}
		for (auto& mouse : mouseInput_)
		{
			if (mouse.first != cType)
			{
				continue;
			}
			for (auto mouseI : mouse.second)
			{
				if (inputManager_->IsMouseTrgDown(mouseI))
				{
					return true;
				}
			}
		}
	}
	if (type == TYPE::PAD || type == TYPE::ALL)
	{
		for (auto& con : conInput_)
		{
			if (con.first != cType)
			{
				continue;
			}
			for (auto conI : con.second)
			{
				if (inputManager_->IsPadBtnTrgDown(no, conI))
				{
					return true;
				}
			}
		}
		for (auto& stick : stickInput_)
		{
			if (stick.first != cType)
			{
				continue;
			}
			for (auto stickI : stick.second)
			{
				if (inputManager_->IsStickDown(no, stickI))
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool KeyConfig::IsTrgUp(CONTROL_TYPE cType, JOYPAD_NO no, TYPE type)
{
	if (type == TYPE::KEYBORD_MOUSE || type == TYPE::ALL)
	{
		for (auto& key : keyInput_)
		{
			if (key.first != cType)
			{
				continue;
			}
			for (auto keyI : key.second)
			{
				if (inputManager_->IsTrgUp(keyI))
				{
					return true;
				}
			}
		}
		for (auto& mouse : mouseInput_)
		{
			if (mouse.first != cType)
			{
				continue;
			}
			for (auto mouseI : mouse.second)
			{
				if (inputManager_->IsMouseTrgUp(mouseI))
				{
					return true;
				}
			}
		}
	}
	if (type == TYPE::PAD || type == TYPE::ALL)
	{
		for (auto& con : conInput_)
		{
			if (con.first != cType)
			{
				continue;
			}
			for (auto conI : con.second)
			{
				if (inputManager_->IsPadBtnTrgUp(no, conI))
				{
					return true;
				}
			}
		}
		for (auto& stick : stickInput_)
		{
			if (stick.first != cType)
			{
				continue;
			}
			for (auto stickI : stick.second)
			{
				if (inputManager_->IsStickUp(no, stickI))
				{
					return true;
				}
			}
		}
	}
	return false;
}

void KeyConfig::Add(CONTROL_TYPE type,int key )
{
	for (auto &keys : keyInput_)
	{
		if (keys.first != type)
		{
			continue;
		}
		keys.second.emplace_back(key);
		return;
	}

	std::vector<int> keys;
	keys.emplace_back(key);
	keyInput_.emplace(type, keys);
}

void KeyConfig::Add(CONTROL_TYPE type,JOYPAD_BTN key)
{
	for (auto& con : conInput_)
	{
		if (con.first != type)
		{
			continue;
		}
		con.second.emplace_back(key);
		return;
	}

	std::vector<JOYPAD_BTN> cons;
	cons.emplace_back(key);
	conInput_.emplace(type, cons);
}

void KeyConfig::Add(CONTROL_TYPE type,JOYPAD_STICK key)
{
	for (auto& stick : stickInput_)
	{
		if (stick.first != type)
		{
			continue;
		}
		stick.second.emplace_back(key);
		return;
	}

	std::vector<JOYPAD_STICK> sticks;
	sticks.emplace_back(key);
	stickInput_.emplace(type, sticks);
}

void KeyConfig::Add(CONTROL_TYPE type, MOUSE key)
{
	for (auto& mouse : mouseInput_)
	{
		if (mouse.first != type)
		{
			continue;
		}
		mouse.second.emplace_back(key);
		return;
	}

	std::vector<MOUSE> mouse;
	mouse.emplace_back(key);
	mouseInput_.emplace(type, mouse);
}


Vector2 KeyConfig::GetMousePos(void) const
{
	return inputManager_->GetMousePos();
}

Vector2 KeyConfig::GetMouseMove(void) const
{
	return inputManager_->GetMousePosDistance();
}

void KeyConfig::SetMousePosScreen(void)
{
	//inputManager_->SetMousePosScreen();
}

void KeyConfig::SetMousePos(const Vector2& pos)
{
	inputManager_->SetMousePos(pos);
}

float KeyConfig::GetLStickDeg(KeyConfig::JOYPAD_NO no) const
{
	float deg = 0.0f;
	Vector2 knockSize = GetKnockLStickSize(no);
	if (knockSize.x == 0.0f && knockSize.y == 0.0f)
	{
		return deg;
	}
	auto rad = std::atan2(knockSize.y, knockSize.x);
	deg = rad * (180.0f / DX_PI_F);
	deg += 90.0f;
	deg = deg < 0 ? deg + 360 : deg;
	return deg;
}

float KeyConfig::GetRStickDeg(KeyConfig::JOYPAD_NO no) const
{
	float deg = 0.0f;
	Vector2 knockSize = GetKnockRStickSize(no);
	if (knockSize.x == 0.0f && knockSize.y == 0.0f)
	{
		return deg;
	}
	auto rad = std::atan2(knockSize.y, knockSize.x);
	deg = rad * (180.0f / DX_PI_F);
	deg += 90.0f;
	deg = deg < 0 ? deg + 360 : deg;
	return deg;
}

Vector2 KeyConfig::GetKnockLStickSize(KeyConfig::JOYPAD_NO no) const
{
	//auto padInfo = padInfos_[static_cast<int>(no)];
	auto padInfo = inputManager_->GetJPadInputState(no);
	return Vector2(padInfo.AKeyLX, padInfo.AKeyLY);
}

Vector2 KeyConfig::GetKnockRStickSize(KeyConfig::JOYPAD_NO no) const
{
	//auto padInfo = padInfos_[static_cast<int>(no)];
	auto padInfo = inputManager_->GetJPadInputState(no);
	return Vector2(padInfo.AKeyRX, padInfo.AKeyRY);
}
int KeyConfig::PadStickOverSize(KeyConfig::JOYPAD_NO no, KeyConfig::JOYPAD_STICK stick) const
{
	int ret = 0;
	auto padInfo = inputManager_->GetJPadInputState(no);
	switch (stick)
	{
	case KeyConfig::JOYPAD_STICK::L_STICK_UP:
		ret = padInfo.AKeyLY < 0 ? padInfo.AKeyLY : 0;
		break;
	case KeyConfig::JOYPAD_STICK::L_STICK_DOWN:
		ret = padInfo.AKeyLY > 0 ? padInfo.AKeyLY : 0;
		break;
	case KeyConfig::JOYPAD_STICK::L_STICK_LEFT:
		ret = padInfo.AKeyLX < 0 ? padInfo.AKeyLX : 0;
		break;
	case KeyConfig::JOYPAD_STICK::L_STICK_RIGHT:
		ret = padInfo.AKeyLX > 0 ? padInfo.AKeyLX : 0;
		break;
	case KeyConfig::JOYPAD_STICK::R_STICK_UP:
		ret = padInfo.AKeyRY < 0 ? padInfo.AKeyRY : 0;
		break;
	case KeyConfig::JOYPAD_STICK::R_STICK_DOWN:
		ret = padInfo.AKeyRY > 0 ? padInfo.AKeyRY : 0;
		break;
	case KeyConfig::JOYPAD_STICK::R_STICK_LEFT:
		ret = padInfo.AKeyRX < 0 ? padInfo.AKeyRX : 0;
		break;
	case KeyConfig::JOYPAD_STICK::R_STICK_RIGHT:
		ret = padInfo.AKeyRX > 0 ? padInfo.AKeyRX : 0;
		break;
	case KeyConfig::JOYPAD_STICK::MAX:
		break;
	default:
		break;
	}
	return abs(ret);
}


void KeyConfig::Destroy(void)
{
	inputManager_->Release();
	keyInput_.clear();
	conInput_.clear();
	stickInput_.clear();
	mouseInput_.clear();
	delete instance_;
}

void KeyConfig::PadVibration(KeyConfig::JOYPAD_NO _no, int _time, int _pow)
{
	int InputType = _no == JOYPAD_NO::PAD1 ? DX_INPUT_PAD1 :
		_no == JOYPAD_NO::PAD2 ? DX_INPUT_PAD2 :
		_no == JOYPAD_NO::PAD3 ? DX_INPUT_PAD3 : DX_INPUT_PAD4;
	StartJoypadVibration(InputType, _pow, _time);
}

void KeyConfig::StopPadVibration(KeyConfig::JOYPAD_NO _no)
{
	int InputType = _no == JOYPAD_NO::PAD1 ? DX_INPUT_PAD1 :
		_no == JOYPAD_NO::PAD2 ? DX_INPUT_PAD2 :
		_no == JOYPAD_NO::PAD3 ? DX_INPUT_PAD3 : DX_INPUT_PAD4;
	StopJoypadVibration(InputType);
}

KeyConfig::KeyConfig(void)
{
}

KeyConfig::KeyConfig(const KeyConfig& manager)
{
}
