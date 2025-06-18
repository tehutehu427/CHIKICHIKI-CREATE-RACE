#include "MultiReady.h"
#include "../../../Manager/System/ResourceManager.h"
#include "../../../Manager/System/KeyConfig.h"
#include "../../../Manager/System/DateBank.h"
#include "../../../Manager/Game/PlayerManager.h"
#include "../../../Scene/SelectScene.h"
#include "../../../Utility/Utility.h"
#include "../MultiInputCheck.h"


MultiReady::MultiReady() :
	 keyConfig_(KeyConfig::GetInstance())
{
	//状態別処理の登録
	RegisterProcessFunc(STATE::NUM_CHECK, SceneBase::ProcessFunction{ [&]() { UpdateNumCheck(); },  [&]() { DrawNumCheck(); } });
	RegisterProcessFunc(STATE::PAD_CHECK, SceneBase::ProcessFunction{ [&]() { UpdatePadCheck(); },  [&]() { DrawPadCheck(); } });
	RegisterProcessFunc(STATE::FINAL_CHECK, SceneBase::ProcessFunction{ [&]() { UpdateFinalCheck(); },  [&]() { DrawFinalCheck(); } });

	//初期化
	state_ = STATE::NUM_CHECK;
	playerNum_ = 0;
	multiInputChecks_ = nullptr;
}

MultiReady::~MultiReady()
{
}

void MultiReady::Load()
{
	multiInputChecks_ = std::make_unique<MultiInputCheck>();
	multiInputChecks_->Load();
}

void MultiReady::Init()
{
	multiInputChecks_->Init();
}

void MultiReady::Update(SelectScene& _parent)
{
	stateTables_[state_].updataFunc_();

	//戻る処理
	if (keyConfig_.IsTrgDown(KeyConfig::CONTROL_TYPE::CANCEL, KeyConfig::JOYPAD_NO::PAD1))
	{
		//ひとつ前の状態を取得
		int state = static_cast<int>(state_) - 1;
		if (state < 0)
		{
			//モードのセレクトに戻る
			_parent.ChangeState(SelectScene::STATE::SELECT_MENU);
		}
		else
		{
			//状態を戻る
			ChangeState(static_cast<STATE>(state));
		}
	}
}

void MultiReady::Draw()
{
	DrawBox(
		0, 0,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		Utility::LIME,
		true
	);

	stateTables_[state_].drawFunc_();
}

void MultiReady::RegisterProcessFunc(const STATE _state, SceneBase::ProcessFunction _funcs)
{
	stateTables_[_state] = _funcs;
}

void MultiReady::UpdateNumCheck()
{
	static constexpr int PLAYER_NUM_MAX = PlayerManager::PLAYER_NUM_MAX;//選べるプレイヤー人数の最大
	static constexpr int PLAYER_NUM_CHOICES = PLAYER_NUM_MAX - PLAYER_NUM_MIN + 1;//定数：選べるプレイヤー人数の数

	//人数の選択
	if (keyConfig_.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_LEFT, KeyConfig::JOYPAD_NO::PAD1))
	{
		//左キーで選択をひとつ戻す（範囲内でループ）
		playerNum_ = (playerNum_ - 1 + PLAYER_NUM_CHOICES) % PLAYER_NUM_CHOICES;
	}
	else if (keyConfig_.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_RIGHT, KeyConfig::JOYPAD_NO::PAD1))
	{
		//右キーで選択をひとつ進める（範囲内でループ）
		playerNum_ = (playerNum_ + 1) % PLAYER_NUM_CHOICES;
	}
	else if (keyConfig_.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
	{
		//データ格納（実際の人数は MIN を加算）
		DateBank::GetInstance().SetPlayerNum(playerNum_ + PLAYER_NUM_MIN);

		//状態遷移
		ChangeState(STATE::PAD_CHECK);
	}
}

void MultiReady::UpdatePadCheck()
{
	//更新処理
	multiInputChecks_->Update();

	//全員の入力確認
	if (multiInputChecks_->IsAllInput())
	{
		ChangeState(STATE::FINAL_CHECK);
	}
}

void MultiReady::UpdateFinalCheck()
{
	//最終確認
	if (keyConfig_.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER,KeyConfig::JOYPAD_NO::PAD1))
	{
		//シーン遷移
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::MULTI);
	}
}

void MultiReady::DrawNumCheck()
{
	DrawFormatString(300, 300, Utility::WHITE, "プレイヤー人数 : %d", playerNum_ + PLAYER_NUM_MIN);
}

void MultiReady::DrawPadCheck()
{
	multiInputChecks_->Draw();
}

void MultiReady::DrawFinalCheck()
{
	DrawFormatString(300, 300, Utility::RED, "Are You OK ?");
}
