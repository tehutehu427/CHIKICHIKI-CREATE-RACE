#include "../../Utility/Utility.h"
#include "../Application.h"

#include "../../Manager/Game/GravityManager.h"
#include "../../Manager/Game/MapEditer.h"
#include "../../Manager/Game/PlayerManager.h"
#include "../../Manager/System/ResourceManager.h"
#include "../../Manager/System/SceneManager.h"
#include"../../Manager/System/DateBank.h"

#include "../../Manager/System/Camera.h"

#include "../../Object/Common/Geometry/Sphere.h"
#include "../../Object/Common/Geometry/Line.h"
#include"../../Object/Common/Geometry/Model.h"

#include "../../Object/Common/AnimationController.h"
#include"../Item/Installation/MoveHoriFloor.h"
#include"../Item/Installation/MoveVerFloor.h"

#include "../../Object/Editor/EditController.h"

#include "../../Manager/Game/ItemManager.h"
#include"./PlayerAction.h"
#include"./PlayerOnHit.h"
#include "./Process/PlayerInput.h"

#include<algorithm>


#include "Player.h"
Player::Player(int _playerNum, KeyConfig::TYPE _cntl, const Collider::TAG _tag)
	:playerNum_(_playerNum)
	, cntl_(_cntl)
	, tag_(_tag)
{

	trans_ = Transform();

	//初めのJOYPADがkey_padなのでパッドの番号に合わせる
	//パッド番号を設定
	padNum_ = static_cast<KeyConfig::JOYPAD_NO>(playerNum_ + 1);


	//プレイヤー状態
	changeStates_.emplace(PLAYER_STATE::ALIVE, [this]() {ChangeAlive();});
	changeStates_.emplace(PLAYER_STATE::DEATH, [this]() {ChangeDeath(); });

	isCol_ = false;

	//コライダ作成
	//*****************************************************
	//接地しているときのライン(床上にとどまっているとき)
	//Lineを引くための上と下の座標をとる
	std::unique_ptr<Line>lineGeo = std::make_unique<Line>(trans_.pos,trans_.quaRot, LOCAL_DOWN_POS, LOCAL_UP_POS);
	MakeCollider(tag_, std::move(lineGeo));

	//プレイヤーの体
	std::unique_ptr<Sphere>bodySphereGeo = std::make_unique<Sphere>(trans_.pos, RADIUS);
	MakeCollider(tag_, std::move(bodySphereGeo));


	//現在の座標と移動後座標を結んだ線のコライダ(移動後座標はOnHitクラスのPosUpdate関数で更新する)
	std::unique_ptr<Line>moveLineGeo = std::make_unique<Line>(trans_.pos,trans_.quaRot, Utility::VECTOR_ZERO, Utility::VECTOR_ZERO);
	MakeCollider(tag_, std::move(moveLineGeo));

	isGoal_ = false;
	isDeath_ = false;
	//*****************************************************
}

Player::~Player(void)
{
}

void Player::Load(void)
{
	//アニメーションでmodelIdを使うので先にモデルセットする
	trans_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::CHICKEN));

	//リソースの読み込みなど
	animationController_ = std::make_unique<AnimationController>(trans_.modelId);
	animationController_->Add(static_cast<int>(ANIM_TYPE::IDLE), DEFAULT_SPD);
	animationController_->Add(static_cast<int>(ANIM_TYPE::WALK), DEFAULT_SPD);
	animationController_->Add(static_cast<int>(ANIM_TYPE::FALL), DEFAULT_SPD);
	animationController_->Add(static_cast<int>(ANIM_TYPE::JUMP), DEFAULT_SPD);
	animationController_->Add(static_cast<int>(ANIM_TYPE::LAND), DEFAULT_SPD);
	animationController_->Add(static_cast<int>(ANIM_TYPE::PUNCH), DEFAULT_SPD / PlayerAction::PUNCH_TIME_MAX);


	action_ = std::make_unique<PlayerAction>(*this, scnMng_, *animationController_);
	////プレイヤーの手(パンチの当たり判定)
	std::unique_ptr<Sphere>handSphereGeo = std::make_unique<Sphere>(action_->GetPunchPos(), PUNCH_RADIUS);
	MakeCollider(tag_, std::move(handSphereGeo));
}

void Player::Init(void)
{
	//Transformの設定
	trans_.quaRot = Quaternion();
	trans_.scl = MODEL_SCL;
	trans_.quaRotLocal = 
		Quaternion::Euler({ 0.0f, Utility::Deg2RadF(MODEL_LOCAL_DEG), 0.0f });

	float posX = PLAYER_ONE_POS_X + DISTANCE_POS * playerNum_;

	trans_.pos={ posX,0.0f,0.0f };

	trans_.localPos = { 0.0f,-Player::RADIUS,0.0f };

	isCol_ = false;
	isGoal_ = false;
	isDeath_ = false;

	//生存状態
	ChangeState(PLAYER_STATE::ALIVE);

	action_->Init();

	//当たり判定
	onHitCol_ = std::make_unique<PlayerOnHit>(*action_, colParam_, trans_);
	onHitCol_->Init();

	trans_.Update();
}

