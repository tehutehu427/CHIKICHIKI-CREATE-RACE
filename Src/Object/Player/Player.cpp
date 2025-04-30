#include "../../Utility/Utility.h"
#include "../../Manager/Game/GravityManager.h"
#include "../../Manager/System/ResourceManager.h"
#include "../../Manager/System/SceneManager.h"
#include "../../Object/Common/Capsule.h"
#include "./Process/PlayerInput.h"
#include "Player.h"

Player::Player(int _playerNum,Transform _trans,PlayerInput::CNTL _cntl):playerNum_(_playerNum), cntl_(_cntl)
{
#ifdef DEBUG_ON
	cubeMovePos_=Utility::VECTOR_ZERO;
	cubePos_=Utility::VECTOR_ZERO;
#endif // DEBUG_ON

	animationController_ = nullptr;
	movedPos_ = Utility::VECTOR_ZERO;

	//初めのJOYPADがkey_padなのでパッドの番号に合わせる
	padNum_ = static_cast<InputManager::JOYPAD_NO>(playerNum_ + 1);
	transform_ = _trans;
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

	capsule_ = std::make_shared<Capsule>(transform_);
	capsule_->SetLocalPosTop(CAPSULE_TOP);
	capsule_->SetLocalPosDown(CAPSULE_DOWN);
	capsule_->SetRadius(20.0f);
}

void Player::Init(void)
{
	transform_.Update();
}

void Player::Update(void)
{
	//入力更新
	input_->Update();
	//アクション関係
	Action();

	VECTOR dirDown = transform_.GetDown();
	//重力
	GravityManager::GetInstance()->CalcGravity(dirDown, jumpPow_);

#ifdef DEBUG_ON
	CubeMove();
#endif // DEBUG_ON

	//衝突判定
	Collision();
	//回転の同期
	transform_.quaRot = playerRotY_;


	transform_.Update();
}

void Player::Draw(void)
{
#ifdef DEBUG_ON
	DrawDebug();
#endif // DEBUG_ON
}


#ifdef DEBUG_ON
void Player::DrawDebug(void)
{
	unsigned int color = 0xffffff;
	if (playerNum_ == 0) { color = 0xffffff; }
	else if (playerNum_ == 1) { color = 0x550000; }
	else if (playerNum_ == 2) { color = 0x00ff00; }
	else if (playerNum_ == 3) { color = 0x0000ff; }
	if (isCol_) { color = 0xff0000; }
	DrawSphere3D(transform_.pos, 10.0f, 10, color, color, true);
	DrawFormatString(0, 16, 0xffffff, "角度(%.2f,%.2f,%.2f)", transform_.rot.x, transform_.rot.y, transform_.rot.z);
	capsule_->Draw();

	DrawSphere3D(punchPos_, 10.0f, 4, 0xff0000, 0xff0000, isPunch_);


	//DrawSphere3D(cube_.centerPos, 7.0f, 4, 0xff0000, 0xff0000, true);
	//DrawSphere3D(cube_.leftPos, 7.0f, 4, 0xff0000, 0xff0000, true);
	//DrawSphere3D(cube_.rightPos, 7.0f, 4, 0xff0000, 0xff0000, true);
	//DrawSphere3D(cube_.upPos, 7.0f, 4, 0xff0000, 0xff0000, true);
	//DrawSphere3D(cube_.downPos, 7.0f, 4, 0xff0000, 0xff0000, true);

	//DrawCube3D(VAdd(cube_.centerPos, { -CUBE_W / 2, -CUBE_H / 2,-CUBE_D / 2 }), VAdd(cube_.centerPos, { CUBE_W / 2, CUBE_H / 2, CUBE_D / 2 })
	//	, 0xffffff, 0xffffff, true);
}
void Player::Action(void)
{
	Rotate();
	if (!isPunch_)
	{
		Jump();
		Move();
	}
	Punch();
}

