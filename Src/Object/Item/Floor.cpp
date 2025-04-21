#include "../../Manager/System/ResourceManager.h"
#include "Floor.h"

Floor::Floor()
{
}

Floor::~Floor()
{
}

void Floor::SetParam(void)
{
	//ÉÇÉfÉãÇÃäÓñ{ê›íË
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::FLOOR));

	size_ = MAP_SIZE;
	status_.isBreak = true;
	status_.isGravity = false;
	status_.effType = EFFECT_TYPE::INSTALLATION;
}

void Floor::Update(void)
{
}

void Floor::Release(void)
{
}
