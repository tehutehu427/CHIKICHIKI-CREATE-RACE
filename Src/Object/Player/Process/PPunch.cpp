#include<DxLib.h>
#include"../../../Utility/Utility.h"
#include"./PlayerInput.h"
#include "PPunch.h"

PPunch::PPunch(std::weak_ptr<PlayerInput>_input):input_(_input)
{
	isPunch_ = false;
	cnt_ = 0.0f;
	pos_ = Utility::VECTOR_ZERO;
}

void PPunch::Init(void)
{
}

void PPunch::Update(Transform& _trans)
{
	//’Ç]‘ÎÛ‚ÌˆÊ’u
	VECTOR followPos = _trans.pos;

	//’Ç]‘ÎÛ‚ÌŒü‚«
	Quaternion followRot = _trans.quaRot;

	//‘Š‘ÎÀ•W
	VECTOR localPos = { 0.0f,50.0f,40.0f };
	
	
	VECTOR addPos = followRot.PosAxis(localPos);
	pos_ = VAdd(_trans.pos, addPos);
	if (cnt_ > PUNCH_TIME_MAX)
	{
		cnt_ = 0.0f;
		isPunch_ = false;
		return;
	}
	if (input_.lock()->CheckAct(PlayerInput::ACT_CNTL::PUNCH))
	{
		isPunch_ = true;
	}
	if (isPunch_)
	{
		cnt_ += PlayerInput::DELTA_TIME;
	}
}

void PPunch::DrawDebug(void)
{
	DrawSphere3D(pos_, 10.0f, 4, 0xff0000, 0xff0000, isPunch_);
}
