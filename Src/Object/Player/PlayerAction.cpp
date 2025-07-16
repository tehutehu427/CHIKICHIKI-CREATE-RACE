#include"../Utility/Utility.h"
#include"Player.h"
#include"Process/PlayerInput.h"
#include "../../Manager/Game/GravityManager.h"
#include "../../Manager/System/Camera.h"
#include "../../Manager/System/SceneManager.h"
#include "../../Manager/System/ResourceManager.h"
#include "../../Manager/System/SoundManager.h"
#include "../../Object/Common/EffectController.h"
#include "../../Object/Common/AnimationController.h"

#include "PlayerAction.h"

PlayerAction::PlayerAction(Player& _player, SceneManager& _scnMng, AnimationController& _animationController):
	player_(_player)
	,scnMng_(_scnMng)
	,animationController_(_animationController)
{
	//操作関連
	//----------------------------------------------------
	changeAction_.emplace(ATK_ACT::NONE, [this]() {ChangeNone(); });
	changeAction_.emplace(ATK_ACT::MOVE, [this]() {ChangeMove(); });
	changeAction_.emplace(ATK_ACT::DASHMOVE, [this]() {ChangeDashMove(); });
	changeAction_.emplace(ATK_ACT::INPUT, [this]() {ChangeInput(); });
 	changeAction_.emplace(ATK_ACT::JUMP, [this]() {ChangeJump(); });
	changeAction_.emplace(ATK_ACT::PUNCH, [this]() {ChangePunch(); });
	changeAction_.emplace(ATK_ACT::KNOCKBACK, [this]() {ChangeKnockBack(); });

	//エフェクト
	effect_ = std::make_unique<EffectController>();
	
	//ジャンプ関係
	isJump_ = false;
	stepJump_ = 0.0f;
	jumpPow_ = Utility::VECTOR_ZERO;
	jumpDeceralation_ = POW_JUMP;

	//パンチ関係の初期化
	punchCnt_ = 0.0f;
	punchCoolCnt_ = 0.0f;
	punchPos_ = Utility::VECTOR_ZERO;
	punchedCnt_ = PUNCHED_TIME;

	effectArrayNum_ = 0.0f;

	dashSeCnt_ = 0.0f;

	isPunchHitTime_ = false;
	input_ = nullptr;
}

PlayerAction::~PlayerAction(void)
{
	StopResource();
}

void PlayerAction::Init(void)
{
	auto num = player_.GetPadNum();
	auto cntl = player_.GetCntl();
	//入力
	input_ = std::make_unique<PlayerInput>(num, cntl);
	input_->Init();

	//ジャンプ関係
	isJump_ = false;
	stepJump_ = 0.0f;
	jumpPow_ = Utility::VECTOR_ZERO;
	jumpDeceralation_ = POW_JUMP;
  	movePow_ = Utility::VECTOR_ZERO;

	//スピード
	speed_ = 0.0f;

	dashSeCnt_ = 0.0f;

	isPunchHitTime_ = false;

	//パンチ関係の初期化
	punchCnt_ = 0.0f;
	punchCoolCnt_ = 0.0f;
	punchPos_ = Utility::VECTOR_ZERO;
	punchedCnt_ = PUNCHED_TIME;

	effectArrayNum_ = 0.0f;


	if (scnMng_.GetInstance().GetSceneID() == SceneManager::SCENE_ID::TITLE)
	{
		cameraNo_ = 0;
	}
	else
	{
		cameraNo_ = player_.GetPlayerNum();
	}

	ChangeAction(ATK_ACT::INPUT);
}

