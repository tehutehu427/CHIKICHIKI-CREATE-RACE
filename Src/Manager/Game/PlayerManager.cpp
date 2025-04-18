#include"../../Utility/Utility.h"
#include"../../Manager/System/ResourceManager.h"
#include "PlayerManager.h"
PlayerManager* PlayerManager::instance_ = nullptr;


PlayerManager::PlayerManager(int _playerNum)
{
	playerNum_ = _playerNum;
}

void PlayerManager::CreateInstance(int _playerNum)
{
	if (instance_ == nullptr)
	{
		instance_ = new PlayerManager(_playerNum);
	}
	instance_->Init();
}

PlayerManager& PlayerManager::GetInstance(void)
{
	return *instance_;
}

void PlayerManager::Init(void)
{
	if (playerNum_ > PLAYER_SINGLE) { cntl_ = PlayerInput::CNTL::PAD; }
	else { cntl_ = PlayerInput::CNTL::KEYBOARD; }
	for (int i = 0; i < playerNum_; i++)
	{
		std::shared_ptr<Player> player;
		Transform trans=FixTrans(i);
		player = std::make_shared<Player>(i,trans,cntl_);
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

Transform PlayerManager::FixTrans(int _playerNum)
{
	Transform trans = Transform();
	//”Ô†‚Åƒ‚ƒfƒ‹‚ğ•Ï‚¦‚é
	PLAYER num = static_cast<PLAYER>(_playerNum);
	float x = 0.0f;
	trans.quaRot = Quaternion();
	trans.scl = Utility::VECTOR_ONE;
	trans.quaRotLocal =
		Quaternion::Euler({ 0.0f, Utility::Deg2RadF(180.0f), 0.0f });
	
	x = PLAYER_ONE_POS_X + DISTANCE_POS * _playerNum;

	//ƒ‚ƒfƒ‹‚Å‚«‚½‚ç”Ô†‚²‚Æ‚Åİ’è‚·‚é
	//trans.modelId=trans.SetModel(ResourceManager::SRC::PLAYER1)
	
	trans.pos = { x,0.0f,0.0f };
	return trans;
}
