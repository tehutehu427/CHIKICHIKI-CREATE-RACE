#include "Floor.h"

Floor::Floor()
{
}

Floor::~Floor()
{
}

void Floor::Load(void)
{
	//ÉÇÉfÉãÇÃäÓñ{ê›íË
	//transform_.SetModel(resMng_.LoadModelDuplicate(
	//	ResourceManager::SRC::FLOOR));
}

void Floor::Init(void)
{
	//å¬êlê›íË
	SetParam();
}

void Floor::SetParam(void)
{
	size_ = MAP_SIZE;
	status_.isBreak = true;
	status_.isGravity = false;
	status_.itemType = ITEM_TYPE::FLOOR;
	status_.effType = EFFECT_TYPE::INSTALLATION;
}

void Floor::Update(void)
{
}

void Floor::Draw(void)
{
}

void Floor::Release(void)
{
}
