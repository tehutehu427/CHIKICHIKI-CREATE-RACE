#include <DxLib.h>
#include "InputManager.h"

InputManager* InputManager::instance_ = nullptr;

void InputManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new InputManager();
	}
	instance_->Init();
}

InputManager& InputManager::GetInstance(void)
{
	if (instance_ == nullptr)
	{
		InputManager::CreateInstance();
	}
	return *instance_;
}

void InputManager::Init(void)
{

	// ゲームで使用したいキーを、
	// 事前にここで登録しておいてください
	InputManager::GetInstance().Add(KEY_INPUT_SPACE);
	InputManager::GetInstance().Add(KEY_INPUT_N);
	InputManager::GetInstance().Add(KEY_INPUT_Z);
	InputManager::GetInstance().Add(KEY_INPUT_X);

	InputManager::GetInstance().Add(KEY_INPUT_UP);
	InputManager::GetInstance().Add(KEY_INPUT_RIGHT);
	InputManager::GetInstance().Add(KEY_INPUT_DOWN);
	InputManager::GetInstance().Add(KEY_INPUT_LEFT);
	InputManager::GetInstance().Add(KEY_INPUT_RCONTROL);

	InputManager::GetInstance().Add(KEY_INPUT_B);

	//プレイヤー操作関連
	//---------------------------------------------------
	//移動キー
	InputManager::GetInstance().Add(KEY_INPUT_W);
	InputManager::GetInstance().Add(KEY_INPUT_A);
	InputManager::GetInstance().Add(KEY_INPUT_S);
	InputManager::GetInstance().Add(KEY_INPUT_D);
	InputManager::GetInstance().Add(KEY_INPUT_Q);
	InputManager::GetInstance().Add(KEY_INPUT_R);
	//ダッシュ
	InputManager::GetInstance().Add(KEY_INPUT_LSHIFT);

	InputManager::GetInstance().Add(KEY_INPUT_LCONTROL);
	//ジャンプ
	InputManager::GetInstance().Add(KEY_INPUT_E);

	//パンチキー
	InputManager::GetInstance().Add(KEY_INPUT_RETURN);
	InputManager::GetInstance().Add(KEY_INPUT_T);
	InputManager::GetInstance().Add(KEY_INPUT_Y);

	//---------------------------------------------------

	//デバッグ
	InputManager::GetInstance().Add(KEY_INPUT_C);	//ゲームクリアへの遷移
	InputManager::GetInstance().Add(KEY_INPUT_RSHIFT);	//タイトルシーン遷移


	InputManager::MouseInfo mouseInfo;

	// マウス
	mouseInfo = InputManager::MouseInfo();
	for (int i = 0;i < static_cast<int>(InputManager::MOUSE::MAX);i++)
	{
		mouseInfos_.emplace(static_cast<MOUSE>(i), mouseInfo);
	}

	//スティック
	InputManager::StickInfo stickInfo;
	stickInfo = InputManager::StickInfo();
	for (int padNo = 0;padNo <= static_cast<int>(JOYPAD_NO::PAD4);padNo++)
	{
		std::vector<StickInfo>sticks;
		for (int i = 0; i < static_cast<int>(InputManager::JOYPAD_STICK::MAX);i++)
		{
			stickInfo.key = static_cast<JOYPAD_STICK>(i);
			sticks.push_back(stickInfo);
		}
		stickInfos_.emplace(static_cast<JOYPAD_NO>(padNo), sticks);
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
		case InputManager::MOUSE::CLICK_RIGHT:
			p.second.keyNew = (mouseInput_ & MOUSE_INPUT_RIGHT) != 0;
			break;
		case InputManager::MOUSE::CLICK_LEFT:
			p.second.keyNew = (mouseInput_ & MOUSE_INPUT_LEFT) != 0;
			break;
		case InputManager::MOUSE::MOVE_LEFT:
			p.second.keyNew = mousePos_.x < mousePrePos_.x;
			break;
		case InputManager::MOUSE::MOVE_RIGHT:
			p.second.keyNew = mousePos_.x > mousePrePos_.x;
			break;
		case InputManager::MOUSE::MOVE_UP:
			p.second.keyNew = mousePos_.y < mousePrePos_.y;
			break;
		case InputManager::MOUSE::MOVE_DOWN:
			p.second.keyNew = mousePos_.y > mousePrePos_.y;
			break;
		case InputManager::MOUSE::WHEEL_FRONT:
			p.second.keyNew = (wheelRot_ > 0);
			break;
		case InputManager::MOUSE::WHEEL_BACK:
			p.second.keyNew = (wheelRot_ < 0);
			break;
		case InputManager::MOUSE::MAX:
			break;
		default:
			break;
		}
		p.second.keyTrgDown = p.second.keyNew && !p.second.keyOld;
		p.second.keyTrgUp = !p.second.keyNew && p.second.keyOld;
	}

	// パッド情報
	SetJPadInState(JOYPAD_NO::KEY_PAD1);
	SetJPadInState(JOYPAD_NO::PAD1);
	SetJPadInState(JOYPAD_NO::PAD2);
	SetJPadInState(JOYPAD_NO::PAD3);
	SetJPadInState(JOYPAD_NO::PAD4);
	for (auto& stickInfo : stickInfos_)
	{
		for (auto& stick : stickInfo.second)
		{
			int overSize = PadStickOverSize(stickInfo.first, stick.key);
			stick.keyOld = stick.keyNew;
			stick.keyNew = overSize > STICK_THRESHOLD;
			stick.keyTrgDown = !stick.keyOld && stick.keyNew;
			stick.keyTrgUp = stick.keyOld && !stick.keyNew;
		}
	}
}

