#include"../Utility/Utility.h"
#include"../Manager/System/Resource.h"
#include"../Manager/System/ResourceManager.h"
#include"../../Common/Geometry/Model.h"
#include "../SubItem/Wind.h"
#include "Fan.h"

Fan::Fan()
{
}

Fan::~Fan()
{
	wind_.reset();
}

void Fan::SetParam(void)
{
	//モデルの基本設定
	trans_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::FAN));

	//モデルIDのコピー
	models_.emplace_back(&trans_.modelId);

	//ステータス初期化
	size_ = MAP_SIZE;
	status_.isBreak = true;
	status_.isGravity = false;
	status_.effType = EFFECT_TYPE::INSTALLATION;

	//前方へ回転
	trans_.quaRot.PosAxis(Utility::AXIS_Z);

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
	MakeCollider({ Collider::TAG::NORMAL_ITEM }, std::move(geo));
}

void Fan::Update(void)
{
	//風を一つ生成
	if (wind_ == nullptr)CreateWind();
}

void Fan::Draw(void)
{
	//共通
	ItemBase::Draw();

	//風
	if (wind_ != nullptr)wind_->Draw();
}

void Fan::OnHit(const std::weak_ptr<Collider> _hitCol)
{
}

void Fan::CreateWind(void)
{
	//風生成
	wind_ = std::make_unique<Wind>(trans_.pos, trans_.quaRot, trans_.scl, MODEL_SIZE);

	//初期設定
	wind_->SetParam();
}
