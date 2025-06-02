#include "UiBase.h"
#include "../../Manager/System/ResourceManager.h"

UiBase::UiBase() : res_(ResourceManager::GetInstance())
{
}

UiBase::~UiBase()
{
}
