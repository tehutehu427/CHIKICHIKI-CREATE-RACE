#include "MultiInputCheck.h"
#include "../../Manager/System/InputManager.h"

MultiInputCheck::MultiInputCheck() : input_(InputManager::GetInstance())
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
	
	if (input_.IsTrgDown(KEY_INPUT_RETURN))
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
