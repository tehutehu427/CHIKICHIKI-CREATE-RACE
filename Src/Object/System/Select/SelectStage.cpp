#include "SelectStage.h"
#include "../../../Manager/System/KeyConfig.h"
#include "../../../Manager/System/DateBank.h"
#include "../../../Manager/System/SceneManager.h"
#include "../../../Manager/System/ResourceManager.h"
#include "../../../Manager/System/SoundManager.h"
#include "../../../Utility/Utility.h"
#include "../../../Scene/SelectScene.h"

SelectStage::SelectStage(void)
{
	stageIndex_ = 0;
	selectIcon_ = -1;
	imgArrow_ = -1;
	imgLevels_ = nullptr;
	imgMessages_ = nullptr;
}

void SelectStage::Load(void)
{
	ResourceManager& res = ResourceManager::GetInstance();
	imgMessages_ = res.Load(ResourceManager::SRC::SELECT_MESSAGES).handleIds_;
	imgArrow_ = res.Load(ResourceManager::SRC::SCROLL_ARROW_ICON).handleId_;
	imgLevels_ = res.Load(ResourceManager::SRC::LEVEL_SELECTS).handleIds_;
}

void SelectStage::Update(SelectScene& _parent)
{
	KeyConfig& key = KeyConfig::GetInstance();
	SoundManager& sndMng = SoundManager::GetInstance();

	if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::CANCEL, KeyConfig::JOYPAD_NO::PAD1))
	{
		//モードセレクトへ戻る
		sndMng.Play(SoundManager::SRC::CANCEL, SoundManager::PLAYTYPE::BACK);
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
		
		sndMng.Play(SoundManager::SRC::SELECT_SCENE_CHANGE, SoundManager::PLAYTYPE::BACK);
		sndMng.Stop(SoundManager::SRC::SELECT_BGM);
		return;
	}
	else if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_LEFT, KeyConfig::JOYPAD_NO::PAD1))
	{
		//左キーで選択をひとつ戻す（範囲内でループ）
		stageIndex_ = (stageIndex_ - 1 + STAGE_TYPE_MAX) % STAGE_TYPE_MAX;
		sndMng.Play(SoundManager::SRC::CLICK_OBJECT_SE, SoundManager::PLAYTYPE::BACK);
		return;
	}
	else if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_RIGHT, KeyConfig::JOYPAD_NO::PAD1))
	{
		//右キーで選択をひとつ進める（範囲内でループ）
		stageIndex_ = (stageIndex_ + 1) % STAGE_TYPE_MAX;
		sndMng.Play(SoundManager::SRC::CLICK_OBJECT_SE, SoundManager::PLAYTYPE::BACK);
		return;
	}
}

void SelectStage::Draw(void)
{
	//矢印の位置を調整
	constexpr int OFFSET_POS_X = 350;
	constexpr float LEVEL_RATE = 1.5f;
	constexpr int LEVEL_POS_Y = 350;
	constexpr float RIGHT_ANGLE = 90.0f;
	constexpr float LEFT_ANGLE = 270.0f;

	//メッセージの描画
	DrawMessage();

	for (int i = 0; i < ARROW_NUM; i++)
	{
		//反転用
		int rev = 1;
		float angle = RIGHT_ANGLE;
		if (i == 0)
		{ 
			rev *= -1;
			angle = LEFT_ANGLE;
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
		LEVEL_POS_Y,
		LEVEL_RATE,
		0.0f,
		imgLevels_[stageIndex_],
		true
	);


}

void SelectStage::DrawMessage(void)
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
