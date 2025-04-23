#include "../../Manager/System/ResourceManager.h"
#include "../../Manager/System/SceneManager.h"
#include "../../Utility/Utility.h"
#include "Cannon.h"

Cannon::Cannon()
{
	targetPos_ = Utility::VECTOR_ZERO;
	turretAddRot_ = Utility::VECTOR_ZERO;
	barrelAddRot_ = Utility::VECTOR_ZERO;
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
	auto delta = SceneManager::GetInstance().GetDeltaTime();

	//–C‘ä‚Ì‰ñ“]—Ê
	//turretAddRot_.y += 1.0f * delta;
	//–C‘ä‰ñ“]
	Rotate(transform_, turretAddRot_);

	//–Cg‚Ì‰ñ“]—Ê
	barrelAddRot_.x += 1.0f * delta;
	//–Cg‰ñ“]
	Rotate(barrelTrans_, VAdd(barrelAddRot_ ,turretAddRot_));
}

void Cannon::Draw(void)
{
	MV1DrawModel(transform_.modelId);
	MV1DrawModel(barrelTrans_.modelId);

	DrawFormatString(0, 32, 0xffffff, "%.2f,%.2f,%.2f", barrelTrans_.pos.x, barrelTrans_.pos.y, barrelTrans_.pos.z);
}

void Cannon::Release(void)
{
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