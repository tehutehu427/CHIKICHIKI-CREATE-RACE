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
}

void Fan::Update(void)
{
}
