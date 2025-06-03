#include"../Utility/Utility.h"
#include"../Manager/System/Resource.h"
#include"../Manager/System/ResourceManager.h"
#include "Fan.h"

Fan::Fan()
{
}

Fan::~Fan()
{
}

void Fan::SetParam(void)
{
	//モデルの基本設定
	trans_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::FAN));

	//ステータス初期化
	size_ = MAP_SIZE;
	status_.isBreak = true;
	status_.isGravity = false;
	status_.effType = EFFECT_TYPE::INSTALLATION;

	//サイズ倍率
	VECTOR adjustSizePer = AdjustSizePer(MODEL_SIZE);

	//サイズ
	trans_.scl.x *= adjustSizePer.x;
	trans_.scl.y *= adjustSizePer.y;
	trans_.scl.z *= adjustSizePer.z;

	//相対座標
	trans_.localPos.x = MAP_LOCALPOS.x * trans_.scl.x;
	trans_.localPos.y = MAP_LOCALPOS.y * trans_.scl.y;
	trans_.localPos.z = MAP_LOCALPOS.z * trans_.scl.z;
}

void Fan::Update(void)
{
}

void Fan::Draw(void)
{
	//共通
	ItemBase::Draw();

	VECTOR fanCenterPos = VAdd(trans_.pos, trans_.localPos);

	//風
	DrawCube3D(VAdd(fanCenterPos, { MODEL_SIZE.x / 2.0f,-MODEL_SIZE.y/2.0f,30.0f }),
		VAdd(fanCenterPos, { -MODEL_SIZE.x / 2.0f,MODEL_SIZE.y / 2.0f,400.0f }),
		Utility::WHITE, Utility::WHITE, false);
}
