#include "PlayerManager.h"
std::unique_ptr<PlayerManager> PlayerManager::instance_ = nullptr;
PlayerManager::PlayerManager(int _playerNum)
{
	playerNum_ = _playerNum;
}

void PlayerManager::CreateInstance(int _playerNum)
{
	if (instance_ == nullptr)
	{
		instance_ = std::make_unique<PlayerManager>(_playerNum);
	}
	instance_->Init();
}

std::unique_ptr<PlayerManager>& PlayerManager::GetInstance(void)
{
	return instance_;
}

void PlayerManager::Init(void)
{
	for (int i = 0; i < playerNum_; i++)
	{
		std::shared_ptr<Player> player;
		player = std::make_shared<Player>(i);
		player->Init();
		players_.push_back(player);
	}
}

void PlayerManager::Update(void)
{
	for (auto& p : players_)
	{
		p->Update();
	}
}

void PlayerManager::Draw(void)
{
	for (auto& p : players_)
	{
		p->Draw();
	}
}

void PlayerManager::Release(void)
{
	for (auto& p : players_)
	{
		p->Release();
	}
}

Transform PlayerManager::SetTrans(int _playerNum)
{
	Transform trans = Transform();
	PLAYER_NUM num = static_cast<PLAYER_NUM>(_playerNum);
}
