#include<algorithm>
#include"../Manager/System/ResourceManager.h"
#include"../../Common/Geometry/Model.h"
#include "Coin.h"

Coin::Coin(void)
{
}

Coin::~Coin(void)
{
}

void Coin::SetParam(void)
{
	//モデルの基本設定
	//trans_.SetModel(resMng_.LoadModelDuplicate(
	//	ResourceManager::SRC::FENCE));

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
	//std::unique_ptr<Model> geo = std::make_unique<Model>(trans_.overAllPos, trans_.quaRot, trans_.modelId);
	//MakeCollider({ Collider::TAG::NORMAL_ITEM }, std::move(geo));

	//マップサイズ
	mapSize_ = MAP_SIZE;
}

void Coin::Update(void)
{
	//追従対象がいないなら何もしない
	if (followCol_.lock() == nullptr)return;

	//追従座標
	VECTOR followPos = followCol_.lock()->GetParent().GetTransform().pos;
	Quaternion followRot = followCol_.lock()->GetParent().GetTransform().quaRot;

	//対象に追従
	trans_.pos = VAdd(followPos, followRot.PosAxis(FOLLOW_LOCAL_POS));
}

void Coin::Draw(void)
{
	DrawSphere3D(trans_.pos, 10.0f, 20, 0xffff00, 0xffff00, true);
}

void Coin::OnHit(const std::weak_ptr<Collider> _hitCol)
{
	//すでに対象についているならスキップ
	if (followCol_.lock() != nullptr)return;

	//タグが影ならスキップ
	auto tags = _hitCol.lock()->GetTags();
	if (std::find(tags.begin(), tags.end(), Collider::TAG::SHADOW) == tags.end())return;

	//追従
	followCol_ = _hitCol;
}
