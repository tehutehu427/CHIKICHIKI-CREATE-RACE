#include "../../Utility/Utility.h"
#include "../../Manager/Game/GravityManager.h"
#include "../../Manager/System/ResourceManager.h"
#include "../../Manager/System/SceneManager.h"
#include "../../Object/Common/Capsule.h"
#include "../../Object/Common/AnimationController.h"
#include "./Process/PlayerInput.h"
#include "Player.h"

Player::Player(int _playerNum,Transform _trans,PlayerInput::CNTL _cntl):playerNum_(_playerNum), cntl_(_cntl)
{
#ifdef DEBUG_ON
	cubeMovePos_=Utility::VECTOR_ZERO;
	cubePos_=Utility::VECTOR_ZERO;
	//cast_ = { [this](ANIM_TYPE type){return static_cast<int>(type); }};
#endif // DEBUG_ON

	trans_ = _trans;

	animationController_ = std::make_shared<AnimationController>(trans_.modelId);
	animationController_->Add(static_cast<int>(ANIM_TYPE::IDLE), DEFAULT_SPD);
	animationController_->Add(static_cast<int>(ANIM_TYPE::WALK), DEFAULT_SPD);
	animationController_->Add(static_cast<int>(ANIM_TYPE::JUMP), DEFAULT_SPD);
	animationController_->Add(static_cast<int>(ANIM_TYPE::LAND), DEFAULT_SPD);
	animationController_->Add(static_cast<int>(ANIM_TYPE::PUNCH), DEFAULT_SPD);
	movedPos_ = Utility::VECTOR_ZERO;

	//初めのJOYPADがkey_padなのでパッドの番号に合わせる
	padNum_ = static_cast<InputManager::JOYPAD_NO>(playerNum_ + 1);
	
	//オブジェクト生成
	//操作関連
	//---------------------------------
	//入力
	input_ = std::make_shared<PlayerInput>(padNum_, cntl_);
	//当たり判定
	isCol_ = false;

	isJump_ = false;
	stepJump_ = 0.0f;
	jumpPow_ = Utility::VECTOR_ZERO;
	jumpDeceralation_ = POW_JUMP;

	//パンチ関係の初期化
	punchCnt_ = 0.0f;
	punchCoolCnt_ = 0.0f;
	isPunch_ = false;
	punchPos_ = Utility::VECTOR_ZERO;
	isPunched_ = false;
	punchedCnt_ = PUNCHED_TIME;

}

void Player::Load(void)
{
	//リソースの読み込みなど
}

void Player::Init(void)
{
	trans_.Update();
}

void Player::Update(void)
{
	//入力更新
	input_->Update();
	animationController_->Update();

#ifdef DEBUG_ON
	CubeMove();
#endif // DEBUG_ON

	static VECTOR dirDown = trans_.GetDown();
	//重力(各アクションに重力を反映させたいので先に重力を先に書く)
	GravityManager::GetInstance()->CalcGravity(dirDown, jumpPow_);


	//衝突判定
	Collision();
	//アクション関係
	Action();
	//回転の同期
	trans_.quaRot = playerRotY_;


	trans_.Update();
}

void Player::Draw(void)
{
	MV1DrawModel(trans_.modelId);
#ifdef DEBUG_ON
	DrawDebug();
#endif // DEBUG_ON
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
	DrawSphere3D(trans_.pos, RADIUS, 10, color, color, false);
	DrawFormatString(0, 16, 0x000000
		, "角度(%.2f,%.2f,%.2f)\njumpDecel(%f)\nstepJump_(%f)"
		, trans_.rot.x, trans_.rot.y, trans_.rot.z
		,jumpDeceralation_
		,stepJump_
	);

	DrawSphere3D(punchPos_, PUNCH_RADIUS, 4, 0xff0000, 0xff0000, isPunch_);

	DrawCube3D({ cube_.centerPos.x - CUBE_W,cube_.centerPos.y - CUBE_H,cube_.centerPos.z - CUBE_D }
	, { cube_.centerPos.x + CUBE_W,cube_.centerPos.y + CUBE_H,cube_.centerPos.z + CUBE_D }, 0xff0000, 0xff0000, true);
	DrawFormatString(0, 80, 0, "POS = %f,%f,%f", movedPos_.x, movedPos_.y, movedPos_.z);
	DrawFormatString(0, 100, 0, "jump = %d", static_cast<int>(isJump_));
}
void Player::Action(void)
{
	Rotate();
	Punch();
	Jump();
	Move();

}

