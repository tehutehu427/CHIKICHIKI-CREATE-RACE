#include"../Manager/System/Resource.h"
#include"../Manager/System/ResourceManager.h"
#include"../../Common/Geometry/Model.h"
#include "GoalFlag.h"

GoalFlag::GoalFlag()
{
}

GoalFlag::~GoalFlag()
{
}

void GoalFlag::SetParam(void)
{
	//モデルの基本設定
	trans_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::GOAL_CUBE));

	//モデルIDのコピー
	models_.emplace_back(&trans_.modelId);

	//ステータス初期化
	size_ = MAP_SIZE;
	status_.isBreak = false;
	status_.isGravity = false;
	status_.effType = EFFECT_TYPE::FIXED;

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

	//コピー
	colModelTrans_ = trans_;

	//当たり判定用モデル
	colModelTrans_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::COL_CUBE));

	colModelTrans_.Update();

	//コライダの作成
	std::unique_ptr<Model> geo = std::make_unique<Model>(trans_.overAllPos, trans_.quaRot, colModelTrans_.modelId);
	MakeCollider({ Collider::TAG::GOAL }, std::move(geo));

	//マップサイズ
	mapSize_ = MAP_SIZE;
}

void GoalFlag::Update(void)
{
}

void GoalFlag::OnHit(const std::weak_ptr<Collider> _hitCol)
{
}
