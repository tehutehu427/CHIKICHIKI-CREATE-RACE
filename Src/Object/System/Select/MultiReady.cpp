#include "MultiReady.h"
#include "../../../Manager/System/ResourceManager.h"
#include "../../../Manager/System/InputManager.h"
#include "../../../Manager/System/DateBank.h"
#include "../../../Manager/Game/PlayerManager.h"
#include "../../../Scene/SelectScene.h"
#include "../../../Utility/Utility.h"
#include "../MultiInputCheck.h"


MultiReady::MultiReady() :
	 input_(InputManager::GetInstance())
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
	if (input_.IsTrgDown(KEY_INPUT_B))
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
	//最大人数
	static const int playerMax = PlayerManager::PLAYER_NUM;

	//人数の選択
	if (input_.IsTrgDown(KEY_INPUT_LEFT))
	{
		playerNum_ = (playerNum_ - 1 + playerMax) % playerMax;
	}
	else if (input_.IsTrgDown(KEY_INPUT_RIGHT))
	{
		playerNum_ = (playerNum_ + 1) % playerMax;
	}
	else if (input_.IsTrgDown(KEY_INPUT_RETURN))
	{
		//データ格納
		DateBank::GetInstance().SetPlayerNum(playerNum_ + 1);

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
	if (input_.IsTrgDown(KEY_INPUT_RETURN))
	{
		//シーン遷移
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::MULTI);
	}
}

void MultiReady::DrawNumCheck()
{
	DrawFormatString(300, 300, Utility::WHITE, "プレイヤー人数 : %d", playerNum_ + 1);
}

void MultiReady::DrawPadCheck()
{
	multiInputChecks_->Draw();
}

void MultiReady::DrawFinalCheck()
{
	DrawFormatString(300, 300, Utility::RED, "Are You OK ?");
}
