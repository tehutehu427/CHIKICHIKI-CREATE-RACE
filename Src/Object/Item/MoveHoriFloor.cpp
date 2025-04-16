#include "../Utility/Utility.h"
#include "MoveHoriFloor.h"

MoveHoriFloor::MoveHoriFloor()
{
}

MoveHoriFloor::~MoveHoriFloor()
{
}

void MoveHoriFloor::Load(void)
{
}

void MoveHoriFloor::Init(void)
{
	//å¬ÅXÇÃê›íË
	SetParam();
}

void MoveHoriFloor::SetParam(void)
{
	size_ = MAP_SIZE;
	status_.isBreak = true;
	status_.isGravity = false;
	status_.itemType = ITEM_TYPE::FLOOR;
	status_.effType = EFFECT_TYPE::INSTALLATION;
}

void MoveHoriFloor::Update(void)
{
	
}

void MoveHoriFloor::Draw(void)
{
}

void MoveHoriFloor::Release(void)
{
}

void MoveHoriFloor::Move(void)
{
	VECTOR movePow = Utility::GetMoveVec(transform_.pos, VAdd(transform_.pos, VGet(size_.x * 100, size_.y * 100, size_.z * 100)));
}
