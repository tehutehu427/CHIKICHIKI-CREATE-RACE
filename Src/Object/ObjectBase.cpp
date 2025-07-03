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
	for (auto& colParam : colParam_)
	{
		//所持している全コライダの削除
		colParam.collider_->Kill();
	}
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

void ObjectBase::MakeCollider(const std::vector<Collider::TAG> _tag, std::unique_ptr<Geometry> _geometry)
{
	//当たり判定情報
	ColParam colParam;

	//形状情報の挿入
	colParam.geometry_ = std::move(_geometry);

	//情報を使ってコライダの作成
	colParam.collider_ = std::make_shared<Collider>(*this, _tag, *colParam.geometry_);

	//コライダを管理マネージャーに追加
	CollisionManager::GetInstance().AddCollider(colParam.collider_);

	//配列にセット
	colParam_.push_back(std::move(colParam));
}

void ObjectBase::DeleteCollider(const int _arrayNum)
{
	//配列番号-1
	int arrayNum = _arrayNum - 1;

	//コライダの削除
	colParam_[_arrayNum].collider_->Kill();

	//配列の削除
	colParam_.erase(colParam_.begin() + arrayNum);
}