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
	//instance_->Init();
}

void PlayerManager::Destroy(void)
{
	instance_ = nullptr;
	delete instance_;
}

PlayerManager& PlayerManager::GetInstance(void)
{
	return *instance_;
}

void PlayerManager::Load(void)
{
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
		player = std::make_unique<Player>(i, cntl_);
		player->Load();
		players_.push_back(std::move(player));
	}
}

void PlayerManager::Init(void)
{
	if (playerNum_ > PLAYER_SINGLE) { cntl_ = PlayerInput::CNTL::PAD; }
	else { cntl_ = PlayerInput::CNTL::KEYBOARD; }
	for (auto& player : players_)
	{
		player->Init();
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
			if (Utility::IsHitSpheres(players_[i]->GetPos(),Player::RADIUS
				,players_[j]->GetPos(),Player::RADIUS))
			{
				players_[i]->SetCollision(true);
				players_[j]->SetCollision(true);
				//P2PPush(i, j);
			}

			//どちらがパンチ中かで吹っ飛ばす対象を決める
			if (players_[i]->GetIsPunch())
			{
				PunchPlayersColl(i, j);
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
}

void PlayerManager::SetInitPos(VECTOR _worldPos)
{
	for (int i = 0; i < playerNum_; i++)
	{
		float posX = i % 2 == 0 ? START_POS : -START_POS;
		float posZ = i < 2 ? START_POS : -START_POS;
		players_[i]->SetPos({ posX+_worldPos.x, _worldPos.y, posZ+_worldPos.z }) ;
	}
}

//void PlayerManager::P2PPush(int _pNum1,int _pNum2)
//{
//	//同じプレイヤー番号なら抜ける
//	if (_pNum1 == _pNum2)return;
//	//プレイヤー1の情報
//	VECTOR pos1 = players_[_pNum1]->GetTransform().pos;
//	std::weak_ptr<Capsule>p1Cap = players_[_pNum1]->GetCapsule();
//
//	//プレイヤー2の情報
//	VECTOR pos2 = players_[_pNum2]->GetTransform().pos;
//	std::weak_ptr<Capsule>p2Cap = players_[_pNum2]->GetCapsule();
//
//	VECTOR vec = VSub(players_[_pNum2]->GetTransform().pos
//		, players_[_pNum1]->GetTransform().pos);
//	float len = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
//	float overlap = p1Cap.lock()->GetRadius() + p2Cap.lock()->GetRadius() - len;
//	float weight = 0.5f;
//	//大きさを1にする
//	vec = VNorm(vec);
//	players_[_pNum1]->SetMovePow(VScale(vec, overlap * weight));
//	players_[_pNum2]->SetMovePow(VScale(vec, overlap * (1.0-weight)));
//		
//}

Transform PlayerManager::FixTrans(int _playerNum)
{
	Transform trans = Transform();
	//モデルできたら番号ごとで設定する
	ResourceManager& resIns = ResourceManager::GetInstance();
	//番号でモデルを変える(予定)
	trans.SetModel(resIns.LoadModelDuplicate(ResourceManager::SRC::CHICKEN));

	//transの初期化
	PLAYER num = static_cast<PLAYER>(_playerNum);
	
	trans.quaRot = Quaternion();
	trans.scl = MODEL_SCL;
	trans.quaRotLocal =
		Quaternion::Euler({ 0.0f, Utility::Deg2RadF(180.0f), 0.0f });

	trans.localPos = { 0.0f,-Player::RADIUS,0.0f };
	return trans;
}

void PlayerManager::PunchPlayersColl(int p1, int p2)
{
	//どちらもパンチしていなかったら処理しない
	//if (!players_[p1]->GetIsPunch() && !players_[p2]->GetIsPunch())return;

	//それぞれのカプセル
	//auto p1Cap = players_[p1]->GetCapsule().lock();
	//auto p2Cap = players_[p2]->GetCapsule().lock();

	//当たった時の処理
	if (Utility::IsHitSpheres(players_[p1]->GetPunchPos(), Player::PUNCH_RADIUS
		, players_[p2]->GetPos(),Player::RADIUS))
	{
		players_[p2]->SetDir(Utility::GetMoveVec(players_[p1]->GetPos(), players_[p2]->GetPos()));
		players_[p2]->SetIsPunched(true);
	}
}

void PlayerManager::DupilicateModel(void)
{
	//モデルを変えるかもしれないので一応配列で格納する
	int model = ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::CHICKEN);

	//配列を追加
	models_.emplace_back(model);
}
