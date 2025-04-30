#include "../Utility/Utility.h"
#include "../Common/Quaternion.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/System/ResourceManager.h"
#include "SlimeFloor.h"

SlimeFloor::SlimeFloor(void)
{
}

SlimeFloor::~SlimeFloor(void)
{
}

void SlimeFloor::SetParam(void)
{
	//ÉÇÉfÉãÇÃäÓñ{ê›íË
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::SLIME_FLOOR));

	size_ = MAP_SIZE;
	status_.isBreak = true;
	status_.isGravity = false;
	status_.effType = EFFECT_TYPE::INSTALLATION;
}

void SlimeFloor::Update(void)
{
}
