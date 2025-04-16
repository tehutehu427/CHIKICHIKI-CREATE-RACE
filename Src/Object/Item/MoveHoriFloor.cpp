#include "../Utility/Utility.h"
#include "../Common/Quaternion.h"
#include "../Manager/System/SceneManager.h"
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
	//個々の設定
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
	//移動処理
	Move();
}

void MoveHoriFloor::Draw(void)
{
	DrawSphere3D(transform_.pos, 50, 10, 0xffffff, 0xffffff, true);
}

void MoveHoriFloor::Release(void)
{
}

void MoveHoriFloor::Move(void)
{
	VECTOR startPos = transform_.pos;
	VECTOR movePos = transform_.quaRot.PosAxis(VGet(size_.x * MOVE_X * 100,0.0f,0.0f));
	VECTOR goalPos = VAdd(startPos, movePos);

	//移動ベクトル
	VECTOR movePow = Utility::GetMoveVec(startPos, goalPos, SPEED);
}
