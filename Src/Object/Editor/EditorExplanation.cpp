#include "EditorExplanation.h"
#include <DxLib.h>
#include "../../Manager/System/ResourceManager.h"

EditorExplanation::EditorExplanation(void)
{
	imgKeyExplanation_ = -1;
	imgPadExplanation_ = -1;
}

void EditorExplanation::Load(void)
{
	auto& res = ResourceManager::GetInstance();
	imgPadExplanation_ = res.Load(ResourceManager::SRC::PAD_EXPLANATION).handleId_;
	imgKeyExplanation_ = res.Load(ResourceManager::SRC::KEY_EXPLANATION).handleId_;
}

void EditorExplanation::Draw(void)
{
	//PAD‚Ì”‚É‡‚í‚¹‚Ä‰æ‘œ‚ğŒˆ‚ß‚é
	int img = GetJoypadNum() == 0 ? imgKeyExplanation_ : imgPadExplanation_;

	//•`‰æ
	DrawRotaGraph(
		POS_X,
		POS_Y,
		RATE,
		0.0f,
		img,
		true,
		false
	);
}
