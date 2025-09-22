#include "MultiReady.h"
#include "../../../Manager/System/ResourceManager.h"
#include "../../../Manager/System/KeyConfig.h"
#include "../../../Manager/System/DateBank.h"
#include "../../../Manager/System/SoundManager.h"
#include "../../../Manager/Game/PlayerManager.h"
#include "../../../Scene/SelectScene.h"
#include "../../../Utility/Utility.h"
#include "../MultiInputCheck.h"
#include "MultiCheckPlayer.h"


MultiReady::MultiReady(void) :
	 keyConfig_(KeyConfig::GetInstance()),
	sndMng_(SoundManager::GetInstance())
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
	alphaDir_ = -11; //アルファ値の変化方向
	mesAlpha_ = -1; //メッセージのアルファ値

}

MultiReady::~MultiReady(void)
{
}

void MultiReady::Load(void)
{
	ResourceManager& res = ResourceManager::GetInstance();
	imgMessages_ = res.Load(ResourceManager::SRC::SELECT_MESSAGES).handleIds_;
	imgNumbers_ = res.Load(ResourceManager::SRC::NUMBERS).handleIds_;
	imgSelectIcon_ = res.Load(ResourceManager::SRC::SCROLL_ARROW_ICON).handleId_;
	imgPushButton_ = res.Load(ResourceManager::SRC::PUSH_B_BUTTON_MES).handleId_;

	multiInputChecks_ = std::make_unique<MultiInputCheck>();
	multiInputChecks_->Load();

}

void MultiReady::Init(void)
{
	//初期化
	multiInputChecks_->Init();
	alphaDir_ = 1; //アルファ値の変化方向
	mesAlpha_ = Utility::ALPHA_MAX; //メッセージのアルファ値
}

void MultiReady::Update(SelectScene& _parent)
{
	stateTables_[state_].updataFunc_();

	//戻る処理
	if (keyConfig_.IsTrgDown(KeyConfig::CONTROL_TYPE::CANCEL, KeyConfig::JOYPAD_NO::PAD1))
	{
		sndMng_.Play(SoundManager::SRC::CANCEL, SoundManager::PLAYTYPE::BACK);

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

void MultiReady::Draw(void)
{
	stateTables_[state_].drawFunc_();
}

void MultiReady::RegisterProcessFunc(const STATE _state, SceneBase::ProcessFunction _funcs)
{
	stateTables_[_state] = _funcs;
}

void MultiReady::UpdateNumCheck(void)
{
	static constexpr int PLAYER_NUM_MAX = PlayerManager::PLAYER_NUM_MAX;//選べるプレイヤー人数の最大
	static constexpr int PLAYER_NUM_CHOICES = PLAYER_NUM_MAX - PLAYER_NUM_MIN + 1;//定数：選べるプレイヤー人数の数

	//人数の選択
	if (keyConfig_.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_LEFT, KeyConfig::JOYPAD_NO::PAD1))
	{
		//左キーで選択をひとつ戻す（範囲内でループ）
		playerNum_ = (playerNum_ - 1 + PLAYER_NUM_CHOICES) % PLAYER_NUM_CHOICES;
		sndMng_.Play(SoundManager::SRC::CLICK_OBJECT_SE, SoundManager::PLAYTYPE::BACK);
	}
	else if (keyConfig_.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_RIGHT, KeyConfig::JOYPAD_NO::PAD1))
	{
		//右キーで選択をひとつ進める（範囲内でループ）
		playerNum_ = (playerNum_ + 1) % PLAYER_NUM_CHOICES;
		sndMng_.Play(SoundManager::SRC::CLICK_OBJECT_SE, SoundManager::PLAYTYPE::BACK);
	}
	else if (keyConfig_.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
	{
		int playerNum = playerNum_ + PLAYER_NUM_MIN;

		sndMng_.Play(SoundManager::SRC::DECISION, SoundManager::PLAYTYPE::BACK);

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

void MultiReady::UpdatePadCheck(void)
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

void MultiReady::UpdateFinalCheck(void)
{
	for (int i = 0; i < players_.size(); i++)
	{
		players_[i]->Update();
	}

	//最終確認
	if (keyConfig_.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER,KeyConfig::JOYPAD_NO::PAD1))
	{
		sndMng_.Play(SoundManager::SRC::SELECT_SCENE_CHANGE, SoundManager::PLAYTYPE::BACK);
		sndMng_.Stop(SoundManager::SRC::SELECT_BGM); 

		//アニメーション開始
		for (auto& player : players_)
		{
			player->SetGameStartAnimation();
		}

		//状態遷移
		ChangeState(STATE::PLAYER_ANIM);
	}
}

void MultiReady::UpdatePlayerAnimation(void)
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

void MultiReady::DrawNumCheck(void)
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

void MultiReady::DrawPadCheck(void)
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

void MultiReady::DrawFinalCheck(void)
{
	multiInputChecks_->Draw();
	//描画位置
	constexpr int POS_X = static_cast<int>(ResourceManager::SELECT_MES_SIZE_X * MESSAGE_RATE / 2 + 170);
	constexpr int POS_Y = 64;
	constexpr float ALPHA_STEP = 1.5f; //アルファ値の変化量
	constexpr float ALPHA_MIN = 50.0f; //アルファ値の最小量
	constexpr int OFFSET_POS_Y = 150;
	DrawMessage(POS_X, POS_Y, static_cast<int>(SelectScene::SELECT_MES::GAME_START));

	for (int i = 0; i < players_.size(); i++)
	{
		players_[i]->Draw();
	}

	//アルファ値を変え
	mesAlpha_ = static_cast<int>(Utility::PingPongUpdate(static_cast<float>(mesAlpha_), ALPHA_STEP, static_cast<float>(Utility::ALPHA_MAX), ALPHA_MIN, alphaDir_));

	//ボタンを押してね画像の描画
	if (state_ != STATE::FINAL_CHECK) { return; }
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, mesAlpha_);
	DrawRotaGraph(
		Application::SCREEN_HALF_X,
		Application::SCREEN_HALF_Y + OFFSET_POS_Y,
		1.0f,
		0.0f,
		imgPushButton_,
		true,
		false
	);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
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