void PlayerAction::Load(void)
{
	auto& res = ResourceManager::GetInstance();
	actSE_.emplace(ACT_SE::DASH,SoundManager::SRC::PLAYER_DASH_START);
	actSE_.emplace(ACT_SE::JUMP,SoundManager::SRC::PLAYER_JUMP);
	actSE_.emplace(ACT_SE::PUNCH, SoundManager::SRC::PLAYER_PUNCH_MOTION);
	actSE_.emplace(ACT_SE::SLIME, SoundManager::SRC::SLIME_SE);

	effect_->Add(res.Load(ResourceManager::SRC::DASH_EFF).handleId_, EffectController::EFF_TYPE::DASH);
	effect_->Add(res.Load(ResourceManager::SRC::JUMP_EFF).handleId_, EffectController::EFF_TYPE::JUMP);
	effect_->Add(res.Load(ResourceManager::SRC::LANDING_EFF).handleId_, EffectController::EFF_TYPE::LANDING);
	effect_->Add(res.Load(ResourceManager::SRC::PUNCH_HIT_EFF).handleId_, EffectController::EFF_TYPE::PUNCH_HIT);
}

void PlayerAction::Update(void)
{
	//入力更新
	input_->Update();

	//エフェクト更新
	effect_->Update();

	//プレイヤーの下を設定
	static VECTOR dirDown = player_.GetTransform().GetDown();

	//重力(各アクションに重力を反映させたいので先に重力を先に書く)
	GravityManager::GetInstance().CalcGravity(dirDown,jumpPow_, GRAVITY_PER);

	//各アクションの更新
	actionUpdate_();

	//プレイヤーの回転
	Rotate();

	//プレイヤーの方向とスピードの更新
	UpdateMoveDirAndPow();
}

void PlayerAction::DrawDebug(void)
{
	DrawFormatString(0, 300, 0x000000, "act(%d)", (int)input_->GetAct());
}

void PlayerAction::NoneUpdate(void)
{
	//何もしない
}

void PlayerAction::ActionInputUpdate(void)
{
	//入力に応じてアクションを変える
	using ACT_CNTL = PlayerInput::ACT_CNTL;
	if (input_->CheckAct(ACT_CNTL::MOVE))
	{
		ChangeAction(ATK_ACT::MOVE);
		return;
	}
	if (input_->CheckAct(ACT_CNTL::DASHMOVE))
	{
		ChangeAction(ATK_ACT::DASHMOVE);
		return;
	}
	if (input_->CheckAct(ACT_CNTL::PUNCH))
	{
		ChangeAction(ATK_ACT::PUNCH);
		return;
	}
	if (CheckJumpInput())
	{
		ChangeAction(ATK_ACT::JUMP);
		return;
	}
}

void PlayerAction::ChangeAction(ATK_ACT _act)
{
	if (act_ == _act)return;
	act_ = _act;
	changeAction_[act_]();
 }

void PlayerAction::ChangeInput(void)
{
	animationController_.Play(static_cast<int>(Player::ANIM_TYPE::IDLE));
	isJump_ = false;
	int effNum = effect_->GetPlayNum(EffectController::EFF_TYPE::DASH);
	//for (int i = 0; i <= effNum; i++)
	//{
	//	effect_->Delete(EffectController::EFF_TYPE::DASH, i);
	//}

	SoundManager::GetInstance().Stop(actSE_[ACT_SE::DASH]);
	actionUpdate_ = std::bind(&PlayerAction::ActionInputUpdate, this);
}

void PlayerAction::ChangeNone(void)
{
	actionUpdate_ = std::bind(&PlayerAction::NoneUpdate, this);
}

