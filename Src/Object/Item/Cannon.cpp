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
	//モデルの基本設定
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::CANNON_TURRET));
	size_ = MAP_SIZE;
	status_.isBreak = true;
	status_.isGravity = false;
	status_.effType = EFFECT_TYPE::INSTALLATION;

	//砲身を砲台に合わせておく
	barrelTrans_.pos = VAdd(transform_.pos,BARREL_LOCAL_POS);
	//角度もまっすぐに
	barrelTrans_.quaRotLocal = Quaternion::Euler(
		Utility::Deg2RadF(BARREL_LOCAL_ROT.x), 
		Utility::Deg2RadF(BARREL_LOCAL_ROT.y),
		Utility::Deg2RadF(BARREL_LOCAL_ROT.z));
	
	//砲身のモデル設定
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

	auto delta = SceneManager::GetInstance().GetDeltaTime();

	//対象を狙う
	Aim();

	//砲台と標的のベクトルの差異
	VECTOR turretVecDiff = VSub(targetVec_, transform_.pos);
	//正規化
	turretVecDiff = VNorm(turretVecDiff);
	//横回転なので、Yベクトルは使わない
	turretVecDiff.y = 0.0f;
	//砲台の回転量
	Quaternion turQuaRot = Quaternion::LookRotation(turretVecDiff);
	turretAddRot_ =turQuaRot.ToEuler();

	//砲台回転
	Rotate(transform_, turretAddRot_);

	//砲身と標的のベクトルの差異
	VECTOR barrelVecDiff = VSub(targetVec_, barrelTrans_.pos);
	//正規化
	barrelVecDiff = VNorm(barrelVecDiff);
	//縦回転なので、XZベクトルは使わない
	barrelVecDiff.x = 0.0f;
	barrelVecDiff.z = 0.0f;
	//砲身の回転量
	Quaternion barQuaRot = Quaternion::LookRotation(barrelVecDiff);
	//距離で割る
	float distance = Utility::Distance(targetVec_, barrelTrans_.pos);
	barQuaRot = barQuaRot.AngleAxis(distance, Utility::AXIS_X);

	barrelAddRot_ = barQuaRot.ToEuler();

	//砲身回転
	Rotate(barrelTrans_, VAdd(barrelAddRot_ ,turretAddRot_));
}

void Cannon::Draw(void)
{
	MV1DrawModel(transform_.modelId);
	MV1DrawModel(barrelTrans_.modelId);

	DrawFormatString(0, 32, 0xffffff, "%.2f,%.2f,%.2f", barrelTrans_.pos.x, barrelTrans_.pos.y, barrelTrans_.pos.z);
	DrawFormatString(0, 48, 0xffffff, "%.2f,%.2f,%.2f", turretAddRot_.x, turretAddRot_.y, turretAddRot_.z);
	DrawFormatString(0, 64, 0xffffff, "%.2f,%.2f,%.2f", barrelAddRot_.x, barrelAddRot_.y, barrelAddRot_.z);

	DrawSphere3D(targetPos_, 10.0, 20, 0xffffff, 0xffffff, true);
}

void Cannon::Aim(void)
{
	//対象への方向ベクトル取得
	targetVec_ = Utility::GetMoveVec(barrelTrans_.pos, targetPos_);
}

void Cannon::Rotate(Transform& _trans, VECTOR _addAxis, const VECTOR _relativePos)const
{
	//回転
	Quaternion rot = Quaternion::Identity();

	//回転を加える
	rot = rot.Mult(Quaternion::Euler(_addAxis));

	//反映
	_trans.pos = VSub(_trans.pos,_trans.quaRot.PosAxis(_relativePos));
	_trans.quaRot = rot;
	_trans.pos = VAdd(_trans.pos, _trans.quaRot.PosAxis(_relativePos));

	//基本情報更新
	_trans.Update();
}