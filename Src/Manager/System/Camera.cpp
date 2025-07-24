#include <math.h>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../../Utility/Utility.h"
#include "../../Object/Common/Transform.h"
#include "../../Manager/Game/GravityManager.h"
#include "../../Manager/System/DateBank.h"
#include "../../Application.h"
#include "Camera.h"

Camera::Camera(int _playerNum)
{
	angles_ = VECTOR();
	cameraUp_ = VECTOR();
	mode_ = MODE::NONE;
	pos_ = Utility::VECTOR_ZERO;
	targetPos_ = Utility::VECTOR_ZERO;
	followTransform_ = nullptr;
	padNo_ = static_cast<KeyConfig::JOYPAD_NO>(_playerNum + 1);
	localPos_ = Utility::VECTOR_ZERO;
}

Camera::~Camera(void)
{
}

void Camera::Init(void)
{

	ChangeMode(MODE::FIXED_POINT);

}

void Camera::Update(void)
{
}

void Camera::SetBeforeDraw(void)
{

	// クリップ距離を設定する(SetDrawScreenでリセットされる)
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		SetBeforeDrawFixedPoint();
		break;
	case Camera::MODE::FOLLOW:
		SetBeforeDrawFollow();
		break;
	case Camera::MODE::FOLLOW_ROTATION:
		SetBeforeDrawFollowRotation();
		break;
	case Camera::MODE::SELF_SHOT:
		SetBeforeDrawSelfShot();
		break;
	case Camera::MODE::FPS:
		SetBeforeDrawFPS();
		break;
	case Camera::MODE::FREE_CONTROLL:
		SetBeforeDrawFreeControll();
		break;
	case Camera::MODE::FIXED_UP:
		SetBeforeDrawFixedUp();
		break;
	case Camera::MODE::FIXED_DIAGONAL:
		SetBeforeDrawFixedDiagonal();
		break;
	}

	//カメラの設定
	CameraSetting();

	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();

}

void Camera::CameraSetting()
{
	// カメラの設定(位置と注視点による制御)
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		targetPos_,
		cameraUp_
	);
}

void Camera::Draw(void)
{
}

void Camera::SetFollow(const Transform* follow)
{
	followTransform_ = follow;
}

VECTOR Camera::GetPos(void) const
{
	return pos_;
}

VECTOR Camera::GetAngles(void) const
{
	return angles_;
}

VECTOR Camera::GetTargetPos(void) const
{
	return targetPos_;
}

Quaternion Camera::GetQuaRot(void) const
{
	return rot_;
}

Quaternion Camera::GetQuaRotOutX(void) const
{
	return rotOutX_;
}

VECTOR Camera::GetForward(void) const
{
	return VNorm(VSub(targetPos_, pos_));
}

void Camera::ChangeMode(MODE mode)
{

	// カメラの初期設定
	SetDefault();

	// カメラモードの変更
	mode_ = mode;

	// 変更時の初期化処理
	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		break;
	case Camera::MODE::FOLLOW:
		localPos_ = LOCAL_F2C_POS;
		break;
	}

}

void Camera::SetDefault(void)
{

	// カメラの初期設定
	pos_ = DEFAULT_CAMERA_POS;

	// 注視点
	targetPos_ = Utility::VECTOR_ZERO;

	// カメラの上方向
	cameraUp_ = Utility::DIR_U;

	angles_.x = Utility::Deg2RadF(30.0f);
	angles_.y = 0.0f;
	angles_.z = 0.0f;
	rot_ = Quaternion();

}

void Camera::SyncFollow(void)
{

	// 同期先の位置
	VECTOR pos = followTransform_->pos;

	// 重力の方向制御に従う
	Quaternion gRot = Quaternion::Euler(VECTOR(0.0, 0.0, 0.0));

	// 正面から設定されたY軸分、回転させる
	rotOutX_ = gRot.Mult(Quaternion::AngleAxis(angles_.y, Utility::AXIS_Y));

	// 正面から設定されたX軸分、回転させる
	rot_ = rotOutX_.Mult(Quaternion::AngleAxis(angles_.x, Utility::AXIS_X));

	VECTOR localPos;

	// 注視点(通常重力でいうところのY値を追従対象と同じにする)
	localPos = rotOutX_.PosAxis(LOCAL_F2T_POS);
	targetPos_ = VAdd(pos, localPos);

	// カメラ位置
	localPos = rot_.PosAxis(localPos_);
	pos_ = VAdd(pos, localPos);

	// カメラの上方向
	cameraUp_ = gRot.GetUp();

}

