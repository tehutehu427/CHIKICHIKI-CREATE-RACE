#include "../Manager/System/ResourceManager.h"
#include "../Manager/Game./MapEditer.h"
#include"../../Common/Geometry/Model.h"
#include "Fence.h"

Fence::Fence()
{
}

Fence::~Fence()
{
}

void Fence::SetParam(void)
{
	//モデルの基本設定
	trans_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::FENCE));

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
	std::unique_ptr<Model> geo = std::make_unique<Model>(trans_);
	MakeCollider(Collider::TAG::NORMAL_ITEM, std::move(geo));
}

void Fence::Update(void)
{
}

void Fence::OnHit(const std::weak_ptr<Collider> _hitCol)
{
}
