#include "../Manager/System/ResourceManager.h"
#include "Floor.h"

Floor::Floor()
{
}

Floor::~Floor()
{
}

void Floor::SetParam(void)
{
	//モデルの基本設定
	trans_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::FLOOR));

	//相対座標
	trans_.localPos = MAP_LOCALPOS;

	//ステータス初期化
	size_ = MAP_SIZE;
	status_.isBreak = true;
	status_.isGravity = false;
	status_.effType = EFFECT_TYPE::INSTALLATION;
}

void Floor::Update(void)
{
}
