#include"../Manager/System/Resource.h"
#include"../Manager/System/ResourceManager.h"
#include "Spiky.h"

Spiky::Spiky()
{
}

Spiky::~Spiky()
{
}

void Spiky::SetParam(void)
{
	//モデルの基本設定
	trans_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::SPIKE));

	//ステータス初期化
	size_ = MAP_SIZE;
	status_.isBreak = true;
	status_.isGravity = false;
	status_.effType = EFFECT_TYPE::INSTALLATION;
}

void Spiky::Update(void)
{
}
