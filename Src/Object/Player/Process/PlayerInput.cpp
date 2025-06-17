#include "PlayerInput.h"
#include"../Object/Player/Player.h"
#include"../Utility/Utility.h"

void PlayerInput::Update(void)
{
	switch (cntl_)
	{
	case DateBank::TYPE::ERORE:
		break;
	case DateBank::TYPE::KEY_BORD:
		InputKeyBoard();
		break;
	case DateBank::TYPE::CONTROLLER:
		InputPad();
		break;
	default:
		break;
	}
}

PlayerInput::PlayerInput(InputManager::JOYPAD_NO _padNum, DateBank::TYPE _cntl):padNum_(_padNum),cntl_(_cntl)
{
	actCntl_ = ACT_CNTL::NONE;
	leftStickX_ = -1;
	leftStickY_ = -1;
	stickDeg_ = -1;
	moveDeg_ = 0.0f;
	moveDir_ = Utility::VECTOR_ZERO;
}

void PlayerInput::InputKeyBoard(void)
{
	auto& ins = InputManager::GetInstance();
	using ATK_ACT = Player::ATK_ACT;
	actCntl_ = ACT_CNTL::NONE;
	//移動(いずれかの移動キーを押していたら)
	if (ins.IsNew(MOVE_FRONT_KEY) || ins.IsNew(MOVE_BACK_KEY)
		|| ins.IsNew(MOVE_LEFT_KEY) || ins.IsNew(MOVE_RIGHT_KEY)) 
	{
		actCntl_ = ACT_CNTL::MOVE; 
	}

	//移動角度を決める
	if (ins.IsNew(PlayerInput::MOVE_FRONT_KEY))
	{ 
		moveDeg_ = FLONT_DEG;
		moveDir_ = Utility::DIR_F;
	}
	else if (ins.IsNew(PlayerInput::MOVE_LEFT_KEY)) 
	{ 
		moveDeg_ = LEFT_DEG; 
		moveDir_ = Utility::DIR_L;
	} 
	else if (ins.IsNew(PlayerInput::MOVE_BACK_KEY))
	{ 
		moveDeg_ = BACK_DEG; 
		moveDir_ = Utility::DIR_B;
	}
	else if (ins.IsNew(PlayerInput::MOVE_RIGHT_KEY))
	{
		moveDeg_ = RIGHT_DEG; 
		moveDir_ = Utility::DIR_R;
	}

	//通常攻撃
	if (ins.IsTrgDown(PUNCH_KEY)) { actCntl_ = ACT_CNTL::PUNCH; }

	//ジャンプキー
	if (ins.IsTrgDown(JUMP_KEY)) { actCntl_ = ACT_CNTL::JUMP; }

}

void PlayerInput::InputPad(void)
{
	auto& ins = InputManager::GetInstance();
	using ATK_ACT = Player::ATK_ACT;
	actCntl_ = ACT_CNTL::NONE;
	// 左スティックの横軸
	leftStickX_ = ins.GetJPadInputState(padNum_).AKeyLX;
	//縦軸
	leftStickY_ = ins.GetJPadInputState(padNum_).AKeyLY;
	auto stickRad = static_cast<float>(atan2(static_cast<double>(leftStickY_), static_cast<double>(leftStickX_)));

	//スティックの角度を求める
	stickDeg_ = static_cast<float>(Utility::DegIn360(Utility::Rad2DegF(stickRad) + STICK_MARGIN_DEG));

	if (leftStickX_ != 0.0f || leftStickY_ != 0.0f)
	{ 
		actCntl_ = ACT_CNTL::MOVE; 
	}

	//スティックの角度によって移動方向を決める
	moveDeg_ = stickDeg_;
	VECTOR stickDir = { leftStickX_ ,0.0f,-leftStickY_ };
	//moveDir_ = { leftStickX_ ,0.0f,leftStickX_ };
	moveDir_ = VNorm(stickDir);

	if (ins.IsPadBtnTrgDown(padNum_, PUNCH_BTN)) 
	{ 
		actCntl_ = ACT_CNTL::PUNCH; 
	}
	if(ins.IsPadBtnTrgDown(padNum_, JUMP_BTN)){
		actCntl_ = ACT_CNTL::JUMP; 
	}
}