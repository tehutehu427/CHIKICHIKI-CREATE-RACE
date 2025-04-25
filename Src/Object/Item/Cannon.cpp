#include "../../Manager/System/ResourceManager.h"
#include "../../Manager/System/InputManager.h"
#include "../../Manager/System/SceneManager.h"
#include "../../Utility/Utility.h"
#include "../../FpsControl/FpsControl.h"
#include "Cannon.h"

Cannon::Cannon()
{
	targetPos_ = { 0.0f,0.0f,0.0f };
	turretAddRot_ = Utility::VECTOR_ZERO;
	barrelAddRot_ = Utility::VECTOR_ZERO;
	targetVec_ = Utility::VECTOR_ZERO;
}

Cannon::~Cannon()
{
}

void Cannon::SetParam(void)
{
	//ƒ‚ƒfƒ‹‚ÌŠî–{İ’è
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::CANNON_TURRET));
	size_ = MAP_SIZE;
	status_.isBreak = true;
	status_.isGravity = false;
	status_.effType = EFFECT_TYPE::INSTALLATION;

	//–Cg‚ğ–C‘ä‚É‡‚í‚¹‚Ä‚¨‚­
	barrelTrans_.pos = VAdd(transform_.pos,BARREL_LOCAL_POS);
	
	//–Cg‚Ìƒ‚ƒfƒ‹İ’è
	barrelTrans_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::CANNON_BARREL));
}

void Cannon::Update(void)
{
	auto& ins = InputManager::GetInstance();
	if (ins.IsNew(KEY_INPUT_UP))targetPos_.z++;
	if (ins.IsNew(KEY_INPUT_RIGHT))targetPos_.x++;
	if (ins.IsNew(KEY_INPUT_DOWN))targetPos_.z--;
	if (ins.IsNew(KEY_INPUT_LEFT))targetPos_.x--;
	if (ins.IsNew(KEY_INPUT_RSHIFT))targetPos_.y++;
	if (ins.IsNew(KEY_INPUT_RCONTROL))targetPos_.y--;

	auto delta = FRAME_RATE;

	//‘ÎÛ‚ğ‘_‚¤
	Aim();

	//–C‘ä‚Ì‰ñ“]—Ê
	VECTOR turretAddRot = VSub(targetVec_, turretAddRot_);
	turretAddRot.x = 0.0f;
	turretAddRot.z = 0.0f;
	turretAddRot = VNorm(turretAddRot);

	turretAddRot_ = VScale(turretAddRot, delta);

	//–C‘ä‰ñ“]
	Rotate(transform_, turretAddRot_);

	//–Cg‚Ì‰ñ“]—Ê
	VECTOR barrelAddRot = VSub(targetVec_, barrelAddRot_);
	barrelAddRot.y = 0.0f;
	barrelAddRot.z = 0.0f;
	barrelAddRot = VNorm(barrelAddRot);

	barrelAddRot_.x += barrelAddRot.x * delta;
	//–Cg‰ñ“]
	Rotate(barrelTrans_, VAdd(barrelAddRot_ ,turretAddRot_));
}

void Cannon::Draw(void)
{
	MV1DrawModel(transform_.modelId);
	MV1DrawModel(barrelTrans_.modelId);

	DrawFormatString(0, 32, 0xffffff, "%.2f,%.2f,%.2f", barrelTrans_.pos.x, barrelTrans_.pos.y, barrelTrans_.pos.z);
	DrawFormatString(0, 348, 0xffffff, "%.2f,%.2f,%.2f", turretAddRot_.x, turretAddRot_.y, turretAddRot_.z);

	DrawSphere3D(targetPos_, 10.0, 20, 0xffffff, 0xffffff, true);
}

void Cannon::Aim(void)
{
	//‘ÎÛ‚Ö‚Ì•ûŒüƒxƒNƒgƒ‹æ“¾
	targetVec_ = Utility::GetMoveVec(barrelTrans_.pos, targetPos_);
}

void Cannon::Rotate(Transform& _trans, VECTOR _addAxis, const VECTOR _relativePos)const
{
	//‰ñ“]
	Quaternion rot = Quaternion::Identity();

	//‰ñ“]‚ğ‰Á‚¦‚é
	rot = rot.Mult(Quaternion::Euler(_addAxis));

	//”½‰f
	_trans.pos = VSub(_trans.pos,_trans.quaRot.PosAxis(_relativePos));
	_trans.quaRot = rot;
	_trans.pos = VAdd(_trans.pos, _trans.quaRot.PosAxis(_relativePos));

	//Šî–{î•ñXV
	_trans.Update();
}