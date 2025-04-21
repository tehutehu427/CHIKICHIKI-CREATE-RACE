#include"../../Utility/Utility.h"
#include"../../Manager/System/ResourceManager.h"
#include"../../Utility/Utility.h"
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
	//if (playerNum_ > PLAYER_SINGLE) { cntl_ = PlayerInput::CNTL::PAD; }
	//else { cntl_ = PlayerInput::CNTL::KEYBOARD; }
	for (int i = 0; i < playerNum_; i++)
	{
		std::shared_ptr<Player> player;
		Transform trans=FixTrans(i);
		player = std::make_shared<Player>(i,trans, PlayerInput::CNTL::KEYBOARD);
		player->Init();
		players_.push_back(player);
	}
}

void PlayerManager::Update(void)
{
	//for (auto& p : players_)
	//{
	//	p->Update();
	//}
	players_[1]->Update();
	PlayersCollision();
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

void PlayerManager::PlayersCollision(void)
{
	bool isCol = false;
	for (int i = 0; i < playerNum_; i++)
	{
		for (int j = 0; j < playerNum_; j++)
		{
			if (i == j)continue;
			if (IsHitCapsules(players_[i]->GetCapsule(), players_[j]->GetCapsule()))
			{
				isCol = true;
			}
			players_[i]->SetCollision(isCol);
			players_[j]->SetCollision(isCol);
		}
	}
}

bool PlayerManager::IsHitCapsules(const std::weak_ptr<Capsule> cap1, const std::weak_ptr<Capsule> cap2)
{
	//カプセル１上の球体＆カプセル２
	if (Utility::IsHitSphereCapsule(cap1.lock()->GetPosTop(), cap1.lock()->GetRadius()
		, cap2.lock()->GetPosTop(), cap2.lock()->GetPosDown(), cap2.lock()->GetRadius()))
	{
		return true;
	}

	//カプセル１下の球体＆カプセル２
	if (Utility::IsHitSphereCapsule(cap1.lock()->GetPosDown(), cap1.lock()->GetRadius()
		, cap2.lock()->GetPosTop(), cap2.lock()->GetPosDown(), cap2.lock()->GetRadius()))
	{
		return true;
	}

	//カプセル２上の球体＆カプセル１
	if (Utility::IsHitSphereCapsule(cap2.lock()->GetPosTop(), cap2.lock()->GetRadius()
		, cap1.lock()->GetPosTop(), cap1.lock()->GetPosDown(), cap1.lock()->GetRadius()))
	{
		return true;
	}

	//カプセル２下の球体＆カプセル１
	if (Utility::IsHitSphereCapsule(cap2.lock()->GetPosDown(), cap2.lock()->GetRadius()
		, cap1.lock()->GetPosTop(), cap1.lock()->GetPosDown(), cap1.lock()->GetRadius()))
	{
		return true;
	}

	//カプセルがクロスしているときの判定
	VECTOR cap1PosTop_To_cap1PosDown_Vec = VSub(cap1.lock()->GetPosDown(), cap1.lock()->GetPosTop());
	VECTOR cap2PosTop_To_cap2PosDownVec = VSub(cap2.lock()->GetPosDown(), cap2.lock()->GetPosTop());

	return false;

	//return false;


}

Transform PlayerManager::FixTrans(int _playerNum)
{
	Transform trans = Transform();
	//番号でモデルを変える
	PLAYER num = static_cast<PLAYER>(_playerNum);
	float x = 0.0f;
	trans.quaRot = Quaternion();
	trans.scl = Utility::VECTOR_ONE;
	trans.quaRotLocal =
		Quaternion::Euler({ 0.0f, Utility::Deg2RadF(180.0f), 0.0f });
	
	x = PLAYER_ONE_POS_X + DISTANCE_POS * _playerNum;

	//モデルできたら番号ごとで設定する
	//trans.modelId=trans.SetModel(ResourceManager::SRC::PLAYER1)
	
	trans.pos = { x,0.0f,0.0f };
	return trans;
}