void InputManager::Destroy(void)
{
	keyInfos_.clear();
	mouseInfos_.clear();
	delete instance_;
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

int InputManager::GetMouse(void) const
{
	return mouseInput_;
}

InputManager::InputManager(void)
{
	mouseInput_ = -1;
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

const InputManager::MouseInfo& InputManager::FindMouse(MOUSE key) const
{
	auto it = mouseInfos_.find(key);
	if (it != mouseInfos_.end())
	{
		return it->second;
	}

	return mouseInfoEmpty_;
}

InputManager::JOYPAD_TYPE InputManager::GetJPadType(JOYPAD_NO no)
{
	return static_cast<InputManager::JOYPAD_TYPE>(GetJoypadType(static_cast<int>(no)));
}

DINPUT_JOYSTATE InputManager::GetJPadDInputState(JOYPAD_NO no)
{
	// コントローラ情報
	GetJoypadDirectInputState(static_cast<int>(no), &joyDInState_);
	return joyDInState_;
}

XINPUT_STATE InputManager::GetJPadXInputState(JOYPAD_NO no)
{
	// コントローラ情報
	GetJoypadXInputState(static_cast<int>(no), &joyXInState_);
	return joyXInState_;
}

void InputManager::SetJPadInState(JOYPAD_NO jpNo)
{

	int no = static_cast<int>(jpNo);
	auto stateNew = GetJPadInputState(jpNo);
	auto& stateNow = padInfos_[no];

	int max = static_cast<int>(JOYPAD_BTN::MAX);
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

InputManager::JOYPAD_IN_STATE InputManager::GetJPadInputState(JOYPAD_NO no)
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

		idx = static_cast<int>(JOYPAD_BTN::RIGHTBUTTON_TOP);
		ret.ButtonsNew[idx] = d.Buttons[3];// Y

		idx = static_cast<int>(JOYPAD_BTN::RIGHTBUTTON_LEFT);
		ret.ButtonsNew[idx] = d.Buttons[2];// X

		idx = static_cast<int>(JOYPAD_BTN::RIGHTBUTTON_RIGHT);
		ret.ButtonsNew[idx] = d.Buttons[1];// B

		idx = static_cast<int>(JOYPAD_BTN::RIGHTBUTTON_DOWN);
		ret.ButtonsNew[idx] = d.Buttons[0];// A

		idx = static_cast<int>(JOYPAD_BTN::R_TRIGGER);
		ret.ButtonsNew[idx] = x.RightTrigger;// R_TRIGGER

		idx = static_cast<int>(JOYPAD_BTN::L_TRIGGER);
		ret.ButtonsNew[idx] = x.LeftTrigger; // L_TRIGGER

		idx = static_cast<int>(JOYPAD_BTN::R_BUTTON);
		ret.ButtonsNew[idx] = d.Buttons[5];// R_BUTTON

		idx = static_cast<int>(JOYPAD_BTN::L_BUTTON);
		ret.ButtonsNew[idx] = d.Buttons[4]; // L_BUTTON

		idx = static_cast<int>(JOYPAD_BTN::START_BUTTON);
		ret.ButtonsNew[idx] = d.Buttons[7];// START

		idx = static_cast<int>(JOYPAD_BTN::SELECT_BUTTON);
		ret.ButtonsNew[idx] = d.Buttons[6]; // SELECT

		idx = static_cast<int>(JOYPAD_BTN::LEFTBUTTON_TOP);
		ret.ButtonsNew[idx] = x.Buttons[0]; // LEFTBUTTON_TOP

		idx = static_cast<int>(JOYPAD_BTN::LEFTBUTTON_DOWN);
		ret.ButtonsNew[idx] = x.Buttons[1]; // LEFTBUTTON_DOWN

		idx = static_cast<int>(JOYPAD_BTN::LEFTBUTTON_LEFT);
		ret.ButtonsNew[idx] = x.Buttons[2]; // LEFTBUTTON_LEFT

		idx = static_cast<int>(JOYPAD_BTN::LEFTBUTTON_RIGHT);
		ret.ButtonsNew[idx] = x.Buttons[3]; // LEFTBUTTON_RIGHT

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

		idx = static_cast<int>(JOYPAD_BTN::RIGHTBUTTON_TOP);
		ret.ButtonsNew[idx] = d.Buttons[3];// △

		idx = static_cast<int>(JOYPAD_BTN::RIGHTBUTTON_LEFT);
		ret.ButtonsNew[idx] = d.Buttons[0];// □

		idx = static_cast<int>(JOYPAD_BTN::RIGHTBUTTON_RIGHT);
		ret.ButtonsNew[idx] = d.Buttons[2];// 〇

		idx = static_cast<int>(JOYPAD_BTN::RIGHTBUTTON_DOWN);
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

bool InputManager::IsPadBtnNew(JOYPAD_NO no, JOYPAD_BTN btn) const
{
	return padInfos_[static_cast<int>(no)].IsNew[static_cast<int>(btn)];
}

bool InputManager::IsPadBtnTrgDown(JOYPAD_NO no, JOYPAD_BTN btn) const
{
	return padInfos_[static_cast<int>(no)].IsTrgDown[static_cast<int>(btn)];
}

bool InputManager::IsPadBtnTrgUp(JOYPAD_NO no, JOYPAD_BTN btn) const
{
	return padInfos_[static_cast<int>(no)].IsTrgUp[static_cast<int>(btn)];
}

bool InputManager::IsStickNew(JOYPAD_NO no, JOYPAD_STICK stick) const
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

bool InputManager::IsStickDown(JOYPAD_NO no, JOYPAD_STICK stick) const
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

bool InputManager::IsStickUp(JOYPAD_NO no, JOYPAD_STICK stick) const
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

bool InputManager::IsMouseNew(MOUSE mouse)
{
	return FindMouse(mouse).keyNew;
}

bool InputManager::IsMouseTrgUp(MOUSE mouse)
{
	return FindMouse(mouse).keyTrgUp;
}

bool InputManager::IsMouseTrgDown(MOUSE mouse)
{
	return FindMouse(mouse).keyTrgDown;
}

int InputManager::PadStickOverSize(JOYPAD_NO no, JOYPAD_STICK stick) const
{
	int ret = 0;
	switch (stick)
	{
	case InputManager::JOYPAD_STICK::L_STICK_UP:
		ret = padInfos_[static_cast<int>(no)].AKeyLY < 0 ? padInfos_[static_cast<int>(no)].AKeyLY : 0;
		break;
	case InputManager::JOYPAD_STICK::L_STICK_DOWN:
		ret = padInfos_[static_cast<int>(no)].AKeyLY > 0 ? padInfos_[static_cast<int>(no)].AKeyLY : 0;
		break;
	case InputManager::JOYPAD_STICK::L_STICK_LEFT:
		ret = padInfos_[static_cast<int>(no)].AKeyLX < 0 ? padInfos_[static_cast<int>(no)].AKeyLX : 0;
		break;
	case InputManager::JOYPAD_STICK::L_STICK_RIGHT:
		ret = padInfos_[static_cast<int>(no)].AKeyLX > 0 ? padInfos_[static_cast<int>(no)].AKeyLX : 0;
		break;
	case InputManager::JOYPAD_STICK::R_STICK_UP:
		ret = padInfos_[static_cast<int>(no)].AKeyRY < 0 ? padInfos_[static_cast<int>(no)].AKeyRY : 0;
		break;
	case InputManager::JOYPAD_STICK::R_STICK_DOWN:
		ret = padInfos_[static_cast<int>(no)].AKeyRY > 0 ? padInfos_[static_cast<int>(no)].AKeyRY : 0;
		break;
	case InputManager::JOYPAD_STICK::R_STICK_LEFT:
		ret = padInfos_[static_cast<int>(no)].AKeyRX < 0 ? padInfos_[static_cast<int>(no)].AKeyRX : 0;
		break;
	case InputManager::JOYPAD_STICK::R_STICK_RIGHT:
		ret = padInfos_[static_cast<int>(no)].AKeyRX > 0 ? padInfos_[static_cast<int>(no)].AKeyRX : 0;
		break;
	case InputManager::JOYPAD_STICK::MAX:
		break;
	default:
		break;
	}
	return abs(ret);
}
