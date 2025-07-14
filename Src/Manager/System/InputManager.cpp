#include <DxLib.h>
#include <cmath>
#include "../Application.h"
#include "InputManager.h"
void InputManager::Init(void)
{

	// ゲームで使用したいキーを、
	// 事前にここで登録しておいてください
	Add(KEY_INPUT_SPACE);
	Add(KEY_INPUT_N);
	Add(KEY_INPUT_M);
	Add(KEY_INPUT_Z);
	Add(KEY_INPUT_X);
	Add(KEY_INPUT_G);

	Add(KEY_INPUT_UP);
	Add(KEY_INPUT_RIGHT);
	Add(KEY_INPUT_DOWN);
	Add(KEY_INPUT_LEFT);
	Add(KEY_INPUT_RCONTROL);

	Add(KEY_INPUT_B);
	Add(KEY_INPUT_BACK);

	//プレイヤー操作関連
	//---------------------------------------------------
	//移動キー
	Add(KEY_INPUT_W);
	Add(KEY_INPUT_A);
	Add(KEY_INPUT_S);
	Add(KEY_INPUT_D);
	Add(KEY_INPUT_Q);
	Add(KEY_INPUT_R);
	//ダッシュ
	Add(KEY_INPUT_LSHIFT);

	Add(KEY_INPUT_LCONTROL);
	//ジャンプ
	Add(KEY_INPUT_E);

	//パンチキー
	Add(KEY_INPUT_RETURN);
	Add(KEY_INPUT_T);
	Add(KEY_INPUT_Y);

	//タブキー
	Add(KEY_INPUT_TAB);

	//---------------------------------------------------

	//デバッグ
	Add(KEY_INPUT_C);	//ゲームクリアへの遷移
	Add(KEY_INPUT_RSHIFT);	//タイトルシーン遷移


	InputManager::MouseInfo mouseInfo;

	// マウス
	mouseInfo = InputManager::MouseInfo();
	for (int i = 0;i < static_cast<int>(KeyConfig::MOUSE::MAX);i++)
	{
		mouseInfos_.emplace(static_cast<KeyConfig::MOUSE>(i), mouseInfo);
	}

	//スティック
	InputManager::StickInfo stickInfo;
	stickInfo = InputManager::StickInfo();
	for (int padNo = 0;padNo <= static_cast<int>(KeyConfig::JOYPAD_NO::PAD4);padNo++)
	{
		std::vector<StickInfo>sticks;
		for (int i = 0; i < static_cast<int>(KeyConfig::JOYPAD_STICK::MAX);i++)
		{
			stickInfo.key = static_cast<KeyConfig::JOYPAD_STICK>(i);
			sticks.push_back(stickInfo);
		}
		stickInfos_.emplace(static_cast<KeyConfig::JOYPAD_NO>(padNo), sticks);
	}
}

void InputManager::Update(void)
{

	// キーボード検知
	for (auto& p : keyInfos_)
	{
		p.second.keyOld = p.second.keyNew;
		p.second.keyNew = CheckHitKey(p.second.key);
		p.second.keyTrgDown = p.second.keyNew && !p.second.keyOld;
		p.second.keyTrgUp = !p.second.keyNew && p.second.keyOld;
	}

	// マウス検知
	mouseInput_ = GetMouseInput();
	mousePrePos_ = mousePos_;
	GetMousePoint(&mousePos_.x, &mousePos_.y);
	wheelRot_ = GetMouseWheelRotVol();

	for (auto& p : mouseInfos_)
	{
		p.second.keyOld = p.second.keyNew;
		switch (p.first)
		{
		case KeyConfig::MOUSE::CLICK_RIGHT:
			p.second.keyNew = (mouseInput_ & MOUSE_INPUT_RIGHT) != 0;
			break;
		case KeyConfig::MOUSE::CLICK_LEFT:
			p.second.keyNew = (mouseInput_ & MOUSE_INPUT_LEFT) != 0;
			break;
		case KeyConfig::MOUSE::MOVE_LEFT:
			p.second.keyNew = mousePos_.x < mousePrePos_.x;
			break;
		case KeyConfig::MOUSE::MOVE_RIGHT:
			p.second.keyNew = mousePos_.x > mousePrePos_.x;
			break;
		case KeyConfig::MOUSE::MOVE_UP:
			p.second.keyNew = mousePos_.y < mousePrePos_.y;
			break;
		case KeyConfig::MOUSE::MOVE_DOWN:
			p.second.keyNew = mousePos_.y > mousePrePos_.y;
			break;
		case KeyConfig::MOUSE::WHEEL_FRONT:
			p.second.keyNew = (wheelRot_ > 0);
			break;
		case KeyConfig::MOUSE::WHEEL_BACK:
			p.second.keyNew = (wheelRot_ < 0);
			break;
		case KeyConfig::MOUSE::MAX:
			break;
		default:
			break;
		}
		p.second.keyTrgDown = p.second.keyNew && !p.second.keyOld;
		p.second.keyTrgUp = !p.second.keyNew && p.second.keyOld;
	}

	// パッド情報
	SetJPadInState(KeyConfig::JOYPAD_NO::KEY_PAD1);
	SetJPadInState(KeyConfig::JOYPAD_NO::PAD1);
	SetJPadInState(KeyConfig::JOYPAD_NO::PAD2);
	SetJPadInState(KeyConfig::JOYPAD_NO::PAD3);
	SetJPadInState(KeyConfig::JOYPAD_NO::PAD4);
	for (auto& stickInfo : stickInfos_)
	{
		for (auto& stick : stickInfo.second)
		{
			int overSize = KeyConfig::GetInstance().PadStickOverSize(stickInfo.first, stick.key);
			stick.keyOld = stick.keyNew;
			stick.keyNew = overSize > STICK_THRESHOLD;
			stick.keyTrgDown = !stick.keyOld && stick.keyNew;
			stick.keyTrgUp = stick.keyOld && !stick.keyNew;
		}
	}
}