void Player::Move(void)
{
	movePow_ = Utility::VECTOR_ZERO;
	if (!isPunched_)
	{
		dir_ = Utility::VECTOR_ZERO;
	}
	VECTOR getDir = input_->GetDir();
	float deg = 0;
	Quaternion cameraRot = scnMng_.GetCamera().lock()->GetQuaRotOutX();
	Quaternion angle = Quaternion::AngleAxis(Utility::Deg2RadF(deg), Utility::AXIS_Y);
	//吹き飛び中でなかったらカメラ方向に移動したい
	if (input_->CheckAct(PlayerInput::ACT_CNTL::MOVE)&&!isPunched_&&!isPunch_)
	{
		deg = input_->GetMoveDeg();
		dir_ = cameraRot.PosAxis(getDir);
		dir_ = VNorm(dir_);
	}

	if (!Utility::EqualsVZero(dir_))
	{
		//パンチされてぶっ飛んでる時と通常の移動の時のスピード
		if (isPunched_){speed_ = FLY_AWAY_SPEED;}
		else{speed_ = MOVE_SPEED;}
		
		animationController_->Play(static_cast<int>(ANIM_TYPE::WALK));
		moveDir_ = dir_;
		//移動量
		movePow_ = VScale(moveDir_, speed_);
		SetGoalRotate(Utility::Deg2RadF(deg));
	}
	else
	{
		speed_ = 0.0f;
		animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE));
	}
}
void Player::Rotate(void)
{
	stepRotTime_ -= PlayerInput::DELTA_TIME;
	// 回転の球面補間
	playerRotY_ = Quaternion::Slerp(
		playerRotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);
}
void Player::SetGoalRotate(double _deg)
{
	VECTOR cameraRot = scnMng_.GetCamera().lock()->GetAngles();
	Quaternion axis = Quaternion::AngleAxis(
		(double)cameraRot.y + _deg, Utility::AXIS_Y);
	// 現在設定されている回転との角度差を取る
	double angleDiff = Quaternion::Angle(axis, goalQuaRot_);
	// しきい値
	if (angleDiff > 0.1)
	{
		stepRotTime_ = TIME_ROT;
	}
	goalQuaRot_ = axis;
}
void Player::Jump(void)
{
	if (isPunch_)return;
	bool isHit = input_->CheckAct(PlayerInput::ACT_CNTL::JUMP);
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();
	// ジャンプ
	if (isHit)
	{
		if (!isJump_)
		{
			stepJump_ = 0.0f;
			 //空中で無理やりアニメーションを切り取ってアニメーションをする
			animationController_->Play(
				(int)ANIM_TYPE::JUMP, false, 0.0f, 60.0f);
			//animationController_->SetEndLoop(23.0f, 25.0f, 5.0f);

			// この後、いくつかのジャンプパターンを試します
		}
		isJump_ = true;
		//ジャンプの入力受付時間を減らす
		stepJump_ += deltaTime;
	}

	if (stepJump_ > 0.0f)
	{
		if (!isJump_)
		{
			stepJump_ = 0.0f;
			return;
		}
			
		stepJump_ += deltaTime;
		float i = stepJump_ * TIME_JUMP_SCALE;
		jumpDeceralation_ -= i;
		jumpPow_ = VScale(trans_.GetUp(), jumpDeceralation_);
	}

	// ボタンを離したらジャンプ力に加算しない
	if(!isJump_)
	{
		jumpDeceralation_ = POW_JUMP;
		fallCnt_ = 0.0f;
		jumpPow_ = Utility::VECTOR_ZERO;
	} 
}
void Player::Punch(void)
{
	//座標を足す
	punchPos_ = AddPosRotate(trans_.pos, trans_.quaRot, PUNCH_LOCAL_POS);
	if (!isJump_ && punchCoolCnt_ >= 0.0f)
	{
		punchCoolCnt_ -= scnMng_.GetDeltaTime();
		return;
	}
	auto isHit = input_->CheckAct(PlayerInput::ACT_CNTL::PUNCH);
	if (punchCnt_ > PUNCH_TIME_MAX)
	{
		punchCnt_ = 0.0f;
		isPunch_ = false;
		punchCoolCnt_ = PUNCH_COOL_TIME;
		return;
	}
	if (isHit){isPunch_ = true;}
	if (isPunch_){punchCnt_ += PlayerInput::DELTA_TIME;}

	if (isPunched_)
	{
		punchedCnt_ -= scnMng_.GetDeltaTime();
	}
	if (punchedCnt_ < 0.0f)
	{
		isPunched_ = false;
		punchedCnt_ = PUNCHED_TIME;
	}
}
#endif // DEBUG_ON



VECTOR Player::AddPosRotate(VECTOR _followPos, Quaternion _followRot, VECTOR _localPos)
{
	//座標回転
	VECTOR addPos = _followRot.PosAxis(_localPos);

	//足したものを返す
	return VAdd(_followPos, addPos);
}

void Player::CalcGravityPow(void)
{

}

void Player::Collision(void)
{
	movedPos_ = VAdd(trans_.pos, movePow_);
	movedPos_ = VAdd(movedPos_, jumpPow_);
	if (CollCube())
	{
		movedPos_ = VAdd(movedPos_, cubeMovePos_);
		movedPos_.y = cube_.upPos.y + RADIUS;
		isJump_ = false;
	}
	else
	{
		isJump_ = true;
	}

#ifdef DEBUG_ON

	//if (movedPos_.y < 0.0f/*||!CollCube()*/)
	//{
	//	movedPos_.y = 0.0f;
	//	isJump_ = false;
	//	stepJump_ = 0.0f;
	//}
#endif // DEBUG_ON
	//移動前の座標を格納する
	moveDiff_ = VSub(movedPos_, trans_.pos);
	// 移動
	trans_.pos = movedPos_;
	// 現在座標を起点に移動後座標を決める
}


void Player::CubeMove(void)
{
	auto& input = InputManager::GetInstance();
	const float SPD = 7.0f;
	cube_.upPos = VAdd(cube_.centerPos, { 0.0f,CUBE_H,0.0f });
	cubeMovePos_ = Utility::VECTOR_ZERO;
	if (input.IsNew(KEY_INPUT_UP))cubeMovePos_.z += SPD;
	if (input.IsNew(KEY_INPUT_DOWN))cubeMovePos_.z -= SPD;
	if (input.IsNew(KEY_INPUT_RIGHT))cubeMovePos_.x += SPD;
	if (input.IsNew(KEY_INPUT_LEFT))cubeMovePos_.x -= SPD;
	cube_.centerPos=VAdd(cube_.centerPos, cubeMovePos_);
}

bool Player::CollCube(void)
{
	VECTOR diff = VSub(cube_.centerPos, movedPos_);
	if(fabsf(diff.x) > CUBE_W+RADIUS
		||fabsf(diff.y) > CUBE_H+RADIUS
		|| fabsf(diff.z) > CUBE_D + RADIUS)
	{
		return false;
	}
	return true;
}
