#include"../../../Manager/System/InputManager.h"
#include"../../../Utility/Utility.h"
#include"../../../Manager/System/SceneManager.h"
#include"../../../Manager/Game/GravityManager.h"
#include"../../../FpsControl/FpsControl.h"
#include"./PlayerInput.h"
#include "PJump.h"

PJump::PJump(void)
{
	isJump_ = false;
	stepJump_ = 0.0f;
	jumpPow_ = Utility::VECTOR_ZERO;
	dirDown_ = Utility::VECTOR_ZERO;
	dirUp_ = Utility::VECTOR_ZERO;
	isEndLanding_ = true;
	jumpDeceralation_ = POW_JUMP;
	
}

void PJump::Init(void)
{
	
}

void PJump::Update(const VECTOR& _dirUp,const VECTOR& _dirDown,const bool& _isEndLanding)
{
	dirUp_ = _dirUp;
	dirDown_ = _dirDown;
	isEndLanding_ = _isEndLanding;
	Jump();
	GravityManager::GetInstance()->CalcGravity(dirDown_, jumpPow_);
}

void PJump::DrawDebug(void)
{
	DrawFormatString(0, 0, 0x000000, "jumpPow(%.2f)", jumpPow_);
}

void PJump::Jump(void)
{
	bool isHit = PlayerInput::GetInstance().CheckAct(PlayerInput::ACT_CNTL::JUMP);
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();
	// ジャンプ
	if (isHit && (isJump_ || isEndLanding_))
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
		stepJump_ += DELTA_TIME;
		if (stepJump_ < TIME_JUMP_IN)
		{
			jumpDeceralation_ -= stepJump_ * TIME_JUMP_IN;
			jumpPow_ = VScale(dirUp_, jumpDeceralation_);
		}
		else
		{
			jumpDeceralation_ += (TIME_JUMP_IN - stepJump_) * TIME_JUMP_IN;
			jumpPow_ = VScale(dirUp_, jumpDeceralation_);
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

void PJump::CalcGravity(void)
{


}