void InputManager::Release(void)
{
	keyInfos_.clear();
	mouseInfos_.clear();
	stickInfos_.clear();
}

void InputManager::Add(int key)
{
	InputManager::Info info = InputManager::Info();
	info.key = key;
	info.keyOld = false;
	info.keyNew = false;
	info.keyTrgDown = false;
	info.keyTrgUp = false;
	keyInfos_.emplace(key, info);
}

void InputManager::Clear(void)
{
	keyInfos_.clear();
}

bool InputManager::IsNew(int key) const
{
	return Find(key).keyNew;
}

bool InputManager::IsTrgDown(int key) const
{
	return Find(key).keyTrgDown;
}

bool InputManager::IsTrgUp(int key) const
{
	return Find(key).keyTrgUp;
}

Vector2 InputManager::GetMousePos(void) const
{
	return mousePos_;
}

Vector2 InputManager::GetMousePosDistance(void) const
{
	return { mousePos_.x - mousePrePos_.x ,mousePos_.y - mousePrePos_.y };
}

void InputManager::SetMousePosScreen(void)
{
	mousePos_.x = (mousePos_.x + Application::SCREEN_SIZE_X * 2)% Application::SCREEN_SIZE_X;
	mousePos_.y = (mousePos_.y + Application::SCREEN_SIZE_Y * 2) % Application::SCREEN_SIZE_Y;
	mousePrePos_.x = (mousePrePos_.x + Application::SCREEN_SIZE_X * 2) % Application::SCREEN_SIZE_X;
	mousePrePos_.y = (mousePrePos_.y + Application::SCREEN_SIZE_Y * 2) % Application::SCREEN_SIZE_Y;
	SetMousePoint(static_cast<int>(mousePos_.x), static_cast<int>(mousePos_.y));
}

void InputManager::SetMousePos(const Vector2& pos)
{
	mousePos_ = pos;
	mousePrePos_ = pos;
	SetMousePoint(static_cast<int>(mousePos_.x), static_cast<int>(mousePos_.y));
}

int InputManager::GetMouse(void) const
{
	return mouseInput_;
}

InputManager::InputManager(void)
{
}

InputManager::~InputManager(void)
{
}

const InputManager::Info& InputManager::Find(int key) const
{

	auto it = keyInfos_.find(key);
	if (it != keyInfos_.end())
	{
		return it->second;
	}

	return infoEmpty_;

}

const InputManager::MouseInfo& InputManager::FindMouse(KeyConfig::MOUSE key) const
{
	auto it = mouseInfos_.find(key);
	if (it != mouseInfos_.end())
	{
		return it->second;
	}

	return mouseInfoEmpty_;
}

InputManager::JOYPAD_TYPE InputManager::GetJPadType(KeyConfig::JOYPAD_NO no)
{
	return static_cast<InputManager::JOYPAD_TYPE>(GetJoypadType(static_cast<int>(no)));
}

DINPUT_JOYSTATE InputManager::GetJPadDInputState(KeyConfig::JOYPAD_NO no)
{
	// コントローラ情報
	GetJoypadDirectInputState(static_cast<int>(no), &joyDInState_);
	return joyDInState_;
}

XINPUT_STATE InputManager::GetJPadXInputState(KeyConfig::JOYPAD_NO no)
{
	// コントローラ情報
	GetJoypadXInputState(static_cast<int>(no), &joyXInState_);
	return joyXInState_;
}