void Player::Update(void)
{
	animationController_->Update();
#ifdef DEBUG_ON
	//CubeMove();
#endif // DEBUG_ON

	//プレイヤー状態更新
	stateUpdate_();

	//回転の同期
	trans_.quaRot = action_->GetPlayerRotY();
	
	trans_.Update();
}

void Player::Draw(void)
{
	MV1DrawModel(trans_.modelId);
#ifdef DEBUG_ON
	DrawDebug();
#endif // DEBUG_ON
}

void Player::OnHit(const std::weak_ptr<Collider> _hitCol)
{
	Collider::TAG tag = _hitCol.lock()->GetTag();
	onHitCol_->Update(_hitCol);
}

#ifdef DEBUG_ON
void Player::DrawDebug(void)
{
	unsigned int color = 0xffffff;
	const int HIGH = 10;
	const int WIDTH = 200;
	if (playerNum_ == 0) { color = 0xffffff; }
	else if (playerNum_ == 1) { color = 0x550000; }
	else if (playerNum_ == 2) { color = 0x00ff00; }
	else if (playerNum_ == 3) { color = 0x0000ff; }
	if (isCol_) { color = 0xff0000; }

	onHitCol_->DrawDebug();


	VECTOR pow = action_->GetMovePow();
	VECTOR jumpPow = action_->GetJumpPow();
	VECTOR movedPos = onHitCol_->GetMovedPos();
	DrawFormatString(0, 16*(playerNum_*9), 0x000000
		, "角度(%.2f,%.2f,%.2f)\njumpDecel(%f)\nstepJump_(%f)\njumpPow(%f,%f,%f)\nmovedPos(%f,%f,%f)\nmovePow(%f,%f,%f)"
		, trans_.rot.x, trans_.rot.y, trans_.rot.z
		,action_->GetJumpDecel()
		,action_->GetStepJump()
		, jumpPow.x, jumpPow.y, jumpPow.z
		,movedPos.x,movedPos.y,movedPos.z
		, pow.x, pow.y, pow.z
	);

	//action_->DrawDebug();

	if (IsDeath())
	{
		static int OFFSET = 32;
		//リトライするかEditシーンに戻るか選べるようにする。
		DrawFormatString(Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y+ OFFSET *playerNum_, 0xff0000, "(%d)GameOver", playerNum_);
	}
	

	//DrawSphere3D(punchPos_, PUNCH_RADIUS, 4, 0xff0000, 0xff0000, isPunchHitTime_);
	//DrawFormatString(0, 80* (playerNum_ + 3), 0, "POS = %f,%f,%f", movedPos_.x, movedPos_.y, movedPos_.z);
	//DrawFormatString(0, 100* (playerNum_ + 3), 0, "jump = %d", static_cast<int>(isJump_));
}
void Player::ChangeState(PLAYER_STATE _state)
{
	state_ = _state;
	changeStates_[state_]();
}
void Player::ChangeAlive(void)
{
	stateUpdate_ = std::bind(&Player::AliveUpdate, this);
}
void Player::AliveUpdate(void)
{
	//奈落に落ちたら死に状態へ遷移
	if (IsDeath())
	{
		ChangeState(PLAYER_STATE::DEATH);
		return;
	}
	//アクション関係
	Action();

	//移動後座標の更新
	onHitCol_->PosUpdate();
}
void Player::ChangeDeath(void)
{
	stateUpdate_ = std::bind(&Player::DeathUpdate, this);
}
void Player::DeathUpdate(void)
{
	//死んだ時の処理
	//落ちているアニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::FALL), true);

	//アニメーションループ
	if (animationController_->GetAnimStep() >= FALL_ANIM_START)
	{
		animationController_->SetEndLoop(FALL_ANIM_START, FALL_ANIM_END, DEFAULT_SPD);
	}

}
#endif // DEBUG_ON
void Player::Action(void)
{
	//アクション関係の更新
	action_->Update();

	//死んだら何もしないようにする
	if (IsDeath())
	{
		//何もできないようにする
		action_->ChangeAction(PlayerAction::ATK_ACT::NONE);
	}
}

bool Player::IsDeath(void)
{
	//奈落に落ちるorデスオブジェクトに当たったら
	if (trans_.pos.y <= DEATH_POS_Y||isDeath_)
	{
		return true;
	}
	return false;
}

void Player::ChangeModelColor(const COLOR_F _colorScale)
{
	MV1SetEmiColorScale(trans_.modelId, _colorScale);
}