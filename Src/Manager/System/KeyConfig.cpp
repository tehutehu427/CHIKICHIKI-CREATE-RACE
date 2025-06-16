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
	Add(CONTROL_TYPE::ENTER, KEY_INPUT_SPACE);
	Add(CONTROL_TYPE::ENTER, InputManager::MOUSE::CLICK_LEFT);
	Add(CONTROL_TYPE::ENTER, InputManager::JOYPAD_BTN::RIGHTBUTTON_RIGHT);
}

void KeyConfig::Update(void)
{
}

bool KeyConfig::IsNew(CONTROL_TYPE cType, InputManager::JOYPAD_NO no,TYPE type)
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
				if (InputManager::GetInstance().IsNew(keyI))
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
				if (InputManager::GetInstance().IsMouseNew(mouseI))
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
	}
	return false;
}

bool KeyConfig::IsTrgDown(CONTROL_TYPE cType, InputManager::JOYPAD_NO no ,TYPE type)
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
				if (InputManager::GetInstance().IsTrgDown(keyI))
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
				if (InputManager::GetInstance().IsMouseTrgDown(mouseI))
				{
					return true;
				}
			}
		}
	}
	if (type == TYPE::KEYBORD_MOUSE || type == TYPE::ALL)
	{
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
	}
	return false;
}

bool KeyConfig::IsTrgUp(CONTROL_TYPE cType, InputManager::JOYPAD_NO no, TYPE type)
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
				if (InputManager::GetInstance().IsTrgUp(keyI))
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
				if (InputManager::GetInstance().IsMouseTrgUp(mouseI))
				{
					return true;
				}
			}
		}
	}
	if (type == TYPE::KEYBORD_MOUSE || type == TYPE::ALL)
	{
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

void KeyConfig::Add(CONTROL_TYPE type,InputManager::JOYPAD_BTN key)
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

void KeyConfig::Add(CONTROL_TYPE type,InputManager::JOYPAD_STICK key)
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

void KeyConfig::Add(CONTROL_TYPE type, InputManager::MOUSE key)
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

	std::vector<InputManager::MOUSE> mouse;
	mouse.emplace_back(key);
	mouseInput_.emplace(type, mouse);
}

void KeyConfig::Destroy(void)
{
	keyInput_.clear();
	conInput_.clear();
	stickInput_.clear();
	mouseInput_.clear();
	delete instance_;
}

KeyConfig::KeyConfig(void)
{
}

KeyConfig::KeyConfig(const KeyConfig& manager)
{
}
