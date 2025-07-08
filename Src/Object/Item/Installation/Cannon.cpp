#include "../Manager/Game/CollisionManager.h"
#include "../Manager/System/ResourceManager.h"
#include "../Manager/System/InputManager.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/System/DateBank.h"
#include "../Utility/Utility.h"
#include "../FpsControl/FpsControl.h"
#include"../../Common/Geometry/Model.h"
#include"../../Common/Geometry/Sphere.h"
#include"../../Common/ToonStyle.h"
#include "../SubItem/CannonShot.h"
#include "Cannon.h"

Cannon::Cannon()
{
	shotCreateCnt_ = 0.0f;
	targetPos_ = { 0.0f,0.0f,0.0f };
	turretAddRot_ = Utility::VECTOR_ZERO;
	barrelAddRot_ = Utility::VECTOR_ZERO;
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
	MakeCollider({ Collider::TAG::NORMAL_ITEM }, std::move(geo));
	
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

	//砲身の初期更新
	barrelTrans_.Update();

	//モデルIDのコピー
	models_.emplace_back(&barrelTrans_.modelId);

	//砲身のコライダの作成
	geo = std::make_unique<Model>(barrelTrans_.pos, barrelTrans_.quaRot, barrelTrans_.modelId);
	MakeCollider({ Collider::TAG::NORMAL_ITEM }, std::move(geo));

	//大砲のエイム範囲
	std::unique_ptr<Sphere>aimGeo = std::make_unique<Sphere>(trans_.pos, AIM_RADIUS);
	MakeCollider({ Collider::TAG::CANNON_AIM }, std::move(aimGeo));

	//マップサイズ
	mapSize_ = MAP_SIZE;
	
}

void Cannon::Update(void)
{	
	//デルタタイム取得
	float delta = SceneManager::GetInstance().GetDeltaTime();

	//生成間隔カウント
	shotCreateCnt_ += delta;

	//砲台の回転
	RotateTurret();

	//砲身の回転
	RotateBarrel();

	if (shot_ != nullptr)
	{
		//弾の更新
		shot_->Update();

		//弾の削除処理
		DeleteShot();
	}
}

void Cannon::Draw(void)
{	
	//弾の描画	
	if (shot_ != nullptr)shot_->Draw();
	
	//カメラ範囲に含まれるか調べる
	if (IsInCameraView())
	{
		//含まれる場合
		return;	//描画を行わない
	}

	//砲台の描画
	//MV1DrawModel(trans_.modelId);
	toonStyle_->Draw();
	//砲身の描画
	toonBarrel_->Draw();
	//MV1DrawModel(barrelTrans_.modelId);

	//DrawFormatString(0, 32, 0xffffff, "%.2f,%.2f,%.2f", barrelTrans_.pos.x, barrelTrans_.pos.y, barrelTrans_.pos.z);
	//DrawFormatString(0, 48, 0xffffff, "%.2f,%.2f,%.2f", turretAddRot_.x, turretAddRot_.y, turretAddRot_.z);
	//DrawFormatString(0, 64, 0xffffff, "%.2f,%.2f,%.2f", barrelAddRot_.x, barrelAddRot_.y, barrelAddRot_.z);

	DrawSphere3D(targetPos_, 10.0, 20, 0xffffff, 0xffffff, true);

	//DrawLine3D(VAdd(barrelTrans_.pos,barrelTrans_.localPos), targetPos_, 0x666666);
	//DrawLine3D(VAdd(barrelTrans_.pos, barrelTrans_.localPos), VScale(barrelTrans_.quaRot.Mult(barrelTrans_.quaRotLocal).GetForward(),1000.0f), 0x666666);

	//DrawSphere3D(trans_.pos, AIM_RADIUS, 5, 0xffffff, 0xffffff, false);
	

}

void Cannon::OnHit(const std::weak_ptr<Collider> _hitCol)
{
	for (auto hitTag : _hitCol.lock()->GetTags())
	{
		//影への当たり判定をしない
		if (hitTag == Collider::TAG::SHADOW)return;
	}

	for (auto hitTag : _hitCol.lock()->GetTags())
	{
		//狙う範囲に当たったか
		if (colParam_[AIM_COL_NUM].collider_->IsHit())
		{
			//総合
			VECTOR genePos = Utility::VECTOR_ZERO;

			//対応タグに格納
			compPos_[hitTag] = _hitCol.lock()->GetParent().GetTransform().pos;

			for (auto compPos : compPos_)
			{
				//元の距離
				float preDis = Utility::Distance(genePos, barrelTrans_.pos);
				
				//比較距離
				float dis = Utility::Distance(compPos.second, barrelTrans_.pos);

				//近いほうを狙う
				genePos = dis > preDis ? genePos : compPos.second;
			}

			//近かったほうを狙う
			targetPos_ = genePos;
		}
	}

	//弾の生成
	CreateShot();
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

void Cannon::ResetValue(void)
{
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


	//弾の消去
	shot_.reset();

	//共通
	ItemBase::ResetValue();
}

void Cannon::RotateTurret(void)
{
	//対象までの回転軸
	turretAddRot_ = Utility::GetRotAxisToTarget(VAdd(trans_.pos,trans_.localPos), targetPos_);

	//砲台回転
	Utility::LookAtTarget(trans_, VGet(0.0f, turretAddRot_.y, 0.0f), AIM_TIME_TURRET);
}

void Cannon::RotateBarrel(void)
{
	//対象までの回転軸
	barrelAddRot_ = Utility::GetRotAxisToTarget(VAdd(barrelTrans_.pos, barrelTrans_.localPos), targetPos_);

	//距離で補正
	VECTOR targetVec = Utility::GetMoveVec(barrelTrans_.pos, targetPos_);
	double dis = Utility::Distance(targetVec, Utility::VECTOR_ZERO);

	//砲身回転
	Utility::LookAtTarget(barrelTrans_, VGet(barrelAddRot_.x / static_cast<float>(dis),turretAddRot_.y, 0.0f), AIM_TIME_BARREL);
}

void Cannon::CreateShot(void)
{
	//生成間隔を達していないなら生成処理をしない
	if (shotCreateCnt_ < SHOT_INTERVAL)return;

	//砲身の全回転
	Quaternion barrelAllRot = barrelTrans_.quaRot.Mult(barrelTrans_.quaRotLocal);

	//弾の生成
	std::unique_ptr<CannonShot> shot = std::make_unique<CannonShot>(VAdd(barrelTrans_.pos,barrelTrans_.localPos), barrelAllRot,barrelTrans_.scl);

	//弾の初期化
	shot->Init();

	//配列にセット
	shot_ = std::move(shot);

	//生成間隔カウンタ初期化
	shotCreateCnt_ = 0;
}

void Cannon::DeleteShot(void)
{
	//弾が生きてるならならスキップ
	if (shot_->IsAlive())return;
	
	//弾消去
	shot_.reset();
}

void Cannon::InitShader(void)
{
	toonStyle_ = std::make_unique<ToonStyle>();
	toonStyle_->Load(trans_.modelId, ToonStyle::MESH_TYPE::MESH);
	toonStyle_->Init();
	toonStyle_->SetTexturesIndex(-1.0f);

	toonBarrel_ = std::make_unique <ToonStyle>();
	toonBarrel_->Load(barrelTrans_.modelId, ToonStyle::MESH_TYPE::NORMAL);
	toonBarrel_->Init();
	toonBarrel_->SetTexturesIndex(-1.0f);
}
