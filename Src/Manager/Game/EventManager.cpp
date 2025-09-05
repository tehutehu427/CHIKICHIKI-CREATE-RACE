#include "EventManager.h"
#include "PlayerManager.h"
#include "PostEffectManager.h"
#include "../System/ResourceManager.h"

EventManager* EventManager::instance_ = nullptr;

void EventManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new EventManager();
	}
	instance_->Load();
}

EventManager& EventManager::GetInstance(void)
{
	return *instance_;
}

void EventManager::Destroy(void)
{
	//自身のインスタンス削除
	delete instance_;
	instance_ = nullptr;
}

void EventManager::Load(void)
{
	auto& res = ResourceManager::GetInstance();

	constexpr ResourceManager::SRC RESOURCES[] =
	{
		ResourceManager::SRC::NONE,
		ResourceManager::SRC::EVENT_STATE_UP,
		ResourceManager::SRC::EVENT_RESPAWN,
		ResourceManager::SRC::EVENT_WIGGLE,
		ResourceManager::SRC::EVENT_FISH_EYE,
		ResourceManager::SRC::EVENT_SAND_STORM
	};

	for (int i = 0; i < static_cast<int>(EVENT_TYPE::MAX); i++)
	{
		imgEvents_[i] = res.Load(RESOURCES[i]).handleId_;
	}
}

void EventManager::Init()
{
	SetEventType(EVENT_TYPE::NONE);
	isStateUp_ = false;
}

void EventManager::Update()
{
	eventUpdateMap_[eventType_]();
}

void EventManager::Draw()
{
	if (eventType_ == EVENT_TYPE::NONE)
	{
		return;
	}

	//イベントタイトルの画像を描画
	DrawGraph(0, 0, imgEvents_[static_cast<int>(eventType_)], true);
}

void EventManager::RegisterSet(const EVENT_TYPE _type, std::function<void(void)> _setter, std::function<void(void)> _update)
{
	eventSetterMap_[_type] = _setter;
	eventUpdateMap_[_type] = _update;
}

void EventManager::SetEventType(const EVENT_TYPE _type)
{
	eventType_ = _type;
	eventSetterMap_[eventType_]();
}

void EventManager::SetRandomEvent()
{
	//ランダムで値取得
	int index = GetRand(EVENT_PER);

	//特定値以外はイベントを実行しない
	if (index != 0)
	{
		return;
	}

	//ランダムで値取得
	index = 1 + GetRand(static_cast<int>(EVENT_TYPE::MAX) - 2);

	//int index = 2;

	//設定
	SetEventType(static_cast<EVENT_TYPE>(index));
}

void EventManager::Reset()
{
	if (eventType_ == EVENT_TYPE::NONE)
	{
		return;
	}

	//プレイヤーのステータスを戻す
	auto& players = PlayerManager::GetInstance().GetPlayers();

	//プレイヤー戻す
	for (auto& player : players)
	{
		player->SetRespawnCnt(0);
	}

	//教科用判定を戻す
	isStateUp_ = false;

	//ポストエフェクトを戻す
	PostEffectManager::GetInstance().ChangeEffectType(PostEffectManager::TYPE::NONE);

	//イベントを戻す
	SetEventType(EVENT_TYPE::NONE);
}

void EventManager::SetSandstorm(void)
{
	PostEffectManager::GetInstance().ChangeEffectType(PostEffectManager::TYPE::SANDSTORM);
}

void EventManager::SetWiggle(void)
{
	PostEffectManager::GetInstance().ChangeEffectType(PostEffectManager::TYPE::WIGGLE);
}

void EventManager::SetFishEye(void)
{
	PostEffectManager::GetInstance().ChangeEffectType(PostEffectManager::TYPE::FISH_EYE);
}

void EventManager::UpdateStateUp(void)
{
	//まだ強化してないとき
	if (!isStateUp_)
	{
		auto& players = PlayerManager::GetInstance().GetPlayers();

		//プレイヤー全て強化させる
		for (auto& player : players)
		{
			player->SetPunchPow(Player::BUFF_KNOCKBACK_CNT_MAX, Player::BUFF_KNOCKBACK_SPEED_MAX);
			player->SetJumpDecelMax(Player::BUFF_JUMP_POW_MAX);
			player->SetSpeed(Player::BUFF_MOVE_SPEED, Player::BUFF_DASH_SPEED);
		}

		//強化反映完了
		isStateUp_ = true;
	}
}

void EventManager::UpdateSandstorm(void)
{
	auto& players = PlayerManager::GetInstance().GetPlayers();
	VECTOR MOVE_POW = { 1.0f, 0.0f, 0.0f };

	//プレイヤー全て強化させる
	for (auto& player : players)
	{
		//ゴールもしくは死んでる場合はパス
		if (player->GetIsDeathMoment() || player->GetIsGoalMoment())
		{
			break;
		}

		//現在位置取得
		VECTOR pos = player->GetTransform().pos;
		
		//値追加
		pos = VAdd(pos, MOVE_POW);
		
		//座標設定
		player->SetPos(pos);
	}
}

void EventManager::UpdateRespawn(void)
{
	//まだ設定してないとき
	if (!isStateUp_)
	{
		auto& players = PlayerManager::GetInstance().GetPlayers();

		//プレイヤー全て強化させる
		for (auto& player : players)
		{
			player->SetRespawnCnt(Player::RESPAWN_CNT);
		}

		//強化反映完了
		isStateUp_ = true;
	}
}

EventManager::EventManager(void)
{
	eventType_ = EVENT_TYPE::NONE;

	//処理の登録
	RegisterSet(EVENT_TYPE::NONE, [&]() { SetNone(); }, [&]() { UpdateNone(); });
	RegisterSet(EVENT_TYPE::RESPAWN, [&]() { SetNone(); }, [&]() { UpdateRespawn(); });
	RegisterSet(EVENT_TYPE::WIGGLE, [&]() { SetWiggle(); }, [&]() { UpdateNone(); });
	RegisterSet(EVENT_TYPE::STATE_UP, [&]() { SetNone(); }, [&]() { UpdateStateUp(); });
	RegisterSet(EVENT_TYPE::FISH_EYE, [&]() { SetFishEye(); }, [&]() { UpdateNone(); });
	RegisterSet(EVENT_TYPE::SAND_STORM, [&]() { SetSandstorm(); }, [&]() { UpdateSandstorm(); });
}

EventManager::~EventManager(void)
{
}
