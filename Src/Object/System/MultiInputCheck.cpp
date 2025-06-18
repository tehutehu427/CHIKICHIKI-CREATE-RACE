#include "MultiInputCheck.h"
#include "../../Manager/System/KeyConfig.h"

MultiInputCheck::MultiInputCheck() : input_(KeyConfig::GetInstance())
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
	
	if (input_.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER,KeyConfig::JOYPAD_NO::PAD1))
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
