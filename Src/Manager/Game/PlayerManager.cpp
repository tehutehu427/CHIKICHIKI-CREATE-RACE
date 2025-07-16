#include<cmath>
#include"../../Utility/Utility.h"
#include"../../Manager/System/ResourceManager.h"
#include"../../Manager/System/Camera.h"
#include"../../Manager/System/SoundManager.h"
#include"../System/DateBank.h"
#include "PlayerManager.h"
PlayerManager* PlayerManager::instance_ = nullptr;


PlayerManager::PlayerManager(void)
{
	playerNum_ = 0;
}

PlayerManager::~PlayerManager(void)
{
	instance_ = nullptr;

}

void PlayerManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new PlayerManager();
	}
	//instance_->Init();
}

void PlayerManager::Destroy(void)
{
	//deleteしてからnullptr
	delete instance_;
	instance_ = nullptr;
}

PlayerManager& PlayerManager::GetInstance(void)
{
	return *instance_;
}

void PlayerManager::Load(void)
{
	//データバンクから人数を取得
	playerNum_ = DateBank::GetInstance().GetPlayerNum();

	for (int i = 0; i < playerNum_; i++)
	{
		goalTime_.emplace_back(0.0f);
	}

	for (int i = 0; i < playerNum_; i++)
	{
		KeyConfig::TYPE cntlType;
  		if (playerNum_==1)
		{
			cntlType = KeyConfig::TYPE::ALL;
		}
		else
		{
			cntlType = KeyConfig::TYPE::PAD;
		}
		std::unique_ptr<Player> player;

		//自分の持つタグを設定する
		Collider::TAG tag;
		tag = static_cast<Collider::TAG>(static_cast<int>(Collider::TAG::PLAYER1) + i);
		player = std::make_unique<Player>(i, cntlType, tag);


		player->Load();
		players_.push_back(std::move(player));

		SoundManager& sndMng = SoundManager::GetInstance();
		sndMng.LoadResource(SoundManager::SRC::PLAYER_DASH_START);
		sndMng.LoadResource(SoundManager::SRC::PLAYER_JUMP);
		sndMng.LoadResource(SoundManager::SRC::PLAYER_PUNCH_HIT);
		sndMng.LoadResource(SoundManager::SRC::PLAYER_PUNCH_MOTION);
		sndMng.LoadResource(SoundManager::SRC::SLIME_SE);
		sndMng.LoadResource(SoundManager::SRC::SPRING_SE);
	}
}

void PlayerManager::Init(void)
{
	for (auto& player : players_)
	{
		player->Init();
	}

	//色を設定する
	InitPlayerColor();
}

void PlayerManager::Update(void)
{
	//time_ += SceneManager::GetInstance().GetDeltaTime();
	for (auto& p : players_)
	{
		p->Update();
	}
	if (SceneManager::GetInstance().GetSceneID() == SceneManager::SCENE_ID::TITLE)
	{
		return;
	}

	for (int i = 0; i < playerNum_; i++)
	{
		if (players_[i]->IsDeath() || players_[i]->IsGoal())
		{
			auto camera =SceneManager::GetInstance().GetCamera(i).lock();
			if (camera->GetMode() != Camera::MODE::FREE_CONTROLL)
			{
				VECTOR pos = camera->GetPos();
				VECTOR targetPos = camera->GetTargetPos();
				VECTOR angle = camera->GetAngles();
				camera->ChangeMode(Camera::MODE::FREE_CONTROLL);
				camera->SetPos(pos);
				camera->SetTargetPos(targetPos);
				camera->SetAngles(angle);
			}
		}
	}
	//PlayersCollision();

	
	//for (int i = 0; i < playerNum_; i++)
	//{
	//	if (goalTime_[i] >= 0.0f)
	//	{
	//		continue;
	//	}
	//}

}

void PlayerManager::Draw(void)
{
	for (auto& p : players_)
	{
		p->Draw();
	}
}


const std::vector<float> PlayerManager::GetGoalTime(void) 
{
	for (int i = 0; i < playerNum_; i++)
	{
		goalTime_[i] = players_[i]->GetGoalTime();
	}
	return goalTime_;
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

bool PlayerManager::IsPlayersEnd(void)
{
	//プレイヤーがまだ操作中か調べる
	for (auto& player : players_)
	{
		//ゴールしていない、かつ倒れていない時
		if (!player->IsDeath() && !player->IsGoal())
		{
			return false;
		}
	}
	//終了
	return true;
}


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

void PlayerManager::InitPlayerColor()
{
	//マルチ 又は タイトル以外は設定を行わない
	if (SceneManager::GetInstance().GetSceneID() != SceneManager::SCENE_ID::MULTI
		&& SceneManager::GetInstance().GetSceneID() != SceneManager::SCENE_ID::TITLE)
	{
		//色はデフォルトの白になる
		return;
	}

	//各プレイヤーごとに色を設定
	for (int i = 0; i < players_.size(); i++)
	{
		players_[i]->ChangeModelColor(PLAYER_COLOR[i]);
	}
}

