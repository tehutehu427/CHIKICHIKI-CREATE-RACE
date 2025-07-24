#include<algorithm>
#include"../Manager/System/ResourceManager.h"
#include"../Manager/Game/MapEditer.h"
#include"../Utility/Utility.h"
#include"../../Common/Geometry/Sphere.h"
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
	trans_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::COIN));

	//モデルIDのコピー
	models_.emplace_back(&trans_.modelId);

	//ステータス初期化
	size_ = MAP_SIZE;
	status_.isBreak = true;
	status_.isGravity = false;
	status_.effType = EFFECT_TYPE::INSTALLATION;

	//サイズ
	trans_.scl = VScale(trans_.scl, SIZE_MULTI);

	//相対座標
	trans_.localPos.x = MapEditer::GetInstance().MapToWorldPos(MAP_SIZE).x / 2;
	trans_.localPos.y = MapEditer::GetInstance().MapToWorldPos(MAP_SIZE).y / 2;
	trans_.localPos.z = MapEditer::GetInstance().MapToWorldPos(MAP_SIZE).z / 2;

	//コライダの作成
	std::unique_ptr<Sphere> geo = std::make_unique<Sphere>(trans_.overAllPos, RADIUS);
	MakeCollider({ Collider::TAG::COIN }, std::move(geo));

	//マップサイズ
	mapSize_ = MAP_SIZE;
}

void Coin::Update(void)
{
	//回転
	trans_.quaRot = trans_.quaRot.Mult(Quaternion::AngleAxis(Utility::Deg2RadF(ROTATE_SPEED), Utility::AXIS_Y));

	//モデル更新
	trans_.Update();

	//追従対象がいないなら
	if (followCol_.lock() == nullptr)
	{
		//コライダがないなら再生成
		if (colParam_.empty())
		{
			//コライダの作成
			std::unique_ptr<Sphere> geo = std::make_unique<Sphere>(trans_.overAllPos, RADIUS);
			MakeCollider({ Collider::TAG::COIN }, std::move(geo));
		}

		//削除
		followCol_.reset();

		//何もしない
		return;
	}

	//追従座標
	VECTOR followPos = followCol_.lock()->GetParent().GetTransform().pos;
	Quaternion followRot = followCol_.lock()->GetParent().GetTransform().quaRot;
	VECTOR followLocalPos = followRot.PosAxis(FOLLOW_LOCAL_POS);

	//対象に追従
	trans_.pos = VSub(VAdd(followPos, followLocalPos), trans_.localPos);
}

void Coin::OnHit(const std::weak_ptr<Collider> _hitCol)
{
	//すでに対象についているならスキップ
	if (followCol_.lock() != nullptr)return;

	//タグが影ならスキップ
	auto tags = _hitCol.lock()->GetTags();
	if (std::find(tags.begin(), tags.end(), Collider::TAG::SHADOW) != tags.end())return;

	//追従
	followCol_ = _hitCol;

	//コライダの消去
	colParam_[0].collider_->Kill();
	colParam_.clear();
}
