#include "../../Utility/Utility.h"
#include "../../Manager/Game/GravityManager.h"
#include "../../Manager/System/ResourceManager.h"
#include "../../Manager/System/SceneManager.h"
#include "../../Object/Common/Capsule.h"
#include "./Process/PlayerInput.h"
#include "Player.h"

Player::Player(int _playerNum,Transform _trans,PlayerInput::CNTL _cntl):playerNum_(_playerNum), cntl_(_cntl)
{
	animationController_ = nullptr;
	state_ = STATE::NONE;
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


	capsule_ = std::make_shared<Capsule>(transform_);
	capsule_->SetLocalPosTop({ 0.0f, 110.0f, 0.0f });
	capsule_->SetLocalPosDown({ 0.0f, 0.0f, 0.0f });
	capsule_->SetRadius(20.0f);

	//状態管理
	stateChanges_.emplace(STATE::NONE, std::bind(&Player::ChangeStateNone, this));
	stateChanges_.emplace(STATE::PLAY, std::bind(&Player::ChangeStatePlay, this));
}

void Player::Init(void)
{
	//transform_.modelId = transform_.SetModel(resMng_.GetInstance().LoadModelDuplicate(ResourceManager::SRC::)
	//transform_.scl = Utility::VECTOR_ONE;
	//transform_.pos = { 0.0f, 0.0f, 0.0f };
	//transform_.quaRot = Quaternion();
	//transform_.quaRotLocal =
	//	Quaternion::Euler({ 0.0f, Utility::Deg2RadF(180.0f), 0.0f });
	transform_.Update();

	// 初期状態
	ChangeState(STATE::PLAY);
}

void Player::Update(void)
{
	//入力更新
	input_->Update();

	//アクション関係
	Move();
	Jump();
	Punch();
	Rotate();

	//衝突判定
	Collision();
	//回転の同期
	transform_.quaRot = playerRotY_;



	// 更新ステップ
	stateUpdate_();

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
	//pJump_->DrawDebug();
	//pPunch_->DrawDebug();

	DrawSphere3D(punchPos_, 10.0f, 4, 0xff0000, 0xff0000, isPunch_);
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
		speed_ = SPEED_MOVE;
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
		speed_ = 0.0f;
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
		stepJump_ += PlayerInput::DELTA_TIME;
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
	} 
}
void Player::Punch(void)
{
	//座標を足す
	punchPos_ = AddPosRotate(transform_.pos, transform_.quaRot, PUNCH_LOCAL_POS);
	if (cnt_ > PUNCH_TIME_MAX)
	{
		cnt_ = 0.0f;
		isPunch_ = false;
		return;
	}
	if (input_->CheckAct(PlayerInput::ACT_CNTL::PUNCH))
	{
		isPunch_ = true;
	}
	if (isPunch_)
	{
		cnt_ += PlayerInput::DELTA_TIME;
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

void Player::ChangeState(STATE _state)
{
	//同じ状態だったら処理しない
	if (state_ == _state)return;

	//状態変更
	state_ = _state;

	//各状態への初期処理
	stateChanges_[state_]();
}

void Player::ChangeStateNone(void)
{
	stateUpdate_ = std::bind(&Player::UpdateNone,this);
}

void Player::ChangeStatePlay(void)
{
	stateUpdate_ = std::bind(&Player::UpdatePlay, this);
}

void Player::UpdateNone(void)
{

}

void Player::UpdatePlay(void)
{

}

void Player::CalcGravityPow(void)
{
}

void Player::Collision(void)
{
	// 現在座標を起点に移動後座標を決める
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
}

bool Player::IsEndLanding(void)
{
	return true;
}