void InputManager::SetJPadInState(KeyConfig::JOYPAD_NO jpNo)
{

	int no = static_cast<int>(jpNo);
	auto stateNew = GetJPadInputState(jpNo);
	auto& stateNow = padInfos_[no];

	int max = static_cast<int>(KeyConfig::JOYPAD_BTN::MAX);
	for (int i = 0; i < max; i++)
	{

		stateNow.ButtonsOld[i] = stateNow.ButtonsNew[i];
		stateNow.ButtonsNew[i] = stateNew.ButtonsNew[i];

		stateNow.IsOld[i] = stateNow.IsNew[i];
		//stateNow.IsNew[i] = stateNow.ButtonsNew[i] == 128 || stateNow.ButtonsNew[i] == 255;
		stateNow.IsNew[i] = stateNow.ButtonsNew[i] > 0;

		stateNow.IsTrgDown[i] = stateNow.IsNew[i] && !stateNow.IsOld[i];
		stateNow.IsTrgUp[i] = !stateNow.IsNew[i] && stateNow.IsOld[i];


		stateNow.AKeyLX = stateNew.AKeyLX;
		stateNow.AKeyLY = stateNew.AKeyLY;
		stateNow.AKeyRX = stateNew.AKeyRX;
		stateNow.AKeyRY = stateNew.AKeyRY;

	}

}

InputManager::JOYPAD_IN_STATE InputManager::GetJPadInputState(KeyConfig::JOYPAD_NO no)
{

	JOYPAD_IN_STATE ret = JOYPAD_IN_STATE();

	auto type = GetJPadType(no);

	switch (type)
	{
	case InputManager::JOYPAD_TYPE::OTHER:
		break;
	case InputManager::JOYPAD_TYPE::XBOX_360:
	{
	}
	break;
	case InputManager::JOYPAD_TYPE::XBOX_ONE:
	{

		auto d = GetJPadDInputState(no);
		auto x = GetJPadXInputState(no);

		int idx;

		//   Y
		// X   B
		//   A

		idx = static_cast<int>(KeyConfig::JOYPAD_BTN::RIGHTBUTTON_TOP);
		ret.ButtonsNew[idx] = d.Buttons[3];// Y

		idx = static_cast<int>(KeyConfig::JOYPAD_BTN::RIGHTBUTTON_LEFT);
		ret.ButtonsNew[idx] = d.Buttons[2];// X

		idx = static_cast<int>(KeyConfig::JOYPAD_BTN::RIGHTBUTTON_RIGHT);
		ret.ButtonsNew[idx] = d.Buttons[1];// B

		idx = static_cast<int>(KeyConfig::JOYPAD_BTN::RIGHTBUTTON_DOWN);
		ret.ButtonsNew[idx] = d.Buttons[0];// A

		idx = static_cast<int>(KeyConfig::JOYPAD_BTN::R_TRIGGER);
		ret.ButtonsNew[idx] = x.RightTrigger;// R_TRIGGER

		idx = static_cast<int>(KeyConfig::JOYPAD_BTN::L_TRIGGER);
		ret.ButtonsNew[idx] = x.LeftTrigger; // L_TRIGGER

		idx = static_cast<int>(KeyConfig::JOYPAD_BTN::R_BUTTON);
		ret.ButtonsNew[idx] = d.Buttons[5];// R_BUTTON

		idx = static_cast<int>(KeyConfig::JOYPAD_BTN::L_BUTTON);
		ret.ButtonsNew[idx] = d.Buttons[4]; // L_BUTTON

		idx = static_cast<int>(KeyConfig::JOYPAD_BTN::START_BUTTON);
		ret.ButtonsNew[idx] = d.Buttons[7];// START

		idx = static_cast<int>(KeyConfig::JOYPAD_BTN::SELECT_BUTTON);
		ret.ButtonsNew[idx] = d.Buttons[6]; // SELECT

		idx = static_cast<int>(KeyConfig::JOYPAD_BTN::LEFTBUTTON_TOP);
		ret.ButtonsNew[idx] = x.Buttons[0]; // LEFTBUTTON_TOP

		idx = static_cast<int>(KeyConfig::JOYPAD_BTN::LEFTBUTTON_DOWN);
		ret.ButtonsNew[idx] = x.Buttons[1]; // LEFTBUTTON_DOWN

		idx = static_cast<int>(KeyConfig::JOYPAD_BTN::LEFTBUTTON_LEFT);
		ret.ButtonsNew[idx] = x.Buttons[2]; // LEFTBUTTON_LEFT

		idx = static_cast<int>(KeyConfig::JOYPAD_BTN::LEFTBUTTON_RIGHT);
		ret.ButtonsNew[idx] = x.Buttons[3]; // LEFTBUTTON_RIGHT

		idx = static_cast<int>(KeyConfig::JOYPAD_BTN::LEFT_STICK);
		ret.ButtonsNew[idx] = x.Buttons[6]; // 左スティック押し込み

		idx = static_cast<int>(KeyConfig::JOYPAD_BTN::RIGHT_STICK);
		ret.ButtonsNew[idx] = x.Buttons[7]; // 右スティック押し込み

		// 左スティック
		ret.AKeyLX = d.X;
		ret.AKeyLY = d.Y;

		// 右スティック
		ret.AKeyRX = d.Rx;
		ret.AKeyRY = d.Ry;

	}
	break;
	case InputManager::JOYPAD_TYPE::DUAL_SHOCK_4:
		break;
	case InputManager::JOYPAD_TYPE::DUAL_SENSE:
	{

		auto d = GetJPadDInputState(no);
		int idx;

		//   △
		// □  〇
		//   ×

		idx = static_cast<int>(KeyConfig::JOYPAD_BTN::RIGHTBUTTON_TOP);
		ret.ButtonsNew[idx] = d.Buttons[3];// △

		idx = static_cast<int>(KeyConfig::JOYPAD_BTN::RIGHTBUTTON_LEFT);
		ret.ButtonsNew[idx] = d.Buttons[0];// □

		idx = static_cast<int>(KeyConfig::JOYPAD_BTN::RIGHTBUTTON_RIGHT);
		ret.ButtonsNew[idx] = d.Buttons[2];// 〇

		idx = static_cast<int>(KeyConfig::JOYPAD_BTN::RIGHTBUTTON_DOWN);
		ret.ButtonsNew[idx] = d.Buttons[1];// ×

		// 左スティック
		ret.AKeyLX = d.X;
		ret.AKeyLY = d.Y;

		// 右スティック
		ret.AKeyRX = d.Z;
		ret.AKeyRY = d.Rz;

	}
	break;
	case InputManager::JOYPAD_TYPE::SWITCH_JOY_CON_L:
		break;
	case InputManager::JOYPAD_TYPE::SWITCH_JOY_CON_R:
		break;
	case InputManager::JOYPAD_TYPE::SWITCH_PRO_CTRL:
		break;
	case InputManager::JOYPAD_TYPE::MAX:
		break;
	}

	return ret;

}

