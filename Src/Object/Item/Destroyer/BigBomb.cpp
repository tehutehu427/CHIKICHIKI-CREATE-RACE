#include"../Manager/System/Resource.h"
#include"../Manager/System/ResourceManager.h"
#include "BigBomb.h"

BigBomb::BigBomb()
{
}

BigBomb::~BigBomb()
{
}

void BigBomb::SetParam(void)
{
	//モデルの基本設定
	//trans_.SetModel(resMng_.LoadModelDuplicate(
	//	ResourceManager::SRC::BOMB));

	//ステータス初期化
	size_ = MAP_SIZE;
	status_.isBreak = true;
	status_.isGravity = false;
	status_.effType = EFFECT_TYPE::DESTROYER;
}

void BigBomb::Update(void)
{
}
