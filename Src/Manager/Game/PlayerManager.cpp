#include<cmath>
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
	if (playerNum_ > PLAYER_SINGLE) { cntl_ = PlayerInput::CNTL::PAD; }
	else { cntl_ = PlayerInput::CNTL::KEYBOARD; }
	for (int i = 0; i < playerNum_; i++)
	{
#ifdef DEBUG_ON
		if (i == 0)
		{
			cntl_ = PlayerInput::CNTL::KEYBOARD;
		}
		else
		{
			cntl_ = PlayerInput::CNTL::PAD;
		}
#endif // DEBUG_ON
		std::unique_ptr<Player> player;
		Transform trans=FixTrans(i);
		player = std::make_unique<Player>(i,trans, cntl_);
		player->Init();
		players_.push_back(std::move(player));
	}
}

void PlayerManager::Update(void)
{
	for (auto& p : players_)
	{
		p->Update();
	}
	PlayersCollision();
}

void PlayerManager::Draw(void)
{
	for (auto& p : players_)
	{
		p->Draw();
	}
}

void PlayerManager::PlayersCollision(void)
{
	for (int i = 0; i < playerNum_; i++)
	{
		players_[i]->SetCollision(false);
		for (int j = 0; j < playerNum_; j++)
		{
			//同じプレイヤー番号の時jを進める
			if (i == j)continue;
			//各プレイヤーに当たっていることを伝える
			if (IsHitCapsules(players_[i]->GetCapsule(), players_[j]->GetCapsule()))
			{
				players_[i]->SetCollision(true);
				players_[j]->SetCollision(true);
				P2PPush(i, j);
			}
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

void PlayerManager::P2PPush(int _pNum1,int _pNum2)
{
	//同じプレイヤー番号なら抜ける
	if (_pNum1 == _pNum2)return;
	//プレイヤー1の情報
	VECTOR pos1 = players_[_pNum1]->GetTransform().pos;
	std::weak_ptr<Capsule>p1Cap = players_[_pNum1]->GetCapsule();

	//プレイヤー2の情報
	VECTOR pos2 = players_[_pNum2]->GetTransform().pos;
	std::weak_ptr<Capsule>p2Cap = players_[_pNum2]->GetCapsule();

	VECTOR vec = VSub(players_[_pNum2]->GetTransform().pos
		, players_[_pNum1]->GetTransform().pos);
	float len = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	float overlap = p1Cap.lock()->GetRadius() + p2Cap.lock()->GetRadius() - len;
	float weight = 0.5f;
	//大きさを1にする
	vec = VNorm(vec);
	players_[_pNum1]->SetMovePow(VScale(vec, overlap * weight));
	players_[_pNum2]->SetMovePow(VScale(vec, overlap * (1.0-weight)));
		
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
