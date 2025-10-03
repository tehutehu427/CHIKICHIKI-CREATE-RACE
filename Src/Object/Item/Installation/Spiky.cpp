#include "../Manager/System/ResourceManager.h"
#include "../../Common/Geometry/Model.h"
#include "Spiky.h"

Spiky::Spiky(void)
{
}

Spiky::~Spiky(void)
{
}

void Spiky::SetParam(void)
{
	//モデルの基本設定
	trans_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::SPIKE));

	//モデルIDのコピー
	models_.emplace_back(&trans_.modelId);

	//ステータス初期化
	size_ = MAP_SIZE;
	status_.isBreak = true;
	status_.isGravity = false;
	status_.effType = EFFECT_TYPE::INSTALLATION;
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

	//コライダの作成
	std::unique_ptr<Model> geo = std::make_unique<Model>(trans_.overAllPos, trans_.quaRot, trans_.modelId);
	MakeCollider({ Collider::TAG::KILLER_SPECIFIC }, std::move(geo), { Collider::TAG::PUNCH });

	//マップサイズ
	mapSize_ = MAP_SIZE;
}

void Spiky::Update(void)
{
}

void Spiky::OnHit(const std::weak_ptr<Collider> _hitCol)
{
}