void PlayerAction::MoveUpdate(void)
{
	Speed();
	//移動中に入力が入った時の状態遷移
	if (CheckJumpInput())
	{
		ChangeAction(ATK_ACT::JUMP);
		return;
	}
	if (input_->CheckAct(PlayerInput::ACT_CNTL::MOVE))
	{
		ChangeAction(ATK_ACT::MOVE);
	}
	if (input_->CheckAct(PlayerInput::ACT_CNTL::DASHMOVE))
	{
		ChangeAction(ATK_ACT::DASHMOVE);
	}
	else if (input_->CheckAct(PlayerInput::ACT_CNTL::PUNCH))
	{
		speed_ = 0.0f;
		ChangeAction(ATK_ACT::PUNCH);
		return;
	}
	else if (!input_->CheckAct(PlayerInput::ACT_CNTL::MOVE)
		&&!input_->CheckAct(PlayerInput::ACT_CNTL::DASHMOVE))
	{
		speed_ = 0.0f;
		ChangeAction(ATK_ACT::INPUT);
		return;
	}

	//エフェクトの座標と角度の追従
	effect_->SetPos(EffectController::EFF_TYPE::DASH, 0, player_.GetTransform().pos);
	effect_->SetQuaRot(EffectController::EFF_TYPE::DASH, 0, player_.GetTransform().quaRot);

	float animationSpeed = Player::DEFAULT_ANIM_SPD * (speed_ / MOVE_SPEED)*3.0f;
	animationController_.SetAnimSpeed(animationSpeed);

	//ダッシュカウント
	if (act_ == ATK_ACT::DASHMOVE)
	{
		dashSeCnt_ += scnMng_.GetInstance().GetDeltaTime();
		if (dashSeCnt_ >= DASH_SE_TIME)
		{
			SoundManager::GetInstance().Play(actSE_[ACT_SE::DASH], SoundManager::PLAYTYPE::BACK);
			dashSeCnt_ = 0.0f;
		}
	}

	//入力方向の移動
	MoveDirFronInput();
}

void PlayerAction::MoveDirFronInput(void)
{
	//移動量を0にリセット
	movePow_ = Utility::VECTOR_ZERO;

	//プレイヤー入力クラスから角度を取得
	VECTOR getDir = input_->GetDir();
	float deg = input_->GetMoveDeg();

	Quaternion cameraRot = scnMng_.GetCamera(cameraNo_).lock()->GetQuaRotOutX();
	Quaternion angle = Quaternion::AngleAxis(Utility::Deg2RadF(deg), Utility::AXIS_Y);
	dir_ = cameraRot.PosAxis(getDir);
	dir_ = VNorm(dir_);

	if (!Utility::EqualsVZero(dir_))
	{
		//補完角度の設定(入力角度まで方向転換する)
		SetGoalRotate(deg);
	}
}

void PlayerAction::ChangeMove(void)
{
	speed_ = MOVE_SPEED;
	animationController_.Play(static_cast<int>(Player::ANIM_TYPE::WALK));
	if (player_.GetIsSlimeFloor())
	{
		SoundManager::GetInstance().Play(actSE_[ACT_SE::SLIME], SoundManager::PLAYTYPE::BACK);
	}

	int effNum = effect_->GetPlayNum(EffectController::EFF_TYPE::DASH);
	for (int i = 0; i <= effNum; i++)
	{
		effect_->Stop(EffectController::EFF_TYPE::DASH, i);
	}

	actionUpdate_ = std::bind(&PlayerAction::MoveUpdate, this);
}

void PlayerAction::ChangeDashMove(void)
{
	speed_ = DASH_SPEED;
	animationController_.Play(static_cast<int>(Player::ANIM_TYPE::WALK));

	auto& trans = player_.GetTransform();
	const float SCL = 10.0f;

	//エフェクトの再生
	effect_->Delete(EffectController::EFF_TYPE::DASH,0);
	effect_->Play(EffectController::EFF_TYPE::DASH, trans.pos, trans.quaRot, { SCL,SCL,SCL },50.0f);
	effectArrayNum_++;

	//スライム床内ならスライム音再生
	if (player_.GetIsSlimeFloor())
	{
		SoundManager::GetInstance().Stop(actSE_[ACT_SE::DASH]);
		SoundManager::GetInstance().Play(actSE_[ACT_SE::SLIME], SoundManager::PLAYTYPE::BACK);
	}
	actionUpdate_ = std::bind(&PlayerAction::MoveUpdate, this);
}



void PlayerAction::UpdateMoveDirAndPow(void)
{
	//方向の更新
	moveDir_ = dir_;
	//移動量の更新
	movePow_ = VScale(moveDir_, speed_);
}

