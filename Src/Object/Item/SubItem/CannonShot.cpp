#include "../Utility/Utility.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/System/Resource.h"
#include "../Manager/System/ResourceManager.h"
#include"../../Common/Geometry/Model.h"
#include"../../Common/Geometry/Sphere.h"
#include"../../Common/EffectController.h"
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
	//エフェクト追加
	effect_->Add(ResourceManager::GetInstance().Load(ResourceManager::SRC::BLAST_EFF).handleId_, EffectController::EFF_TYPE::BLAST);

	//弾モデル
	trans_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::CANNON_SHOT));

	//モデルIDのコピー
	models_.emplace_back(&trans_.modelId);

	//大きさ
	trans_.scl = VScale(trans_.scl,SCALE);

	//移動量
	movePow_ = VScale(trans_.quaRot.GetForward(), SPEED);

	//状態ごとの更新、描画
	update_ = &CannonShot::UpdateAlive;
	draw_ = &CannonShot::DrawAlive;

	//生存判定
	isAlive_ = true;

	//コライダの作成
	std::unique_ptr<Model> geo = std::make_unique<Model>(trans_.pos, trans_.quaRot, trans_.modelId);
	MakeCollider({ Collider::TAG::KILLER_ALL }, std::move(geo));
}

void CannonShot::Update(void)
{
	//弾の生存判定
	if (!isAlive_)return;

	//カウンタアップ
	cnt_ += SceneManager::GetInstance().GetDeltaTime();

	//更新
	(this->*update_)();

	//モデル情報更新
	trans_.Update();
}

void CannonShot::Draw(void)
{
	//更新
	(this->*draw_)();
}

void CannonShot::OnHit(const std::weak_ptr<Collider> _hitCol)
{
	//爆発
	Blast();
}

void CannonShot::UpdateAlive(void)
{
	//生存時間を超えると
	if (cnt_ >= ALIVE_TIME)
	{
		//爆発
		Blast();
	}

	//移動
	Move();
}

void CannonShot::UpdateBlast(void)
{
	//爆発時間が終わると
	if (cnt_ >= BLAST_TIME)
	{
		//死亡
		Kill();
	}
}

void CannonShot::UpdateDead(void)
{
	//何もしない
}

void CannonShot::DrawAlive(void)
{
	//モデル描画
	MV1DrawModel(trans_.modelId);
}

void CannonShot::DrawBlast(void)
{
	//colParam_[1].geometry_->Draw();
}

void CannonShot::DrawDead(void)
{
	//何もしない
}

void CannonShot::Move(void)
{
	trans_.pos = VAdd(trans_.pos, movePow_);
}

void CannonShot::Blast(void)
{
	//状態ごとの更新、描画
	update_ = &CannonShot::UpdateBlast;
	draw_ = &CannonShot::DrawBlast;

	//爆発エフェクト
	effect_->Play(EffectController::EFF_TYPE::BLAST, trans_.pos, Quaternion(), VGet(BLAST_SCALE, BLAST_SCALE, BLAST_SCALE));

	//コライダの作成
	std::unique_ptr<Sphere> geo = std::make_unique<Sphere>(trans_.pos, BLAST_COL_SCALE);
	MakeCollider({ Collider::TAG::KILLER_ALL }, std::move(geo));

	//カウンタの初期化
	cnt_ = 0.0f;
}

void CannonShot::Kill(void)
{
	//状態ごとの更新、描画
	update_ = &CannonShot::UpdateDead;
	draw_ = &CannonShot::DrawDead;

	//弾を削除
	isAlive_ = false;
}
