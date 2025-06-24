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

PlayerInput::PlayerInput(KeyConfig::JOYPAD_NO _padNum, DateBank::TYPE _cntl):padNum_(_padNum),cntl_(_cntl)
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
	auto& ins = KeyConfig::GetInstance();
	using ATK_ACT = Player::ATK_ACT;
	actCntl_ = ACT_CNTL::NONE;
	//移動(いずれかの移動キーを押していたら)
	if (ins.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_FRONT, KeyConfig::JOYPAD_NO::PAD1,KeyConfig::TYPE::KEYBORD_MOUSE)
		|| ins.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_BACK, KeyConfig::JOYPAD_NO::PAD1, KeyConfig::TYPE::KEYBORD_MOUSE)
		|| ins.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_LEFT, KeyConfig::JOYPAD_NO::PAD1, KeyConfig::TYPE::KEYBORD_MOUSE)
		|| ins.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_RIGHT, KeyConfig::JOYPAD_NO::PAD1, KeyConfig::TYPE::KEYBORD_MOUSE))
	{
		actCntl_ = ACT_CNTL::MOVE; 
		if (ins.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_FRONT, KeyConfig::JOYPAD_NO::PAD1, KeyConfig::TYPE::KEYBORD_MOUSE))
		{
			actCntl_ = ACT_CNTL::DASHMOVE;
		}
	}

	//移動角度を決める
	if (ins.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_FRONT, KeyConfig::JOYPAD_NO::PAD1, KeyConfig::TYPE::KEYBORD_MOUSE))
	{ 
		moveDeg_ = FLONT_DEG;
		moveDir_ = Utility::DIR_F;
	}
	else if (ins.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_LEFT, KeyConfig::JOYPAD_NO::PAD1, KeyConfig::TYPE::KEYBORD_MOUSE))
	{ 
		moveDeg_ = LEFT_DEG; 
		moveDir_ = Utility::DIR_L;
	} 
	else if (ins.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_BACK, KeyConfig::JOYPAD_NO::PAD1, KeyConfig::TYPE::KEYBORD_MOUSE))
	{ 
		moveDeg_ = BACK_DEG; 
		moveDir_ = Utility::DIR_B;
	}
	else if (ins.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_RIGHT, KeyConfig::JOYPAD_NO::PAD1, KeyConfig::TYPE::KEYBORD_MOUSE))
	{
		moveDeg_ = RIGHT_DEG; 
		moveDir_ = Utility::DIR_R;
	}

	//通常攻撃
	if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::PLAYER_PUNCH, KeyConfig::JOYPAD_NO::PAD1, KeyConfig::TYPE::KEYBORD_MOUSE)) { actCntl_ = ACT_CNTL::PUNCH; }

	//ジャンプキー
	if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::PLAYER_JUMP, KeyConfig::JOYPAD_NO::PAD1, KeyConfig::TYPE::KEYBORD_MOUSE)) { actCntl_ = ACT_CNTL::JUMP; }

}

void PlayerInput::InputPad(void)
{
	auto& ins = KeyConfig::GetInstance();
	using ATK_ACT = Player::ATK_ACT;
	actCntl_ = ACT_CNTL::NONE;
	// 左スティックの横軸
	//leftStickX_ = ins.GetJPadInputState(padNum_).AKeyLX;
	//縦軸
	//leftStickY_ = ins.GetJPadInputState(padNum_).AKeyLY;
	//auto stickRad = static_cast<float>(atan2(static_cast<double>(leftStickY_), static_cast<double>(leftStickX_)));



	int LstickUpSize = ins.PadStickOverSize(padNum_, KeyConfig::JOYPAD_STICK::L_STICK_UP);
	int LstickLeftSize = ins.PadStickOverSize(padNum_, KeyConfig::JOYPAD_STICK::L_STICK_LEFT);
	int LstickDownSize = ins.PadStickOverSize(padNum_, KeyConfig::JOYPAD_STICK::L_STICK_DOWN);
	int LstickRightSize = ins.PadStickOverSize(padNum_, KeyConfig::JOYPAD_STICK::L_STICK_RIGHT);

	LStickAngleSize_ = ins.GetKnockLStickSize(padNum_);

	//スティックの倒れ値が200以上だったら
	if (LStickAngleSize_.x<=-STICK_MOVE_SIZE_MIN|| LStickAngleSize_.x >= STICK_MOVE_SIZE_MIN
		|| LStickAngleSize_.y <= -STICK_MOVE_SIZE_MIN || LStickAngleSize_.y >= STICK_MOVE_SIZE_MIN)
	{ 
		actCntl_ = ACT_CNTL::MOVE;
		if (ins.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_DASH, padNum_, KeyConfig::TYPE::PAD))
		{
			actCntl_ = ACT_CNTL::DASHMOVE;
		}
		//スティックの角度を求める
		stickDeg_ = ins.GetLStickDeg(padNum_);
	}

	//スティックの角度によって移動方向を決める
	moveDeg_ = stickDeg_;
	VECTOR stickDir = { LStickAngleSize_.x ,0.0f,-LStickAngleSize_.y };
	//moveDir_ = { leftStickX_ ,0.0f,leftStickX_ };
	moveDir_ = VNorm(stickDir);

	if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::PLAYER_PUNCH,padNum_,KeyConfig::TYPE::PAD) )
	{ 
		actCntl_ = ACT_CNTL::PUNCH; 
	}
	if(ins.IsTrgDown(KeyConfig::CONTROL_TYPE::PLAYER_JUMP, padNum_, KeyConfig::TYPE::PAD))
	{
		actCntl_ = ACT_CNTL::JUMP; 
	}
}