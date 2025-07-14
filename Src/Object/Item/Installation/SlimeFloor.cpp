#include "../Manager/System/ResourceManager.h"
#include"../../Common/Geometry/Model.h"
#include "SlimeFloor.h"

SlimeFloor::SlimeFloor(void)
{
}

SlimeFloor::~SlimeFloor(void)
{
}

void SlimeFloor::SetParam(void)
{
	//モデルの基本設定
	trans_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::SLIME_FLOOR));

	//モデルIDのコピー
	models_.emplace_back(&trans_.modelId);

	//ステータス初期化
	size_ = MAP_SIZE;
	status_.isBreak = true;
	status_.isGravity = false;
	status_.effType = EFFECT_TYPE::INSTALLATION;

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

	//コライダの作成
	std::unique_ptr<Model> geo = std::make_unique<Model>(trans_.pos, trans_.quaRot, trans_.modelId);
	MakeCollider({ Collider::TAG::SLIME_FLOOR }, std::move(geo));

	//マップサイズ
	mapSize_ = MAP_SIZE;
}

void SlimeFloor::Update(void)
{
}

void SlimeFloor::OnHit(const std::weak_ptr<Collider> _hitCol)
{
}
