#include "KeyConfig.h"

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
	KeyConfig::GetInstance().Add(InputManager::JOYPAD_BTN::LEFTBUTTON_DOWN, CONTROL_TYPE::SELECT_DOWN);
	KeyConfig::GetInstance().Add(InputManager::JOYPAD_BTN::LEFTBUTTON_DOWN, CONTROL_TYPE::EDIT_CAMERA_ROT_DOWN);
	KeyConfig::GetInstance().Add(InputManager::JOYPAD_BTN::LEFTBUTTON_DOWN, CONTROL_TYPE::PLAY_CAMERA_MOVE_DOWN);
	KeyConfig::GetInstance().Add(InputManager::JOYPAD_BTN::LEFTBUTTON_TOP, CONTROL_TYPE::SELECT_UP);
	KeyConfig::GetInstance().Add(InputManager::JOYPAD_BTN::LEFTBUTTON_TOP, CONTROL_TYPE::EDIT_CAMERA_ROT_UP);
	KeyConfig::GetInstance().Add(InputManager::JOYPAD_BTN::LEFTBUTTON_TOP, CONTROL_TYPE::PLAY_CAMERA_MOVE_UP);
	KeyConfig::GetInstance().Add(InputManager::JOYPAD_BTN::LEFTBUTTON_LEFT, CONTROL_TYPE::SELECT_LEFT);
	KeyConfig::GetInstance().Add(InputManager::JOYPAD_BTN::LEFTBUTTON_LEFT, CONTROL_TYPE::EDIT_CAMERA_ROT_LEFT);
	KeyConfig::GetInstance().Add(InputManager::JOYPAD_BTN::LEFTBUTTON_LEFT, CONTROL_TYPE::PLAY_CAMERA_MOVE_LEFT);
	KeyConfig::GetInstance().Add(InputManager::JOYPAD_BTN::LEFTBUTTON_RIGHT, CONTROL_TYPE::SELECT_RIGHT);
	KeyConfig::GetInstance().Add(InputManager::JOYPAD_BTN::LEFTBUTTON_RIGHT, CONTROL_TYPE::EDIT_CAMERA_ROT_RIGHT);
	KeyConfig::GetInstance().Add(InputManager::JOYPAD_BTN::LEFTBUTTON_RIGHT, CONTROL_TYPE::PLAY_CAMERA_MOVE_RIGHT);
	
	KeyConfig::GetInstance().Add(InputManager::JOYPAD_BTN::RIGHTBUTTON_RIGHT, CONTROL_TYPE::ENTER);
	KeyConfig::GetInstance().Add(InputManager::JOYPAD_BTN::RIGHTBUTTON_RIGHT, CONTROL_TYPE::PLAYER_JUMP);
	KeyConfig::GetInstance().Add(InputManager::JOYPAD_BTN::RIGHTBUTTON_DOWN, CONTROL_TYPE::CANCEL);
	KeyConfig::GetInstance().Add(InputManager::JOYPAD_BTN::RIGHTBUTTON_LEFT, CONTROL_TYPE::PLAYER_PUNCH);

	KeyConfig::GetInstance().Add(KEY_INPUT_LCONTROL, CONTROL_TYPE::CANCEL);
	KeyConfig::GetInstance().Add(KEY_INPUT_SPACE, CONTROL_TYPE::ENTER);
}

void KeyConfig::Update(void)
{
}

bool KeyConfig::IsNew(CONTROL_TYPE cType, InputManager::JOYPAD_NO no)
{
	for (auto &key : keyInput_)
	{
		if (key.first != cType)
		{
			continue;
		}
		for (auto keyI : key.second)
		{
			if (InputManager::GetInstance().IsNew(keyI))
			{
				return true;
			}
		}
	}
	for (auto& con : conInput_)
	{
		if (con.first != cType)
		{
			continue;
		}
		for (auto conI : con.second)
		{
			if (InputManager::GetInstance().IsPadBtnNew(no, conI))
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
			if (InputManager::GetInstance().IsStickNew(no, stickI))
			{
				return true;
			}
		}
	}
	return false;
}

bool KeyConfig::IsTrgDown(CONTROL_TYPE cType, InputManager::JOYPAD_NO no)
{
	for (auto& key : keyInput_)
	{
		if (key.first != cType)
		{
			continue;
		}
		for (auto keyI : key.second)
		{
			if (InputManager::GetInstance().IsTrgDown(keyI))
			{
				return true;
			}
		}
	}
	for (auto& con : conInput_)
	{
		if (con.first != cType)
		{
			continue;
		}
		for (auto conI : con.second)
		{
			if (InputManager::GetInstance().IsPadBtnTrgDown(no, conI))
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
			if (InputManager::GetInstance().IsStickDown(no, stickI))
			{
				return true;
			}
		}
	}
	return false;
}

bool KeyConfig::IsTrgUp(CONTROL_TYPE cType, InputManager::JOYPAD_NO no)
{
	for (auto& key : keyInput_)
	{
		if (key.first != cType)
		{
			continue;
		}
		for (auto keyI : key.second)
		{
			if (InputManager::GetInstance().IsTrgUp(keyI))
			{
				return true;
			}
		}
	}
	for (auto& con : conInput_)
	{
		if (con.first != cType)
		{
			continue;
		}
		for (auto conI : con.second)
		{
			if (InputManager::GetInstance().IsPadBtnTrgUp(no, conI))
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
			if (InputManager::GetInstance().IsStickUp(no, stickI))
			{
				return true;
			}
		}
	}
	return false;
}

void KeyConfig::Add(int key, CONTROL_TYPE type)
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

void KeyConfig::Add(InputManager::JOYPAD_BTN key, CONTROL_TYPE type)
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

	std::vector<InputManager::JOYPAD_BTN> cons;
	cons.emplace_back(key);
	conInput_.emplace(type, cons);
}

void KeyConfig::Add(InputManager::JOYPAD_STICK key, CONTROL_TYPE type)
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

	std::vector<InputManager::JOYPAD_STICK> sticks;
	sticks.emplace_back(key);
	stickInput_.emplace(type, sticks);
}

void KeyConfig::Destroy(void)
{
	keyInput_.clear();
	conInput_.clear();
	delete instance_;
}

KeyConfig::KeyConfig(void)
{
}

KeyConfig::KeyConfig(const KeyConfig& manager)
{
}
