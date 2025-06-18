#include "../Manager/System/ResourceManager.h"
#include "../Manager/System/InputManager.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/Game/ItemManager.h"
#include "../Utility/Utility.h"
#include "../FpsControl/FpsControl.h"
#include"../../Common/Geometry/Model.h"
#include"../../Common/Geometry/Sphere.h"
#include "CannonShot.h"
#include "Cannon.h"

Cannon::Cannon()
{
	shotNum_ = 0;
	shotCreateCnt_ = 0.0f;
	targetPos_ = { 0.0f,0.0f,0.0f };
	turretAddRot_ = Utility::VECTOR_ZERO;
	barrelAddRot_ = Utility::VECTOR_ZERO;
	targetVec_ = Utility::VECTOR_ZERO;
}

Cannon::~Cannon()
{
}

void Cannon::SetParam(void)
{
	//モデルの基本設定
	trans_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::CANNON_TURRET));

	//モデルIDのコピー
	models_.emplace_back(&trans_.modelId);

	//ステータス初期化
	size_ = MAP_SIZE;
	status_.isBreak = true;
	status_.isGravity = false;
	status_.effType = EFFECT_TYPE::INSTALLATION;

	//相対座標
	trans_.localPos = MAP_LOCALPOS;

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

	//砲台のコライダの作成
	std::unique_ptr<Model> geo = std::make_unique<Model>(trans_.pos, trans_.quaRot, trans_.modelId);
	MakeCollider(Collider::TAG::NORMAL_ITEM, std::move(geo));
	
	//砲身
	barrelTrans_ = trans_;

	//砲台からの相対座標
	VECTOR barrelLocalPos = BARREL_LOCAL_POS;
	barrelLocalPos.x *= barrelTrans_.scl.x;
	barrelLocalPos.y *= barrelTrans_.scl.y;
	barrelLocalPos.z *= barrelTrans_.scl.z;

	//砲台からの相対回転
	VECTOR barrelLocalRot = BARREL_LOCAL_ROT;
	barrelLocalRot.x *= barrelTrans_.scl.x;
	barrelLocalRot.y *= barrelTrans_.scl.y;
	barrelLocalRot.z *= barrelTrans_.scl.z;

	//砲身を砲台に合わせておく
	barrelTrans_.pos = VAdd(trans_.pos, barrelLocalPos);
	//角度もまっすぐに
	barrelTrans_.quaRotLocal = Quaternion::Euler(
		Utility::Deg2RadF(barrelLocalRot.x),
		Utility::Deg2RadF(barrelLocalRot.y),
		Utility::Deg2RadF(barrelLocalRot.z));
	
	//砲身のモデル設定
	barrelTrans_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::CANNON_BARREL));

	//モデルIDのコピー
	models_.emplace_back(&barrelTrans_.modelId);

	//砲身のコライダの作成
	geo = std::make_unique<Model>(barrelTrans_.pos, barrelTrans_.quaRot, barrelTrans_.modelId);
	MakeCollider(Collider::TAG::NORMAL_ITEM, std::move(geo));

	//大砲のエイム範囲
	std::unique_ptr<Sphere>aimGeo = std::make_unique<Sphere>(trans_.pos, AIM_RADIUS);
	MakeCollider(Collider::TAG::CANNON_AIM, std::move(aimGeo));
}

void Cannon::Update(void)
{	
	//弾の削除処理
	DeleteShot();

	//射程内なら
	if (IsWithinRange())
	{
		//弾の生成
		CreateShot();

		//砲台の回転
		RotateTurret();

		//砲身の回転
		RotateBarrel();
	}

	//弾の更新
	for (auto& shot : shots_)
	{
		//弾がないならスキップ
		if (shot == nullptr)continue;

		//弾の更新
		shot->Update();
	}
}

void Cannon::Draw(void)
{
	//砲台の描画
	MV1DrawModel(trans_.modelId);
	//砲身の描画
	MV1DrawModel(barrelTrans_.modelId);

	//DrawFormatString(0, 32, 0xffffff, "%.2f,%.2f,%.2f", barrelTrans_.pos.x, barrelTrans_.pos.y, barrelTrans_.pos.z);
	//DrawFormatString(0, 48, 0xffffff, "%.2f,%.2f,%.2f", turretAddRot_.x, turretAddRot_.y, turretAddRot_.z);
	//DrawFormatString(0, 64, 0xffffff, "%.2f,%.2f,%.2f", barrelAddRot_.x, barrelAddRot_.y, barrelAddRot_.z);

	DrawSphere3D(targetPos_, 10.0, 20, 0xffffff, 0xffffff, true);

	//DrawLine3D(VAdd(barrelTrans_.pos,barrelTrans_.localPos), targetPos_, 0x666666);
	//DrawLine3D(VAdd(barrelTrans_.pos, barrelTrans_.localPos), VScale(barrelTrans_.quaRot.Mult(barrelTrans_.quaRotLocal).GetForward(),1000.0f), 0x666666);

	//DrawSphere3D(trans_.pos, AIM_RADIUS, 5, 0xffffff, 0xffffff, false);

	colParam_[2].geometry_->Draw();

	//弾関係
	for (auto& shot : shots_)
	{
		//弾がないならスキップ
		if (shot == nullptr)continue;

		//弾の描画
		shot->Draw();
	}
}

