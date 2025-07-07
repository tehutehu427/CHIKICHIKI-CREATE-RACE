#include "SelectStage.h"
#include "../../../Manager/System/KeyConfig.h"
#include "../../../Manager/System/DateBank.h"
#include "../../../Manager/System/SceneManager.h"
#include "../../../Manager/System/ResourceManager.h"
#include "../../../Scene/SelectScene.h"

SelectStage::SelectStage()
{
	stageIndex_ = 0;
	selectIcon_ = -1;
	
}

SelectStage::~SelectStage()
{
}

void SelectStage::Load()
{
	ResourceManager& res = ResourceManager::GetInstance();
	imgMessages_ = res.Load(ResourceManager::SRC::SELECT_MESSAGES).handleIds_;

}

void SelectStage::Init()
{
}

void SelectStage::Update(SelectScene& _parent)
{
	KeyConfig& key = KeyConfig::GetInstance();

	if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::CANCEL, KeyConfig::JOYPAD_NO::PAD1))
	{
		//モードセレクトへ戻る
		_parent.ChangeState(SelectScene::STATE::SELECT_MENU);
	}
	else if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
	{
		//ステージ番号を格納
		DateBank::GetInstance().SetStageNo(stageIndex_);

		//人数を設定
		DateBank::GetInstance().SetPlayerNum(1);

		//ソロチャレンジにシーン遷移
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::SOLO);
	}
	else if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_LEFT, KeyConfig::JOYPAD_NO::PAD1)
		&& stageIndex_ % LINE != 0)
	{
		stageIndex_--;
	}
	else if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_RIGHT, KeyConfig::JOYPAD_NO::PAD1)
		&& stageIndex_ % LINE != LINE - 1)
	{
		stageIndex_++;
	}
	else if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_UP, KeyConfig::JOYPAD_NO::PAD1)
		&& stageIndex_ >= LINE)
	{
		stageIndex_ -= LINE;
	}
	else if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_DOWN, KeyConfig::JOYPAD_NO::PAD1) && stageIndex_ + LINE < LINE * COL)
	{
		stageIndex_ += LINE;
	}
}

void SelectStage::Draw()
{
	//メッセージの描画
	DrawMessage();

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

void SelectStage::DrawMessage()
{
	constexpr float RATE = 0.7f;
	constexpr int POS_X = static_cast<int>(ResourceManager::SELECT_MES_SIZE_X * RATE / 2 + Application::SCREEN_HALF_X - 280);
	constexpr int POS_Y = 64;

	DrawRotaGraph(
		POS_X,
		POS_Y,
		RATE,
		0.0f,
		imgMessages_[static_cast<int>(SelectScene::SELECT_MES::STAGE_SELECT)],
		true,
		false
	);
}