void Camera::SyncFollowFPS(void)
{
	auto gIns = GravityManager::GetInstance();

	// 同期先の位置
	VECTOR pos = followTransform_->pos;

	// 重力の方向制御に従う
	//Quaternion gRot = gIns.GetTransform().quaRot;

	// 正面から設定されたY軸分、回転させる
	//rotOutX_ = gRot.Mult(Quaternion::AngleAxis(angles_.y, Utility::AXIS_Y));

	// 正面から設定されたX軸分、回転させる
	rot_ = rotOutX_.Mult(Quaternion::AngleAxis(angles_.x, Utility::AXIS_X));

	VECTOR localPos;

	// 注視点(通常重力でいうところのY値を追従対象と同じにする)
	//localPos = rotOutX_.PosAxis(LOCAL_F2T_POS);
	localPos = rot_.PosAxis(FPS_LOCAL_F2T_POS);
	targetPos_ = VAdd(pos, localPos);

	// カメラ位置
	localPos = rotOutX_.PosAxis(FPS_LOCAL_F2C_POS);
	pos_ = VAdd(pos, localPos);

	// カメラの上方向
	cameraUp_ = rot_.GetUp();
}

void Camera::ProcessRot(void)
{
	auto& ins = KeyConfig::GetInstance();
	float rotPow = Utility::Deg2RadF(SPEED);
	auto playerMaxNum = DateBank::GetInstance().GetPlayerNum();
	auto keyType = playerMaxNum == 1 ? KeyConfig::TYPE::ALL : KeyConfig::TYPE::PAD;
	if (ins.IsNew(KeyConfig::CONTROL_TYPE::PLAY_CAMERA_MOVE_RIGHT, padNo_, keyType)) { angles_.y += rotPow; }
	if (ins.IsNew(KeyConfig::CONTROL_TYPE::PLAY_CAMERA_MOVE_LEFT, padNo_, keyType)) { angles_.y -= rotPow; }
	if (ins.IsNew(KeyConfig::CONTROL_TYPE::PLAY_CAMERA_MOVE_UP, padNo_, keyType)) { angles_.x += rotPow; }
	if (ins.IsNew(KeyConfig::CONTROL_TYPE::PLAY_CAMERA_MOVE_DOWN, padNo_, keyType)) { angles_.x -= rotPow; }


	auto rStick = ins.GetKnockRStickSize(padNo_);
	rotPow = SPEED_PAD;
	angles_.x += Utility::Deg2RadF(rStick.y * rotPow);
	angles_.y += Utility::Deg2RadF(rStick.x * rotPow);
	if (keyType == KeyConfig::TYPE::ALL)
	{
		auto mouseMove = ins.GetMouseMove();
		rotPow = SPEED_MOUSE;
		angles_.x += Utility::Deg2RadF(mouseMove.y * rotPow);
		angles_.y += Utility::Deg2RadF(mouseMove.x * rotPow);
	}
	KeyConfig::GetInstance().SetMousePos({ Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y});

	if (angles_.x >= LIMIT_X_UP_RAD)
	{
		angles_.x = LIMIT_X_UP_RAD;
	}
	else if (angles_.x <= LIMIT_X_DW_RAD)
	{
		angles_.x = LIMIT_X_DW_RAD;
	}
}

void Camera::ProcessZoom(void)
{
	auto& ins = KeyConfig::GetInstance();
	auto playerMaxNum = DateBank::GetInstance().GetPlayerNum();
	auto keyType = playerMaxNum == 1 ? KeyConfig::TYPE::ALL : KeyConfig::TYPE::PAD;
	auto vec = VNorm(VSub(LOCAL_F2T_POS,LOCAL_F2C_POS));
	if (ins.IsNew(KeyConfig::CONTROL_TYPE::PLAY_CAMERA_ZOOM_IN, padNo_, keyType))
	{
		localPos_ = VAdd(localPos_, VScale(vec, ZOOM_SPEED));
	}
	if (ins.IsNew(KeyConfig::CONTROL_TYPE::PLAY_CAMERA_ZOOM_OUT, padNo_, keyType))
	{
		localPos_ = VAdd(localPos_, VScale(VScale(vec,-1), ZOOM_SPEED));
	}
	if (Utility::Distance(LOCAL_F2C_POS, localPos_) > ZOOM_RADIUS)
	{
		vec = VNorm(VSub(localPos_,LOCAL_F2C_POS));
		localPos_ = VAdd(LOCAL_F2C_POS, VScale(vec, ZOOM_RADIUS));
	}
}

