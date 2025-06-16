#include "../Manager/System/ResourceManager.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/Game/CollisionManager.h"
#include "ObjectBase.h"

ObjectBase::ObjectBase(void)
	: resMng_(ResourceManager::GetInstance()),
	scnMng_(SceneManager::GetInstance())
{
	size_ = { 0,0,0 };
}

ObjectBase::~ObjectBase(void)
{
	//コライダの削除
	col_.collider_->Kill();
}

void ObjectBase::ChangeModelColor(const COLOR_F _colorScale)
{
	if (MV1SetDifColorScale(trans_.modelId, _colorScale))
	{
#ifdef _DEBUG

		OutputDebugString("ChangeModelColorの失敗");

#endif // _DEBUG
	}
}

void ObjectBase::MakeCollider(Collider::TAG _tag)
{
	//情報を使ってコライダの作成
	col_.collider_ = std::make_shared<Collider>(*this, _tag, *col_.geometry_);

	//コライダを管理マネージャーに追加
	CollisionManager::GetInstance().AddCollider(col_.collider_);
}
