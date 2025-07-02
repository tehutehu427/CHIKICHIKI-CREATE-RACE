#include "../../../Manager/System/ResourceManager.h"
#include"../../Common/Geometry/Sphere.h"
#include "Wind.h"

Wind::Wind(const VECTOR _pos, const Quaternion _quaRot, const VECTOR _scl, const VECTOR _size)
{
	trans_.pos = _pos;
	parentPos_ = _pos;
	trans_.quaRot = _quaRot;
	trans_.scl = _scl;
	size_ = _size;
	radius_ = 0.0f;
}

Wind::~Wind()
{
}

void Wind::SetParam(void)
{
	//弾モデル
	trans_.SetModel(-1);

	//モデルIDのコピー
	models_.emplace_back(&trans_.modelId);

	//半径
	radius_ = size_.x / 2.0f;

	//前方
	VECTOR forward = trans_.quaRot.GetForward();

	//移動量
	movePow_ = VScale(forward, POW);

	//座標(直径分動かす)
	trans_.pos = VAdd(trans_.pos, VScale(forward, radius_ * 2.0f));

	//コライダの作成
	std::unique_ptr<Sphere> geo = std::make_unique<Sphere>(trans_.pos,radius_);
	MakeCollider({ Collider::TAG::WIND }, std::move(geo));
}

void Wind::Update(void)
{
}

void Wind::Draw(void)
{
	colParam_[0].geometry_->Draw();
}

void Wind::OnHit(const std::weak_ptr<Collider> _hitCol)
{
}