void Camera::ProcessRotMause(float* x_m, float* y_m, const float fov_per)
{
	int x_t, y_t;
	GetMousePoint(&x_t, &y_t);
	*x_m += float(std::clamp(x_t - Application::SCREEN_SIZE_X / 2, -120, 120)) * fov_per / GetFPS();
	*y_m += float(std::clamp(y_t - Application::SCREEN_SIZE_Y / 2, -120, 120)) * fov_per / GetFPS();
	SetMousePoint(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2);

	// マウスを表示状態にする
	SetMouseDispFlag(FALSE);

	if (angles_.x <= FPS_LIMIT_X_UP_RAD)
	{
		angles_.x = FPS_LIMIT_X_UP_RAD;
	}
	if (angles_.x >= FPS_LIMIT_X_DW_RAD)
	{
		angles_.x = FPS_LIMIT_X_DW_RAD;
	}
}

void Camera::SetBeforeDrawFixedPoint(void)
{
	rot_ = Quaternion::Quaternion(angles_);

	// 重力の方向制御に従う
	Quaternion gRot = Quaternion::Euler(VECTOR(0.0, 0.0, 0.0));

	// 正面から設定されたY軸分、回転させる
	rotOutX_ = gRot.Mult(Quaternion::AngleAxis(angles_.y, Utility::AXIS_Y));
}


void Camera::SetBeforeDrawFollow(void)
{

	// カメラ操作
	ProcessRot();

	ProcessZoom();
	// 追従対象との相対位置を同期
	SyncFollow();
}

void Camera::SetBeforeDrawFollowRotation(void)
{
	const float SPEED_DEG = 1.5f;
	angles_.y += Utility::Deg2RadF(SPEED_DEG);

	auto vec = VNorm(VSub(LOCAL_F2T_POS, LOCAL_F2C_POS));
	localPos_ = VAdd(localPos_, VScale(vec, ZOOM_SPEED));
	if (Utility::Distance(LOCAL_F2C_POS, localPos_) > ZOOM_RADIUS)
	{
		vec = VNorm(VSub(localPos_, LOCAL_F2C_POS));
		localPos_ = VAdd(LOCAL_F2C_POS, VScale(vec, ZOOM_RADIUS));
	}

	// 追従対象との相対位置を同期
	SyncFollow();
}

void Camera::SetBeforeDrawSelfShot(void)
{
	auto gIns = GravityManager::GetInstance();

	// 同期先の位置
	VECTOR pos = followTransform_->pos;


	// 正面から設定されたX軸分、回転させる
	rot_ = rotOutX_.Mult(Quaternion::AngleAxis(angles_.x, Utility::AXIS_X));

	VECTOR localPos;

	// 注視点(通常重力でいうところのY値を追従対象と同じにする)
	localPos = rotOutX_.PosAxis(LOCAL_F2T_POS);
	targetPos_ = VAdd(pos, localPos);

	// カメラ位置
	localPos = rot_.PosAxis(LOCAL_F2C_POS);
	pos_ = VAdd(pos, localPos);


}

void Camera::SetBeforeDrawFPS(void)
{
	//マウスでのカメラ操作
	ProcessRotMause(&angles_.y, &angles_.x, 0.2f);

	// 追従対象との相対位置を同期
	SyncFollowFPS();
}

