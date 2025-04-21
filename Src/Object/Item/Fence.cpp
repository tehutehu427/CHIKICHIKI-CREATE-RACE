#include "../../Manager/System/ResourceManager.h"
#include "Fence.h"

Fence::Fence()
{
}

Fence::~Fence()
{
}

void Fence::SetParam(void)
{
	//ÉÇÉfÉãÇÃäÓñ{ê›íË
	//transform_.SetModel(resMng_.LoadModelDuplicate(
	//	ResourceManager::SRC::FLOOR));

	size_ = MAP_SIZE;
	status_.isBreak = true;
	status_.isGravity = false;
	status_.effType = EFFECT_TYPE::INSTALLATION;
}

void Fence::Update(void)
{
}

void Fence::Draw(void)
{
	DrawCube3D({ -100,-50,-50 }, { 100,50,50 }, 0xffffff, 0xffffff, true);
}

void Fence::Release(void)
{
}