void PlayerAction::Speed(void)
{
	if (input_->CheckAct(PlayerInput::ACT_CNTL::MOVE))
	{
		speed_ = MOVE_SPEED;
	}
	else if (input_->CheckAct(PlayerInput::ACT_CNTL::DASHMOVE))
	{
		speed_ = DASH_SPEED;
	}
	else
	{
		speed_ = 0.0f;
	}
	if (speed_ > 0.0f && player_.GetIsSlimeFloor())
	{
		//スライム床の上だけスピードを半分にする
		speed_ /= 2.0f;
	}
}

void PlayerAction::JumpUpdate(void)
{
	//移動入力があったらスピードセット
	Speed();
	//ジャンプ処理
	Jump();
}

void PlayerAction::Jump(void)
{
	//ステップジャンプを基準にジャンプ減衰量を決める
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();
	stepJump_ += deltaTime;

	//空中アニメーションステップのループ設定
	animationController_.SetEndLoop(JUMP_ANIM_LOOP_START_FRAME
		, JUMP_ANIM_LOOP_END_FRAME, JUMP_ANIM_ATTACK_BLEND_TIME);

	//ジャンプ中も移動できるようにする
	MoveDirFronInput();

	//ジャンプカウントが0以上なら
	if (stepJump_ > 0.0f)
	{
		stepJump_ += deltaTime;
		//プレイヤーが落下していたら
		if (jumpDeceralation_ < 0.0f)
		{
			animationController_.Play(static_cast<int>(Player::ANIM_TYPE::LAND));
		}
		//減衰量の計算
		float deceralation = stepJump_ * TIME_JUMP_SCALE;
		jumpDeceralation_ -= deceralation;

		//ジャンプ量に掛ける
		jumpPow_ = VScale(player_.GetTransform().GetUp(), jumpDeceralation_);
	}

	//地面に着いたらジャンプ関係の変数リセット
	if (!isJump_)
	{
		jumpDeceralation_ = POW_JUMP;
		jumpPow_ = Utility::VECTOR_ZERO;
		stepJump_ = 0.0f;

		//着地エフェクト
		Transform trans = player_.GetTransform();
		const float EFF_SCL = 30.0f;
		effect_->Play(EffectController::EFF_TYPE::LANDING, trans.pos, trans.quaRot, { EFF_SCL,EFF_SCL, EFF_SCL });

		//動いていた場合の移動量リセット
		speed_ = 0.0f;
		ChangeAction(ATK_ACT::INPUT);
		return;
	}
}

void PlayerAction::ChangeJump(void)
{
	//ジャンプ関係
  	isJump_ = true;
	stepJump_ = 0.0f;

	//プレイヤーの情報
	Transform trans = player_.GetTransform();
	//エフェクトのスケール
	const VECTOR EFF_SCL = { 10.0f,10.0f,10.0f };
	//アニメーションの再生
	animationController_.Play(
		(int)Player::ANIM_TYPE::JUMP, false, JUMP_ANIM_START_FRAME, JUMP_ANIM_END_FRAME);

	SoundManager::GetInstance().Play(actSE_[ACT_SE::JUMP], SoundManager::PLAYTYPE::BACK);

	//ジャンプエフェクト
	effect_->Stop(EffectController::EFF_TYPE::JUMP,0);
	effect_->Play(EffectController::EFF_TYPE::JUMP, trans.pos, trans.quaRot, EFF_SCL, 50.0f);

	if(player_.GetIsSlimeFloor())SetJumpDecel(SLIME_FLOOR_JUMP_POW);

	//パンチの当たり判定を消す
	isPunchHitTime_ = false;
	player_.KillPunchCol();

	//状態遷移
	actionUpdate_ = [this]() {JumpUpdate(); };
}

bool PlayerAction::CheckJumpInput(void)
{
	return input_->CheckAct(PlayerInput::ACT_CNTL::JUMP);
}

