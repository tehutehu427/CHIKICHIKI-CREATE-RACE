#include "../Player.h"
#include "../../../Manager/System/InputManager.h"
#include "../../../Manager/System/SceneManager.h"
#include "../../../Manager/System/Camera.h"
#include "./PlayerInput.h"
#include "../../../Utility/Utility.h"
#include "PMove.h"

PMove::PMove(void)
{
	speed_ = 0.0f;
	moveDir_ = Utility::VECTOR_ZERO;
	movePow_ = Utility::VECTOR_ZERO;
}

PMove::~PMove(void)
{
}

void PMove::Init(void)
{
}

void PMove::Update(const std::weak_ptr<Camera>& _camera, bool& _isJump, Transform& _trans)
{
	auto& ins = InputManager::GetInstance();
	movePow_ = Utility::VECTOR_ZERO;
	VECTOR dir = Utility::VECTOR_ZERO;
	VECTOR getDir = PlayerInput::GetInstance().GetDir();
	float deg = 0;
	//プレイヤーの周囲にあるステージポリゴンの取得
	//MV1_COLL_RESULT_POLY_DIM hitDim[STAGECOLLOBJ_MAXNUM + 1];
	Quaternion cameraRot = _camera.lock()->GetQuaRotOutX();
	Quaternion angle = Quaternion::AngleAxis(Utility::Deg2RadF(deg), Utility::AXIS_Y);
	//カメラ方向に移動したい
	if (PlayerInput::GetInstance().CheckAct(PlayerInput::ACT_CNTL::MOVE))
	{
		dir = cameraRot.PosAxis(getDir);
		deg = PlayerInput::GetInstance().GetMoveDeg();
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
		SetGoalRotate(Utility::Deg2RadF(deg), _camera);

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

void PMove::SetGoalRotate(double _deg, const std::weak_ptr<Camera> _camera)
{
	VECTOR cameraRot = _camera.lock()->GetAngles();
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

void PMove::Rotate(void)
{
	stepRotTime_ -= PlayerInput::DELTA_TIME;
	// 回転の球面補間
	playerRotY_ = Quaternion::Slerp(
		playerRotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);
}
