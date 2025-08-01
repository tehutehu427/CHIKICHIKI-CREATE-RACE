#include"../Object/Player/Player.h"
#include"../Utility/Utility.h"
#include "PlayerInput.h"

PlayerInput::PlayerInput(KeyConfig::JOYPAD_NO _padNum, KeyConfig::TYPE _cntl):padNum_(_padNum),cntl_(_cntl)
{
	actCntl_ = ACT_CNTL::NONE;
	leftStickX_ = -1;
	leftStickY_ = -1;
	stickDeg_ = -1;
	moveDeg_ = 0.0f;
	moveDir_ = Utility::VECTOR_ZERO;

	//各入力処理の代入
	inputUpdates_[KeyConfig::TYPE::ALL] = [this]() {InputAll(); };
	inputUpdates_[KeyConfig::TYPE::PAD] = [this]() {InputPad(); };
}

void PlayerInput::Init(void)
{
	actCntl_ = ACT_CNTL::NONE;
	leftStickX_ = -1;
	leftStickY_ = -1;
	stickDeg_ = -1;
	moveDeg_ = 0.0f;
	moveDir_ = Utility::VECTOR_ZERO;
}

void PlayerInput::Update(void)
{
	inputUpdates_[cntl_]();
}



void PlayerInput::InputAll(void)
{
	auto& ins = KeyConfig::GetInstance();
	using ATK_ACT = Player::ATK_ACT;
	actCntl_ = ACT_CNTL::NONE;
	//移動入力処理
	//キーボード
	KeyMove();
	//パッド
	PadMove();

	//ダッシュ
	if (IsDashInputDuringMove())
	{
		actCntl_ = ACT_CNTL::DASHMOVE;
	}
	//パンチ
	if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::PLAYER_PUNCH, padNum_))
	{
		actCntl_ = ACT_CNTL::PUNCH;
	}
	//ジャンプ
	if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::PLAYER_JUMP, padNum_))
	{
		actCntl_ = ACT_CNTL::JUMP;
	}


}

void PlayerInput::InputPad(void)
{
	auto& ins = KeyConfig::GetInstance();
	using ATK_ACT = Player::ATK_ACT;
	actCntl_ = ACT_CNTL::NONE;
	//パッドの移動入力処理
	PadMove();
	if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::PLAYER_PUNCH,padNum_,KeyConfig::TYPE::PAD))
	{ 
		actCntl_ = ACT_CNTL::PUNCH; 
	}
	if(ins.IsTrgDown(KeyConfig::CONTROL_TYPE::PLAYER_JUMP, padNum_, KeyConfig::TYPE::PAD))
	{
		actCntl_ = ACT_CNTL::JUMP; 
	}
}

void PlayerInput::KeyMove(void)
{
	auto& ins = KeyConfig::GetInstance();
	moveDeg_ = 0.0f;
	moveDir_ = Utility::VECTOR_ZERO;
	if (ins.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_FRONT, padNum_, KeyConfig::TYPE::KEYBORD_MOUSE))
	{
		actCntl_ = ACT_CNTL::MOVE;
		moveDeg_ = FLONT_DEG;
		moveDir_ = VAdd(moveDir_, Utility::DIR_F);
	}
	//左
	if (ins.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_LEFT, padNum_, KeyConfig::TYPE::KEYBORD_MOUSE))
	{
		actCntl_ = ACT_CNTL::MOVE;
		moveDeg_ = LEFT_DEG;
		moveDir_ = VAdd(moveDir_, Utility::DIR_L);
	}
	//後
	if (ins.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_BACK, padNum_, KeyConfig::TYPE::KEYBORD_MOUSE))
	{
		actCntl_ = ACT_CNTL::MOVE;
		moveDeg_ = BACK_DEG;
		moveDir_ = VAdd(moveDir_, Utility::DIR_B);
	}
	//右
	if (ins.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_RIGHT, padNum_, KeyConfig::TYPE::KEYBORD_MOUSE))
	{
		actCntl_ = ACT_CNTL::MOVE;
		moveDeg_ = RIGHT_DEG;
		moveDir_ = VAdd(moveDir_, Utility::DIR_R);
	}
	//斜め入力
	if (std::abs(moveDir_.x) > 0.0f && std::abs(moveDir_.z) > 0.0f)
	{
		if (Utility::GetSign(moveDir_.x) == 1)
		{
			Utility::GetSign(moveDir_.z)==1? moveDeg_ = FLONTRIGHT_DEG: moveDeg_ = BACKRIGHT_DEG;
		}
		else if (Utility::GetSign(moveDir_.x) == -1)
		{
			Utility::GetSign(moveDir_.z)==1 ? moveDeg_ = FLONTLEFT_DEG : moveDeg_ = BACKLEFT_DEG;
		}
	}
	//正規化
	moveDir_ = VNorm(moveDir_);
}

void PlayerInput::PadMove(void)
{
	auto& ins = KeyConfig::GetInstance();
	//スティックの倒れ値が200以上だったら
	if (IsStickMoved())
	{
		actCntl_ = ACT_CNTL::MOVE;
		if (ins.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_DASH, padNum_, KeyConfig::TYPE::PAD))
		{
			actCntl_ = ACT_CNTL::DASHMOVE;
		}
		//スティックの角度を求める
		stickDeg_ = ins.GetLStickDeg(padNum_);
		//スティックの角度によって移動方向を決める
		moveDeg_ = stickDeg_;
		VECTOR stickDir = { LStickAngleSize_.x ,0.0f,-LStickAngleSize_.y };
		moveDir_ = VNorm(stickDir);
	}
}

const bool PlayerInput::IsDashInputDuringMove(void)
{
	auto& ins = KeyConfig::GetInstance();
	return actCntl_ == ACT_CNTL::MOVE
		&& ins.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_DASH
			, KeyConfig::JOYPAD_NO::PAD1, KeyConfig::TYPE::KEYBORD_MOUSE);
}

const bool PlayerInput::IsStickMoved(void)
{
	auto& ins = KeyConfig::GetInstance();
	//スティックの倒れた大きさの取得
	 LStickAngleSize_ = ins.GetKnockLStickSize(padNum_);

	return LStickAngleSize_.x <= -STICK_MOVE_SIZE_MIN || LStickAngleSize_.x >= STICK_MOVE_SIZE_MIN
		|| LStickAngleSize_.y <= -STICK_MOVE_SIZE_MIN || LStickAngleSize_.y >= STICK_MOVE_SIZE_MIN;
}

