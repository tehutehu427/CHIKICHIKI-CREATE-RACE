#include "SelectStage.h"
#include "../../../Manager/System/KeyConfig.h"
#include "../../../Manager/System/DateBank.h"
#include "../../../Manager/System/SceneManager.h"
#include "../../../Manager/System/ResourceManager.h"
#include "../../../Utility/Utility.h"
#include "../../../Scene/SelectScene.h"

SelectStage::SelectStage()
{
	stageIndex_ = 0;
	selectIcon_ = -1;
	imgArrow_ = -1;
	int i = -1;
	imgMessages_ = &i;
}

SelectStage::~SelectStage()
{
}

void SelectStage::Load()
{
	ResourceManager& res = ResourceManager::GetInstance();
	imgMessages_ = res.Load(ResourceManager::SRC::SELECT_MESSAGES).handleIds_;
	imgArrow_ = res.Load(ResourceManager::SRC::SCROLL_ARROW_ICON).handleId_;
	imgLevels_ = res.Load(ResourceManager::SRC::LEVEL_SELECTS).handleIds_;
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
		return;
	}
	else if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
	{
		//ステージ番号を格納
		DateBank::GetInstance().SetStageNo(stageIndex_);

		//人数を設定
		DateBank::GetInstance().SetPlayerNum(1);

		//ソロチャレンジにシーン遷移
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::SOLO);
		return;
	}
	else if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_LEFT, KeyConfig::JOYPAD_NO::PAD1))
	{
		//左キーで選択をひとつ戻す（範囲内でループ）
		stageIndex_ = (stageIndex_ - 1 + STAGE_TYPE_MAX) % STAGE_TYPE_MAX;
		return;
	}
	else if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_RIGHT, KeyConfig::JOYPAD_NO::PAD1))
	{
		//右キーで選択をひとつ進める（範囲内でループ）
		stageIndex_ = (stageIndex_ + 1) % STAGE_TYPE_MAX;
		return;
	}
}

void SelectStage::Draw()
{
	//矢印の位置を調整
	constexpr int OFFSET_POS_X = 350;

	//メッセージの描画
	DrawMessage();

	for (int i = 0; i < ARROW_NUM; i++)
	{
		//反転用
		int rev = 1;
		float angle = 90.0f;
		if (i == 0)
		{ 
			rev *= -1;
			angle += 180.0f;
		}

		DrawRotaGraph(
			Application::SCREEN_HALF_X + OFFSET_POS_X * rev,
			Application::SCREEN_HALF_Y,
			1.0f,
			Utility::Deg2RadF(angle),
			imgArrow_,
			true
		);
	}

	//レベル
	DrawRotaGraph(
		Application::SCREEN_HALF_X,
		Application::SCREEN_HALF_Y,
		1.5f,
		0.0f,
		imgLevels_[stageIndex_],
		true
	);


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
