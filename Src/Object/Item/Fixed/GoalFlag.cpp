#include"../Manager/System/Resource.h"
#include"../Manager/System/ResourceManager.h"
#include "GoalFlag.h"

GoalFlag::GoalFlag()
{
}

GoalFlag::~GoalFlag()
{
}

void GoalFlag::SetParam(void)
{
	//モデルの基本設定
	trans_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::GOAL_CUBE));

	//ステータス初期化
	size_ = MAP_SIZE;
	status_.isBreak = false;
	status_.isGravity = false;
	status_.effType = EFFECT_TYPE::FIXED;
}

void GoalFlag::Update(void)
{
}