void Camera::SetBeforeDrawFreeControll(void)
{
	auto& ins = KeyConfig::GetInstance();
	float rotPow = Utility::Deg2RadF(SPEED);
	auto playerMaxNum = DateBank::GetInstance().GetPlayerNum();
	auto keyType = playerMaxNum == 1 ? KeyConfig::TYPE::ALL : KeyConfig::TYPE::PAD;
	if (ins.IsNew(KeyConfig::CONTROL_TYPE::EDIT_CAMERA_ROT_RIGHT, padNo_,keyType)) { angles_.y += rotPow; }
	if (ins.IsNew(KeyConfig::CONTROL_TYPE::EDIT_CAMERA_ROT_LEFT, padNo_,keyType)) { angles_.y -= rotPow; }
	if (ins.IsNew(KeyConfig::CONTROL_TYPE::EDIT_CAMERA_ROT_UP, padNo_,keyType)) { angles_.x -= rotPow; }
	if (ins.IsNew(KeyConfig::CONTROL_TYPE::EDIT_CAMERA_ROT_DOWN, padNo_,keyType)) { angles_.x += rotPow; }

	if (angles_.x <= FPS_LIMIT_X_UP_RAD)
	{
		angles_.x = FPS_LIMIT_X_UP_RAD;
	}
	if (angles_.x >= FPS_LIMIT_X_DW_RAD)
	{
		angles_.x = FPS_LIMIT_X_DW_RAD;
	}
	
	auto rStick = ins.GetKnockRStickSize(padNo_);
	rotPow = SPEED_PAD;
	angles_.x += Utility::Deg2RadF(rStick.y *rotPow);
	angles_.y += Utility::Deg2RadF(rStick.x *rotPow);

	//if (ins.IsNew(KEY_INPUT_W)) 
	//{
	//	pos_ = VAdd(pos_, VScale(Quaternion::Quaternion(angles_).GetForward(), 3.0f));
	//}
	//if (ins.IsNew(KEY_INPUT_S))
	//{
	//	pos_ = VAdd(pos_, VScale(Quaternion::Quaternion(angles_).GetBack(), moveSpeed));
	//}
	static float moveSpeed = 10.0f;
	static float moveSpeedFB = 30.0f;
	pos_ = VAdd(pos_, VScale(Quaternion::Quaternion(angles_).GetForward(), ins.IsNew(KeyConfig::CONTROL_TYPE::EDIT_CAMERA_MOVE_FRONT, padNo_,keyType) * moveSpeedFB));
	pos_ = VAdd(pos_, VScale(Quaternion::Quaternion(angles_).GetBack(), ins.IsNew(KeyConfig::CONTROL_TYPE::EDIT_CAMERA_MOVE_BACK, padNo_,keyType) * moveSpeedFB));
	if (ins.IsNew(KeyConfig::CONTROL_TYPE::EDIT_CAMERA_MOVE_LEFT, padNo_,keyType))
	{
		pos_ = VAdd(pos_, VScale(Quaternion::Quaternion(angles_).GetLeft(), moveSpeed));
	}
	if (ins.IsNew(KeyConfig::CONTROL_TYPE::EDIT_CAMERA_MOVE_RIGHT, padNo_,keyType))
	{
		pos_ =VAdd(pos_, VScale(Quaternion::Quaternion(angles_).GetRight(), moveSpeed));
	}
	if (ins.IsNew(KeyConfig::CONTROL_TYPE::EDIT_CAMERA_MOVE_UP, padNo_,keyType))
	{
		pos_ =VAdd(pos_, VScale(Quaternion::Quaternion(angles_).GetUp(), moveSpeed));
	}
	if (ins.IsNew(KeyConfig::CONTROL_TYPE::EDIT_CAMERA_MOVE_DOWN, padNo_,keyType))
	{
		pos_ =VAdd(pos_, VScale(Quaternion::Quaternion(angles_).GetDown(), moveSpeed));
	}

	VECTOR localPos;
	rot_ =(Quaternion::Quaternion(angles_));
	// 注視点(通常重力でいうところのY値を追従対象と同じにする)
	localPos = rot_.PosAxis(LOCAL_F2T_POS);
	targetPos_ = VAdd(pos_, localPos);


}

void Camera::SetBeforeDrawFixedUp(void)
{
	targetPos_ = VAdd(pos_, FIXED_LOCAL_P2T_POS);
	rot_ = Quaternion::Quaternion(angles_);
	cameraUp_ = rot_.GetUp();
}

void Camera::SetBeforeDrawFixedDiagonal(void)
{
	targetPos_ = FIXED_DIAGONAL_TARGET_POS;
	rot_ = Quaternion::Quaternion(angles_);
	cameraUp_ = rot_.GetUp();
}
