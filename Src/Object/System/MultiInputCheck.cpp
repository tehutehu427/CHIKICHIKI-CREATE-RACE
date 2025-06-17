#include "MultiInputCheck.h"
#include "../../Manager/System/InputManager.h"
#include "../../Manager/System/KeyConfig.h"
#include "../../Manager/System/KeyConfig.h"

MultiInputCheck::MultiInputCheck() : key_(KeyConfig::GetInstance())
{
	allInput_ = false;
}

MultiInputCheck::~MultiInputCheck()
{
}

void MultiInputCheck::Load()
{
}

void MultiInputCheck::Init()
{
}

void MultiInputCheck::Update()
{
	// ‚·‚×‚Ä‚Ì“ü—Í‚ªŠ®—¹‚µ‚Ä‚¢‚éê‡‚Í‰½‚à‚µ‚È‚¢
	if (allInput_) { return; }

#ifdef _DEBUG
	
	if (key_.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, InputManager::JOYPAD_NO::PAD1))
	{
		allInput_ = true;
	}

#endif 
}

void MultiInputCheck::Draw()
{
}

void MultiInputCheck::Reset()
{
	// ‚·‚×‚Ä‚Ì“ü—Í‚ªŠ®—¹‚µ‚Ä‚¢‚È‚¢ó‘Ô‚ÉƒŠƒZƒbƒg
	allInput_ = false;
}
