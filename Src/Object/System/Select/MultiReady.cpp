#include "MultiReady.h"
#include "../../../Manager/System/ResourceManager.h"
#include "../../../Manager/System/KeyConfig.h"
#include "../../../Manager/System/DateBank.h"
#include "../../../Manager/Game/PlayerManager.h"
#include "../../../Scene/SelectScene.h"
#include "../../../Utility/Utility.h"
#include "../MultiInputCheck.h"
#include "MultiCheckPlayer.h"


MultiReady::MultiReady() :
	 keyConfig_(KeyConfig::GetInstance())
{
	//状態別処理の登録
	RegisterProcessFunc(STATE::NUM_CHECK, SceneBase::ProcessFunction{ [&]() { UpdateNumCheck(); },  [&]() { DrawNumCheck(); } });
	RegisterProcessFunc(STATE::PAD_CHECK, SceneBase::ProcessFunction{ [&]() { UpdatePadCheck(); },  [&]() { DrawPadCheck(); } });
	RegisterProcessFunc(STATE::FINAL_CHECK, SceneBase::ProcessFunction{ [&]() { UpdateFinalCheck(); },  [&]() { DrawFinalCheck(); } });
	RegisterProcessFunc(STATE::PLAYER_ANIM, SceneBase::ProcessFunction{ [&]() { UpdatePlayerAnimation(); },  [&]() { DrawFinalCheck(); } });

	//初期化
	state_ = STATE::NUM_CHECK;
	playerNum_ = 0;
	multiInputChecks_ = nullptr;
	players_.clear();
}

MultiReady::~MultiReady()
{
}

void MultiReady::Load()
{
	ResourceManager& res = ResourceManager::GetInstance();
	imgMessages_ = res.Load(ResourceManager::SRC::SELECT_MESSAGES).handleIds_;
	imgNumbers_ = res.Load(ResourceManager::SRC::NUMBERS).handleIds_;
	imgSelectIcon_ = res.Load(ResourceManager::SRC::SCROLL_ARROW_ICON).handleId_;

	multiInputChecks_ = std::make_unique<MultiInputCheck>();
	multiInputChecks_->Load();

}

void MultiReady::Init()
{
	//初期化
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
			return;
		}
		else
		{
			//状態を戻す
			ChangeState(static_cast<STATE>(state));

			//パッド入力状況のリセット
			multiInputChecks_->ResetInput();

			//プレイヤーの状態やエフェクトを初期化
			for (auto& player : players_) { player->Reset(); }

			return;
		}
	}
}

void MultiReady::Draw()
{
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
		int playerNum = playerNum_ + PLAYER_NUM_MIN;

		//データ格納（実際の人数は MIN を加算）
		DateBank::GetInstance().SetPlayerNum(playerNum);

		//パッド確認のリセット
		multiInputChecks_->Reset();

		//状態遷移
		ChangeState(STATE::PAD_CHECK);

		//プレイヤーの中身削除
		players_.clear();

		//プレイヤー生成
		for (int i = 0; i < playerNum_ + PLAYER_NUM_MIN; i++)
		{
			auto player = std::make_unique<MultiCheckPlayer>();
			player->Create(i, playerNum);
			players_.push_back(std::move(player));
		}
	}
}

void MultiReady::UpdatePadCheck()
{
	//更新処理
	multiInputChecks_->Update();

	for (int i = 0; i < players_.size(); i++)
	{
		if (!multiInputChecks_->IsInput(i)) { continue; }
		players_[i]->Update();
	}

	//全員の入力確認
	if (multiInputChecks_->IsAllInput())
	{
		ChangeState(STATE::FINAL_CHECK);
	}
}

void MultiReady::UpdateFinalCheck()
{
	for (int i = 0; i < players_.size(); i++)
	{
		players_[i]->Update();
	}

	//最終確認
	if (keyConfig_.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER,KeyConfig::JOYPAD_NO::PAD1))
	{
		//アニメーション開始
		for (auto& player : players_)
		{
			player->SetGameStartAnimation();
		}

		//状態遷移
		ChangeState(STATE::PLAYER_ANIM);
	}
}

void MultiReady::UpdatePlayerAnimation()
{	
	//一人だけ確認
	if (players_[0]->IsFinishGameStartAnimation())
	{
		return;
	}

	//更新
	for (auto& player : players_)
	{
		player->Update();
	}

	if (players_[0]->IsFinishGameStartAnimation())
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::MULTI);
		return;
	}
}

void MultiReady::DrawNumCheck()
{
	//描画位置
	constexpr int POS_X = static_cast<int>(ResourceManager::SELECT_MES_SIZE_X * MESSAGE_RATE / 2 + 200);
	constexpr int OFFSET_POS_Y = 64;
	DrawMessage(POS_X, OFFSET_POS_Y, static_cast<int>(SelectScene::SELECT_MES::PLAYER_NUM));

	//選択用アイコンの描画
	for (int i = 0; i < ICON_NUM; i++)
	{
		int inversion = -1;
		float angle = 270.0f;
		if( i == 0)
		{
			inversion *= -1;
			angle = 90.0f;
		}

		DrawRotaGraph(
			Application::SCREEN_HALF_X + inversion* 300,
			Application::SCREEN_HALF_Y,
			1.0f,
			Utility::Deg2RadF(angle),
			imgSelectIcon_,
			true,
			false
		);
	}

	//人数の描画
	DrawRotaGraph(
		Application::SCREEN_HALF_X,
		Application::SCREEN_HALF_Y,
		NUMBER_RATE,
		0.0f,
		imgNumbers_[playerNum_ + PLAYER_NUM_MIN],
		true,
		false
	);
}

void MultiReady::DrawPadCheck()
{
	multiInputChecks_->Draw();
	//描画位置
	constexpr int POS_X = static_cast<int>(ResourceManager::SELECT_MES_SIZE_X * MESSAGE_RATE / 2 + 150);
	constexpr int POS_Y = 64;
	DrawMessage(POS_X, POS_Y, static_cast<int>(SelectScene::SELECT_MES::PUSH_BUTTON));

	for (int i = 0; i < players_.size(); i++)
	{
		if (!multiInputChecks_->IsInput(i)) { continue; }
		players_[i]->Draw();
	}
}

void MultiReady::DrawFinalCheck()
{
	multiInputChecks_->Draw();
	//描画位置
	constexpr int POS_X = static_cast<int>(ResourceManager::SELECT_MES_SIZE_X * MESSAGE_RATE / 2 + 170);
	constexpr int POS_Y = 64;
	DrawMessage(POS_X, POS_Y, static_cast<int>(SelectScene::SELECT_MES::GAME_START));

	for (int i = 0; i < players_.size(); i++)
	{
		players_[i]->Draw();
	}
}

void MultiReady::DrawMessage(const int _posX, const int _posY, const int _imgIndex_)
{	
	DrawRotaGraph(
		_posX,
		_posY,
		MESSAGE_RATE,
		0.0f,
		imgMessages_[_imgIndex_],
		true,
		false
	);
}