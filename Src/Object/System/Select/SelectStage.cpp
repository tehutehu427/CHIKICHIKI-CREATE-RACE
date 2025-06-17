#include "SelectStage.h"
#include "../../../Manager/System/InputManager.h"
#include "../../../Manager/System/KeyConfig.h"
#include "../../../Manager/System/DateBank.h"
#include "../../../Manager/System/SceneManager.h"
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
	KeyConfig& key = KeyConfig::GetInstance();

	if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_BACK, InputManager::JOYPAD_NO::PAD1))
	{
		//モードセレクトへ戻る
		_parent.ChangeState(SelectScene::STATE::SELECT_MENU);
	}
	else if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, InputManager::JOYPAD_NO::PAD1))
	{
		//ステージ番号を格納
		DateBank::GetInstance().SetStageNo(stageIndex_);

		//ソロチャレンジにシーン遷移
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::SOLO);
	}
	else if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_LEFT, InputManager::JOYPAD_NO::PAD1)
		&& stageIndex_ % LINE != 0)
	{
		stageIndex_--;
	}
	else if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_RIGHT, InputManager::JOYPAD_NO::PAD1)
		&& stageIndex_ % LINE != LINE - 1)
	{
		stageIndex_++;
	}
	else if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_UP, InputManager::JOYPAD_NO::PAD1)
		&& stageIndex_ >= LINE)
	{
		stageIndex_ -= LINE;
	}
	else if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_DOWN, InputManager::JOYPAD_NO::PAD1) && stageIndex_ + LINE < LINE * COL)
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
