#include "../Manager/System/ResourceManager.h"
#include "../Manager/System/SceneManager.h"
#include "ObjectBase.h"

ObjectBase::ObjectBase(void)
	: resMng_(ResourceManager::GetInstance()),
	scnMng_(SceneManager::GetInstance())
{
	size_ = { 0,0,0 };
}

ObjectBase::~ObjectBase(void)
{
}

void ObjectBase::ChangeModelColor(const COLOR_F _colorScale)
{
	if (MV1SetDifColorScale(trans_.modelId, _colorScale))
	{
#ifdef _DEBUG

		OutputDebugString("ChangeModelColor‚ÌŽ¸”s");

#endif // _DEBUG
	}
}

void ObjectBase::Onhit(CollisionManager::COL_TAG _tag)
{

}