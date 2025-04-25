#include"../../../Manager/System/InputManager.h"
#include"../../../Utility/Utility.h"
#include"../../../Manager/System/SceneManager.h"
#include"../../../Manager/Game/GravityManager.h"
#include"../../../FpsControl/FpsControl.h"
#include"./PlayerInput.h"
#include "PJump.h"

PJump::PJump(std::weak_ptr<PlayerInput>_input):input_(_input)
{
	//isJump_ = false;
	//stepJump_ = 0.0f;
	//jumpPow_ = Utility::VECTOR_ZERO;
	//dirDown_ = Utility::VECTOR_ZERO;
	//dirUp_ = Utility::VECTOR_ZERO;
	//isEndLanding_ = true;
	//jumpDeceralation_ = POW_JUMP;
	
}

void PJump::Init(void)
{
	
}

void PJump::Update(const VECTOR& _dirUp,const VECTOR& _dirDown,const bool& _isEndLanding)
{
	//dirUp_ = _dirUp;
	//dirDown_ = _dirDown;
	//isEndLanding_ = _isEndLanding;
	//Jump();
	//GravityManager::GetInstance()->CalcGravity(dirDown_, jumpPow_);
}

void PJump::DrawDebug(void)
{
	//DrawFormatString(0, 0, 0x000000, "jumpPow(%.2f)", jumpPow_);
}

void PJump::Jump(void)
{

}

void PJump::CalcGravity(void)
{


}
