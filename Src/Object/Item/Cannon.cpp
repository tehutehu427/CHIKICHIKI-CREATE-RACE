#include "../../Manager/System/ResourceManager.h"
#include "../../Utility/Utility.h"
#include "Cannon.h"

Cannon::Cannon()
{
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

	//位置補間
	transform_.pos = VSub(transform_.pos, transform_.quaRot.PosAxis({ size_.x * 100.0f / 2.0f, size_.y * 100.0f / 2.0f, size_.z * 100.0f / 2.0f }));

	//砲身を砲台に合わせておく
	barrelTrans_ = transform_;
	
	//砲身のモデル設定
	barrelTrans_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::CANNON_BARREL));

	//砲身の初期更新
	barrelTrans_.Update();
}

void Cannon::Update(void)
{
	barrelTrans_.quaRot = barrelTrans_.quaRot.Mult(barrelTrans_.quaRot.AngleAxis(Utility::Deg2RadF(30.0f), Utility::AXIS_X));

	//大砲の初期更新
	transform_.Update();
	barrelTrans_.Update();
}

void Cannon::Draw(void)
{
	MV1DrawModel(transform_.modelId);
	MV1DrawModel(barrelTrans_.modelId);
}

void Cannon::Release(void)
{
}
