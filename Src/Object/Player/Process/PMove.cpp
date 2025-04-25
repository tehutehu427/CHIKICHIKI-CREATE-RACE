#include "../Player.h"
#include "../../../Manager/System/InputManager.h"
#include "../../../Manager/System/SceneManager.h"
#include "../../../Manager/System/Camera.h"
#include "./PlayerInput.h"
#include "../../../Utility/Utility.h"
#include "PMove.h"

PMove::PMove(std::weak_ptr<PlayerInput>_input):input_(_input)
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

void PMove::Update(const std::weak_ptr<Camera>& _camera, bool& _isJump)
{

}

void PMove::SetGoalRotate(double _deg, const std::weak_ptr<Camera> _camera)
{
	VECTOR cameraRot = _camera.lock()->GetAngles();
	Quaternion axis = Quaternion::AngleAxis(
		(double)cameraRot.y + _deg, Utility::AXIS_Y);
	// Œ»ÝÝ’è‚³‚ê‚Ä‚¢‚é‰ñ“]‚Æ‚ÌŠp“x·‚ðŽæ‚é
	double angleDiff = Quaternion::Angle(axis, goalQuaRot_);
	// ‚µ‚«‚¢’l
	if (angleDiff > 0.1)
	{
		stepRotTime_ = TIME_ROT;
	}
	goalQuaRot_ = axis;
}

void PMove::Rotate(void)
{

}
