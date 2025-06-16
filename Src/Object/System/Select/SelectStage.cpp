#include "SelectStage.h"
#include "../../../Manager/System/InputManager.h"
#include "../../../Manager/System/DateBank.h"
#include "../../../Scene/SelectScene.h"

SelectStage::SelectStage()
{
	stageIndex_ = 0;
}

SelectStage::~SelectStage()
{
}

void SelectStage::Load()
{
}

void SelectStage::Init()
{
}

void SelectStage::Update(SelectScene& _parent)
{
	InputManager& input = InputManager::GetInstance();

	if (input.IsTrgDown(KEY_INPUT_B))
	{
		//モードセレクトへ戻る
		_parent.ChangeState(SelectScene::STATE::SELECT_MENU);
	}
	else if (input.IsTrgDown(KEY_INPUT_RETURN))
	{
		//ステージ番号を格納
		DateBank::GetInstance().SetStageNo(stageIndex_);
	}
	else if (input.IsTrgDown(KEY_INPUT_LEFT) && stageIndex_ % LINE != 0)
	{
		stageIndex_--;
	}
	else if (input.IsTrgDown(KEY_INPUT_RIGHT) && stageIndex_ % LINE != LINE - 1)
	{
		stageIndex_++;
	}
	else if (input.IsTrgDown(KEY_INPUT_UP) && stageIndex_ >= LINE)
	{
		stageIndex_ -= LINE;
	}
	else if (input.IsTrgDown(KEY_INPUT_DOWN) && stageIndex_ + LINE < LINE * COL)
	{
		stageIndex_ += LINE;
	}
}

void SelectStage::Draw()
{
	int size = 50;
	int index = 0;
	for (int y = 0; y < COL; y++)
	{
		for (int x = 0; x < LINE; x++)
		{
			int color = 0xff00ff;
			if (stageIndex_ == index) { color = 0xffff00; }
			DrawBox(
				100 + x * size,
				300 + y * size,
				100 + (x + 1) * size,
				300 + (y + 1) * size,
				color,
				true);
			index++;
		}
	}
}
