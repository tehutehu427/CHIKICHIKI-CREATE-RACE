#include "MultiInputCheck.h"
#include "../../Application.h"
#include "../../Manager/System/KeyConfig.h"
#include "../../Manager/System/DateBank.h"
#include "../../Manager/System/ResourceManager.h"
#include "../../Manager/System/SoundManager.h"
#include "../../Manager/System/SceneManager.h"
#include "../../Utility/Utility.h"

MultiInputCheck::MultiInputCheck() : key_(KeyConfig::GetInstance())
{
	players_.clear();
	imgOk_ = -1;
}

MultiInputCheck::~MultiInputCheck()
{
}

void MultiInputCheck::Load()
{
	//リソースの読み込み
	ResourceManager& res = ResourceManager::GetInstance();
	SoundManager& sndMng = SoundManager::GetInstance();
	imgOk_ = res.Load(ResourceManager::SRC::OK).handleId_;
	sndMng.LoadResource(SoundManager::SRC::OK);
	sndMng.LoadResource(SoundManager::SRC::CHICKEN_SE_3);
}

void MultiInputCheck::Init()
{
	Reset();
}

void MultiInputCheck::Update()
{
	SoundManager& sndMng = SoundManager::GetInstance();

	// すべての入力が完了している場合は何もしない
	if (IsAllInput()) { return; }

	//全員の入力を確認する
	constexpr int OFFSET = static_cast<int>(KeyConfig::JOYPAD_NO::PAD1);
	constexpr float TIME = 1.0f;

	for (int i = 0; i < players_.size(); i++)
	{
		if (key_.IsTrgDown(KeyConfig::CONTROL_TYPE::DECISION_KEY_AND_PAD, static_cast<KeyConfig::JOYPAD_NO>(OFFSET + i)) && !players_[i].isProcess)
		{
			players_[i].isProcess = true;
			sndMng.Play(SoundManager::SRC::OK, SoundManager::PLAYTYPE::BACK);
			sndMng.Play(SoundManager::SRC::CHICKEN_SE_3, SoundManager::PLAYTYPE::BACK);
		}
		//画像の拡大イージング処理
		if (players_[i].isProcess)
		{	
			//時間更新
			players_[i].step+= SceneManager::GetInstance().GetDeltaTime();

			//拡大率の更新
			players_[i].rate = Utility::EaseInOutBack(players_[i].step, TIME, 0.0f, RATE);

			//拡大率が一定以上になったら
			if (players_[i].step >= TIME)
			{
				players_[i].isInput = true;
			}
		}
	}
}

void MultiInputCheck::Draw()
{
	for (auto& player : players_)
	{
		if (!player.isProcess) { continue; }

		DrawRotaGraph(
			player.uiPos.x, 
			player.uiPos.y, 
			player.rate,
			0.0f,
			imgOk_,
			true,
			false);
	}
}

void MultiInputCheck::Reset()
{
	//人数を取得
	const int playerNum = DateBank::GetInstance().GetPlayerNum();

	//中身がある場合削除する
	if (!players_.empty()) { players_.clear(); }

	//リサイズ
	PlayerCheckInput p;
	players_.resize(playerNum, p);

	//座標設定
	for (int i = 0; i < playerNum; i++)
	{
		players_[i].uiPos.x = Application::SCREEN_SIZE_X / (playerNum + 1) * (i + 1);
		players_[i].uiPos.y = Application::SCREEN_SIZE_Y - 100;
	}
}

bool MultiInputCheck::IsAllInput()
{
	//空の場合はfalse
	if (players_.empty()) { return false; }

	//全員の入力状況を調べる
	for (int i = 0; i < players_.size(); i++)
	{
		if (!players_[i].isInput) { return false; }
	}
	//問題なければtrueを返す
	return true;
}

void MultiInputCheck::ResetInput()
{
	for (auto& player : players_)
	{
		player.isInput = false;
		player.isProcess = false;
		player.rate = 0.0f;
		player.step = 0.0f;
	}
}
