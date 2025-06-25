#include "../Utility/Utility.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/System/Resource.h"
#include "../Manager/System/ResourceManager.h"
#include"../../Common/Geometry/Model.h"
#include "CannonShot.h"

CannonShot::CannonShot(const VECTOR _pos, const Quaternion _quaRot, const VECTOR _scl)
{
	size_ = INT_VECTOR_ZERO;
	trans_.pos = _pos;
	trans_.quaRot = _quaRot;
	trans_.scl = _scl;
	isAlive_ = false;
	cnt_ = 0.0f;
}

CannonShot::~CannonShot()
{
}

void CannonShot::SetParam(void)
{
	//弾モデル
	trans_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::CANNON_SHOT));

	//モデルIDのコピー
	models_.emplace_back(&trans_.modelId);

	//大きさ
	trans_.scl = VScale(trans_.scl,SCALE);

	//移動量
	movePow_ = VScale(trans_.quaRot.GetForward(), SPEED);

	//生存判定
	isAlive_ = true;

	//コライダの作成
	std::unique_ptr<Model> geo = std::make_unique<Model>(trans_.pos, trans_.quaRot, trans_.modelId);
	MakeCollider({ Collider::TAG::KILLER_ITEM }, std::move(geo));
}

void CannonShot::Update(void)
{
	//弾の生存判定
	if (!isAlive_)return;

	//カウンタアップ
	cnt_ += SceneManager::GetInstance().GetDeltaTime();
	if (cnt_ >= ALIVE_TIME)
	{
		//消去処理
		Kill();

		return;
	}

	//移動
	Move();

	//モデル情報更新
	trans_.Update();
}

void CannonShot::Draw(void)
{
	//生きていないなら処理しない
	if (!isAlive_)return;

	//モデル描画
	MV1DrawModel(trans_.modelId);
}

void CannonShot::OnHit(const std::weak_ptr<Collider> _hitCol)
{
}

void CannonShot::Move(void)
{
	trans_.pos = VAdd(trans_.pos, movePow_);
}

void CannonShot::Kill(void)
{
	//弾を削除
	isAlive_ = false;
}
