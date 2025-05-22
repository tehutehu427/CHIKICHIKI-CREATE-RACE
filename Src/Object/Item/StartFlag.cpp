#include"../Manager/System/Resource.h"
#include"../Manager/System/ResourceManager.h"
#include "StartFlag.h"

StartFlag::StartFlag()
{
}

StartFlag::~StartFlag()
{
}

void StartFlag::SetParam(void)
{
	//モデルの基本設定
	trans_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::START_CUBE));

	//ステータス初期化
	size_ = MAP_SIZE;
	status_.isBreak = false;
	status_.isGravity = false;
	status_.effType = EFFECT_TYPE::FIXED;
}

void StartFlag::Update(void)
{
}
