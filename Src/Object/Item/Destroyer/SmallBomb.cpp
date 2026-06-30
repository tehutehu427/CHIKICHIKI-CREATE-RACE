#include"../Manager/System/ResourceManager.h"
#include"../../Common/Geometry/Model.h"
#include "SmallBomb.h"

SmallBomb::SmallBomb(void)
{
}

SmallBomb::~SmallBomb(void)
{
}

void SmallBomb::SetParam(void)
{
	//モデルの基本設定
	trans_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::BOMB));

	//モデルIDのコピー
	models_.emplace_back(&trans_.modelId);

	//ステータス初期化
	size_ = MAP_SIZE;
	status_.isBreak = true;
	status_.isGravity = false;
	status_.effType = EFFECT_TYPE::DESTROYER;
	status_.rotType = ROTATION_TYPE::HALF_ROTATION;

	//サイズ倍率
	VECTOR adjustSizePer = AdjustSizePer(MODEL_SIZE);

	//サイズ
	trans_.scl.x *= adjustSizePer.x;
	trans_.scl.y *= adjustSizePer.y;
	trans_.scl.z *= adjustSizePer.z;

	//相対座標
	trans_.localPos.x = MAP_LOCALPOS.x * trans_.scl.x;
	trans_.localPos.y = MAP_LOCALPOS.y * trans_.scl.y;
	trans_.localPos.z = MAP_LOCALPOS.z * trans_.scl.z;

	//マップサイズ
	mapSize_ = MAP_SIZE;
}

void SmallBomb::Update(void)
{
}

void SmallBomb::OnHit(const std::weak_ptr<Collider> _hitCol)
{
}
