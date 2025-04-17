#include "../Player.h"
#include "../../../Manager/System/InputManager.h"
#include "../../../Manager/System/SceneManager.h"
#include "../../../Manager/System/Camera.h"
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

void PMove::Update(const std::shared_ptr<Camera>& _camera, bool& _isJump )
{
	auto& ins = InputManager::GetInstance();
	movePow_ = Utility::VECTOR_ZERO;
	VECTOR dir = Utility::VECTOR_ZERO;

	//プレイヤーの周囲にあるステージポリゴンの取得
	//MV1_COLL_RESULT_POLY_DIM hitDim[STAGECOLLOBJ_MAXNUM + 1];
	Quaternion cameraRot = _camera->GetQuaRotOutX();

	//カメラ方向に移動したい
	if (ins.IsNew(KEY_INPUT_W))
	{
		dir = cameraRot.GetForward();
		SetGoalRotate(Utility::Deg2RadD(0.0),_camera);
	}
	if (ins.IsNew(KEY_INPUT_A))
	{
		dir = cameraRot.GetLeft();
		SetGoalRotate(Utility::Deg2RadD(270.0), _camera);
	}
	if (ins.IsNew(KEY_INPUT_S))
	{
		dir = cameraRot.GetBack();
		SetGoalRotate(Utility::Deg2RadD(180.0), _camera);
	}
	if (ins.IsNew(KEY_INPUT_D))
	{
		dir = cameraRot.GetRight();
		SetGoalRotate(Utility::Deg2RadD(90.0), _camera);
	}


	if (!Utility::EqualsVZero(dir) /*&& (_isJump || IsEndLanding())*/)
	{
		//入力でスピードを変える
		if (ins.IsNew(KEY_INPUT_LSHIFT))
		{
			speed_ = SPEED_RUN;
			//animationController_->Play((int)ANIM_TYPE::FAST_RUN);
		}
		else
		{
			speed_ = SPEED_MOVE;
			//animationController_->Play((int)ANIM_TYPE::RUN);
		}

		//if ((!_isJump && IsEndLanding()))
		//{
	

		//}

		moveDir_ = dir;
		//移動量
		movePow_ = VScale(moveDir_, speed_);


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

void PMove::SetGoalRotate(double rotRad, const std::shared_ptr<Camera>& _camera)
{
	VECTOR cameraRot = _camera->GetAngles();
	Quaternion axis =
		Quaternion::AngleAxis(
			(double)cameraRot.y + rotRad, Utility::AXIS_Y);
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
	stepRotTime_ -= SceneManager::GetInstance().GetDeltaTime();
	// 回転の球面補間
	playerRotY_ = Quaternion::Slerp(
		playerRotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);
}