bool InputManager::IsPadBtnNew(KeyConfig::JOYPAD_NO no, KeyConfig::JOYPAD_BTN btn) const
{
	return padInfos_[static_cast<int>(no)].IsNew[static_cast<int>(btn)];
}

bool InputManager::IsPadBtnTrgDown(KeyConfig::JOYPAD_NO no, KeyConfig::JOYPAD_BTN btn) const
{
	return padInfos_[static_cast<int>(no)].IsTrgDown[static_cast<int>(btn)];
}

bool InputManager::IsPadBtnTrgUp(KeyConfig::JOYPAD_NO no, KeyConfig::JOYPAD_BTN btn) const
{
	return padInfos_[static_cast<int>(no)].IsTrgUp[static_cast<int>(btn)];
}

bool InputManager::IsStickNew(KeyConfig::JOYPAD_NO no, KeyConfig::JOYPAD_STICK stick) const
{
	for (auto& stickInfo : stickInfos_)
	{
		if (stickInfo.first != no)
		{
			continue;
		}
		for (auto& stickI : stickInfo.second)
		{
			if (stickI.key != stick)
			{
				continue;
			}
			return stickI.keyNew;
		}
	}
	return false;
}

bool InputManager::IsStickDown(KeyConfig::JOYPAD_NO no, KeyConfig::JOYPAD_STICK stick) const
{
	for (auto& stickInfo : stickInfos_)
	{
		if (stickInfo.first != no)
		{
			continue;
		}
		for (auto& stickI : stickInfo.second)
		{
			if (stickI.key != stick)
			{
				continue;
			}
			return stickI.keyTrgDown;
		}
	}
	return false;
}

bool InputManager::IsStickUp(KeyConfig::JOYPAD_NO no, KeyConfig::JOYPAD_STICK stick) const
{
	for (auto& stickInfo : stickInfos_)
	{
		if (stickInfo.first != no)
		{
			continue;
		}
		for (auto& stickI : stickInfo.second)
		{
			if (stickI.key != stick)
			{
				continue;
			}
			return stickI.keyTrgUp;
		}
	}
	return false;
}

bool InputManager::IsMouseNew(KeyConfig::MOUSE mouse)
{
	return FindMouse(mouse).keyNew;
}

bool InputManager::IsMouseTrgUp(KeyConfig::MOUSE mouse)
{
	return FindMouse(mouse).keyTrgUp;
}

bool InputManager::IsMouseTrgDown(KeyConfig::MOUSE mouse)
{
	return FindMouse(mouse).keyTrgDown;
}


