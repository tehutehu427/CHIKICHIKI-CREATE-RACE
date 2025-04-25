#include<DxLib.h>
#include"../../../Utility/Utility.h"
#include"./PlayerInput.h"
#include "PPunch.h"

PPunch::PPunch(std::weak_ptr<PlayerInput>_input):input_(_input)
{
	//isPunch_ = false;
	//cnt_ = 0.0f;
	//pos_ = Utility::VECTOR_ZERO;
}

void PPunch::Init(void)
{
}

void PPunch::Update(Transform& _trans)
{
	
	

}

void PPunch::DrawDebug(void)
{
	
}
