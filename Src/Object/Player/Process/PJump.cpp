#include"../../../Manager/InputManager.h"
#include"../../../Utility/Utility.h"
#include"../../../Manager/SceneManager.h"
#include"../../../Manager/GravityManager.h"
#include "PJump.h"

PJump::PJump(void)
{
	isJump_ = false;
	stepJump_ = 0.0f;
	jumpPow_ = Utility::VECTOR_ZERO;
	dirDown_ = Utility::VECTOR_ZERO;
	dirUp_ = Utility::VECTOR_ZERO;
	isEndLanding_ = true;
	gravity_ = std::make_shared<GravityManager>();
	gravity_->Init();
	
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
	gravity_->CalcGravity(dirDown_, jumpPow_);
}

void PJump::DrawDebug(void)
{
	DrawFormatString(0, 0, 0x000000, "jumpPow(%.2f)", jumpPow_);
}

void PJump::Jump(void)
{
	bool isHit = InputManager::GetInstance().IsNew(KEY_INPUT_E);
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
		// ジャンプの入力受付時間をヘラス
		stepJump_ += deltaTime;
		if (stepJump_ < TIME_JUMP_IN)
		{
			jumpPow_ = VScale(dirUp_, POW_JUMP);
		}
	}
	// ボタンを離したらジャンプ力に加算しない
	if (!isHit)
	{
		stepJump_ = TIME_JUMP_IN;
	}
}

void PJump::CalcGravity(void)
{


}
