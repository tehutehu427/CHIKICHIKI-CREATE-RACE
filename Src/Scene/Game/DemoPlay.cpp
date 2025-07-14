#include "../Manager/Game/CollisionManager.h"
#include "../Manager/Game/ItemManager.h"
#include "../Manager/Game/GravityManager.h"
#include "../Manager/Game/PlayerManager.h"
#include "DemoPlay.h"

DemoPlay::DemoPlay(void)
{
}

DemoPlay::~DemoPlay(void)
{
	ItemManager::GetInstance().Destroy();
	GravityManager::GetInstance().Destroy();
	PlayerManager::GetInstance().Destroy();
}

void DemoPlay::Load(void)
{
	ItemManager::CreateInstance();
	GravityManager::CreateInstance();
	PlayerManager::CreateInstance();
	PlayerManager::GetInstance().Load();
}

void DemoPlay::Init(void)
{
	//èâä˙âª
	PlayerManager::GetInstance().Init();
}

void DemoPlay::Update(void)
{
}

void DemoPlay::Draw(void)
{
}