void Cannon::OnHit(const std::weak_ptr<Collider> _hitCol)
{
	switch (_hitCol.lock()->GetTag())
	{
	case Collider::TAG::PLAYER1:
	case Collider::TAG::PLAYER2:
	case Collider::TAG::PLAYER3:
	case Collider::TAG::PLAYER4:
		//当たったのがエイム範囲ならプレイヤーを狙う
		if (colParam_[AIM_COL_NUM].collider_->IsHit())
			targetPos_ = _hitCol.lock()->GetParent().GetTransform().pos;
		break;

	default:
		break;
	}
}

void Cannon::ChangeModelColor(const COLOR_F _colorScale)
{
	//砲台
	if (MV1SetDifColorScale(trans_.modelId, _colorScale))
	{
#ifdef _DEBUG

		OutputDebugString("ChangeModelColorの失敗");

#endif // _DEBUG
	}

	//砲身
	if (MV1SetDifColorScale(barrelTrans_.modelId, _colorScale))
	{
#ifdef _DEBUG

		OutputDebugString("ChangeModelColorの失敗");

#endif // _DEBUG
	}

}

void Cannon::RotateTurret(void)
{
	//対象までの回転軸
	turretAddRot_ = Utility::GetRotAxisToTarget(VAdd(trans_.pos,trans_.localPos), targetPos_,Utility::AXIS_XZ);

	//砲台回転
	Utility::LookAtTarget(trans_, turretAddRot_, AIM_TIME_TURRET);
}

void Cannon::RotateBarrel(void)
{
	//対象までの回転軸
	barrelAddRot_ = Utility::GetRotAxisToTarget(VAdd(barrelTrans_.pos,barrelTrans_.localPos), targetPos_, Utility::AXIS_Y);

	//距離で補正
	double distance = Utility::Distance(Utility::GetMoveVec(barrelTrans_.pos, targetPos_), barrelTrans_.pos);
	barrelAddRot_ = Quaternion::Euler(barrelAddRot_).AngleAxis(distance, Utility::AXIS_X).ToEuler();

	//砲身回転
	Utility::LookAtTarget(barrelTrans_, VAdd(barrelAddRot_, turretAddRot_), AIM_TIME_BARREL);
}

void Cannon::CreateShot(void)
{
	//デルタタイム取得
	float delta = SceneManager::GetInstance().GetDeltaTime();

	//生成間隔カウント
	shotCreateCnt_ += delta;

	//弾が最大数生成されている　又は　生成間隔を達していないなら生成処理をしない
	if (shotNum_ >= SHOT_MAX || shotCreateCnt_ < SHOT_INTERVAL)return;

	//アイテムマネージャー
	ItemManager& itemMng = ItemManager::GetInstance();

	//砲身の全回転
	Quaternion barrelAllRot = barrelTrans_.quaRot.Mult(barrelTrans_.quaRotLocal);

	//弾の生成
	std::unique_ptr<CannonShot> shot = std::make_unique<CannonShot>(VAdd(barrelTrans_.pos,barrelTrans_.localPos), barrelAllRot,barrelTrans_.scl);
	//std::shared_ptr<CannonShot> shot = std::make_shared<CannonShot>(barrelTrans_.pos, barrelAllRot, this);

	//弾の初期化
	shot->Init();

	//配列にセット
	shots_[shotNum_] = std::move(shot);

	//生成数カウント増加
	shotNum_++;

	//マネージャーに所有権を渡す
	//itemMng.MoveSubItemOwner(ITEM_TYPE::CANNON_SHOT, std::move(shot));

	//生成間隔カウンタ初期化
	shotCreateCnt_ = 0;
}

void Cannon::DeleteShot(void)
{
	for (int i = 0 ; i < SHOT_MAX ; i++)
	{
		//弾がない 又は 弾が生きてるならならスキップ
		if (shots_[i] == nullptr || shots_[i]->IsAlive())continue;

		//弾消去
		shots_[i].reset();

		//配列が空いたので一番後ろの弾を代入
		shots_[i] = std::move(shots_[shotNum_ - 1]);

		//弾カウント減少
		shotNum_--;
	}
}

bool Cannon::IsWithinRange(void)
{
	return Utility::MagnitudeF(targetPos_) <= AIM_RADIUS;
}