void Player::Move(void)
{
	movePow_ = Utility::VECTOR_ZERO;
	VECTOR dir = Utility::VECTOR_ZERO;
	VECTOR getDir = input_->GetDir();
	float deg = 0;
	//プレイヤーの周囲にあるステージポリゴンの取得
	//MV1_COLL_RESULT_POLY_DIM hitDim[STAGECOLLOBJ_MAXNUM + 1];
	Quaternion cameraRot = scnMng_.GetCamera().lock()->GetQuaRotOutX();
	Quaternion angle = Quaternion::AngleAxis(Utility::Deg2RadF(deg), Utility::AXIS_Y);
	//カメラ方向に移動したい
	if (input_->CheckAct(PlayerInput::ACT_CNTL::MOVE))
	{
		dir = cameraRot.PosAxis(getDir);
		deg = input_->GetMoveDeg();
	}

	if (!Utility::EqualsVZero(dir) /*&& (_isJump || IsEndLanding())*/)
	{
		speed_ = MOVE_SPEED;
		//animationController_->Play((int)ANIM_TYPE::RUN);

		//if ((!_isJump && IsEndLanding()))
		//{


		//}

		moveDir_ = dir;
		//移動量
		movePow_ = VScale(moveDir_, speed_);
		SetGoalRotate(Utility::Deg2RadF(deg));

	}
	else
	{
		//speed_ = 0.0f;
		//animationController_->Play((int)ANIM_TYPE::IDLE);

		//if (!_isJump && IsEndLanding())
		//{

		//}
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
	bool isHit = input_->CheckAct(PlayerInput::ACT_CNTL::JUMP);
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();
	// ジャンプ
	if (isHit && (isJump_ || IsEndLanding()))
	{
		if (!isJump_)
		{
			// 空中で無理やりアニメーションを切り取ってアニメーションをする
			//animationController_->Play(
			//	(int)ANIM_TYPE::JUMP, true, 13.0f, 25.0f);
			//animationController_->SetEndLoop(23.0f, 25.0f, 5.0f);

			// この後、いくつかのジャンプパターンを試します
		}
		isJump_ = true;
	}

	if (isJump_)
	{
		// ジャンプの入力受付時間を減らす
		stepJump_ += scnMng_.GetDeltaTime();
		if (stepJump_ < TIME_JUMP_IN)
		{
			jumpDeceralation_ -= stepJump_ * TIME_JUMP_IN;
			jumpPow_ = VScale(transform_.GetUp(), jumpDeceralation_);
		}
		else
		{
			jumpDeceralation_ += (TIME_JUMP_IN - stepJump_) * TIME_JUMP_IN;
			jumpPow_ = VScale(transform_.GetUp(), jumpDeceralation_);
		}
	}
	// ボタンを離したらジャンプ力に加算しない
	else
	{
		stepJump_ = TIME_JUMP_IN;
		jumpDeceralation_ = POW_JUMP;
		fallCnt_ = 0.0f;
		jumpPow_ = Utility::VECTOR_ZERO;
	} 
}
void Player::Punch(void)
{
	//座標を足す
	punchPos_ = AddPosRotate(transform_.pos, transform_.quaRot, PUNCH_LOCAL_POS);
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
	movedPos_ = VAdd(transform_.pos, movePow_);
	movedPos_ = VAdd(movedPos_, jumpPow_);
#ifdef DEBUG_ON
	if (movedPos_.y < 0.0f)
	{
		movedPos_.y = 0.0f;
		isJump_ = false;
		stepJump_ = 0.0f;
	}
#endif // DEBUG_ON
	//移動前の座標を格納する
	//moveDiff_ = VSub(movedPos_, transform_.pos);
	// 移動
	transform_.pos = movedPos_;
	// 現在座標を起点に移動後座標を決める

}

bool Player::IsEndLanding(void)
{
	return true;
}

void Player::CubeMove(void)
{
	auto& input = InputManager::GetInstance();
	const float SPD = 3.0f;
	cubeMovePos_ = Utility::VECTOR_ZERO;
	if (input.IsNew(KEY_INPUT_UP))cubeMovePos_.y += SPD;
	if (input.IsNew(KEY_INPUT_DOWN))cubeMovePos_.y -= SPD;
	if (input.IsNew(KEY_INPUT_RIGHT))cubeMovePos_.x += SPD;
	if (input.IsNew(KEY_INPUT_LEFT))cubeMovePos_.x -= SPD;
	cube_.centerPos=VAdd(cube_.centerPos, cubeMovePos_);
}

bool Player::CollCube(void)
{
	cube_.leftPos = VAdd(cube_.centerPos, { -(CUBE_W / 2.0f),0.0f,0.0f });
	cube_.rightPos = VAdd(cube_.centerPos, { CUBE_W / 2.0f,0.0f,0.0f });
	cube_.upPos = VAdd(cube_.centerPos, { 0.0f,CUBE_H / 2.0f,0.0f });
	cube_.downPos = VAdd(cube_.centerPos, { 0.0f,-(CUBE_H / 2.0f),0.0f });

	VECTOR diff = VSub(cube_.centerPos, transform_.pos);
	if(fabsf(diff.x)>CUBE_W/2+RADIUS
		||fabsf(diff.y)>CUBE_H/2+RADIUS
		|| fabsf(diff.z) > CUBE_D / 2 + RADIUS)
	{
		return false;
	}
	return true;
}
