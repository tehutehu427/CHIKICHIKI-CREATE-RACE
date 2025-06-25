#include "MultiInputCheck.h"
#include "../../Application.h"
#include "../../Manager/System/KeyConfig.h"
#include "../../Manager/System/DateBank.h"
#include "../../Manager/System/ResourceManager.h"
#include "../../Utility/Utility.h"

MultiInputCheck::MultiInputCheck() : key_(KeyConfig::GetInstance())
{
	players_.clear();
}

MultiInputCheck::~MultiInputCheck()
{
}

void MultiInputCheck::Load()
{
	//リソースの読み込み
	ResourceManager& res = ResourceManager::GetInstance();
}

void MultiInputCheck::Init()
{
	Reset();
}

void MultiInputCheck::Update()
{
	// すべての入力が完了している場合は何もしない
	if (IsAllInput()) { return; }

	//全員の入力を確認する
	constexpr int OFFSET = static_cast<int>(KeyConfig::JOYPAD_NO::PAD1);
	for (int i = 0; i < players_.size(); i++)
	{
		if (key_.IsTrgDown(KeyConfig::CONTROL_TYPE::DECISION_KEY_AND_PAD, static_cast<KeyConfig::JOYPAD_NO>(OFFSET + i)))
		{
			players_[i].isInput = true;
		}
	}
}

void MultiInputCheck::Draw()
{
	int index = 0;
	int color = 0;
	for (auto& player : players_)
	{
		if (!player.isInput) { break; }
		switch (index)
		{
		case 0:
			color = Utility::BLUE;
			break;
		case 1:
			color = Utility::RED;
			break;
		case 2:
			color = Utility::YELLOW;
			break;
		case 3:
			color = Utility::GREEN;
			break;
		}
		DrawFormatString(player.uiPos.x, player.uiPos.y, color, "OK");
		index++;
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
	}
}