void PlayerAction::Punch(void)
{
	//プレイヤーの手の座標を設定する
	punchPos_ = MV1GetFramePosition(player_.GetTransform().modelId, HAND_FRAME_NUM);

	//アニメステップを取得して一定のところで攻撃判定を発生させる
	float animStep = animationController_.GetAnimStep();

	if (animStep > PUNCH_HIT_END_ANIM_STEP)
	{
		//パンチの当たり判定を消す
		isPunchHitTime_ = false;
		player_.KillPunchCol();
	}
	else if (animStep > PUNCH_HIT_START_ANIM_STEP)
	{
		//パンチの当たり判定を作る
		player_.MakePunchCol();
		isPunchHitTime_ = true;
	}
	
	//アニメーションが終わったら
	if (animationController_.IsEnd())
	{
		//パンチクールタイムセット
		punchCoolCnt_ = PUNCH_COOL_TIME;
		ChangeAction(ATK_ACT::INPUT);
	}
}

void PlayerAction::ChangePunch(void)
{
	punchCnt_ = 0.0f;
	punchCoolCnt_ = PUNCH_COOL_TIME;
	//アニメーション
	animationController_.Play((int)Player::ANIM_TYPE::PUNCH, false);

	SoundManager::GetInstance().Play(actSE_[ACT_SE::PUNCH], SoundManager::PLAYTYPE::BACK);

	actionUpdate_ = [this]() {Punch(); };
}

void PlayerAction::KnockBack(void)
{
	//カウントの減算
	punchedCnt_ -= scnMng_.GetDeltaTime();
	if (punchedCnt_ < 0.0f)
	{
		punchedCnt_ = PUNCHED_TIME;
		speed_ = 0.0f;
		ChangeAction(ATK_ACT::INPUT);
	}
}

void PlayerAction::ChangeKnockBack(void)
{
	//ダメージアニメーション
	//animationController_->Play((int)ANIM_TYPE::DAMAGE,true,)
	speed_ = FLY_AWAY_SPEED;
	actionUpdate_ = [this]() {KnockBack(); };
	//エフェクト
	Transform trans = player_.GetTransform();
	const float EFF_SCL = 15.0f;
	effect_->Play(EffectController::EFF_TYPE::PUNCH_HIT, trans.pos, trans.quaRot, { EFF_SCL,EFF_SCL, EFF_SCL });
	//SoundManager::GetInstance().Play(actSE_[ACT_SE::])
	//パンチの当たり判定を消す
	isPunchHitTime_ = false;
	player_.KillPunchCol();
	
}




VECTOR PlayerAction::AddPosRotate(VECTOR _followPos, Quaternion _followRot, VECTOR _localPos)
{
	//座標回転
	VECTOR addPos = _followRot.PosAxis(_localPos);

	//足したものを返す
	return VAdd(_followPos, addPos);
}

void PlayerAction::StopSe(const ACT_SE _se)
{
	for (auto& se : actSE_)
	{
		if (se.first == _se)continue;
		SoundManager::GetInstance().Stop(se.second);
	}
}

void PlayerAction::Rotate(void)
{
	stepRotTime_ -= SceneManager::GetInstance().GetDeltaTime();
	// 回転の球面補間
	playerRotY_ = Quaternion::Slerp(
		playerRotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);
}

void PlayerAction::SetGoalRotate(double _deg)
{
	//カメラの角度を取得
	VECTOR cameraRot = scnMng_.GetCamera(cameraNo_).lock()->GetAngles();
	Quaternion axis = Quaternion::AngleAxis(
		(double)cameraRot.y + Utility::Deg2RadF(_deg), Utility::AXIS_Y);


	 //現在設定されている回転との角度差を取る
	double angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	// しきい値
	if (angleDiff > 0.1)
	{
		stepRotTime_ = TIME_ROT;
	}
	goalQuaRot_ = axis;
}

void PlayerAction::StopResource(void)
{
	for (auto& se : actSE_)
	{
		SoundManager::GetInstance().Stop(se.second);
	}
	int effNum = effect_->GetPlayNum(EffectController::EFF_TYPE::DASH);
	for (int i = 0; i <= effNum; i++)
	{
		effect_->Stop(EffectController::EFF_TYPE::DASH, i);
	}
	
}
