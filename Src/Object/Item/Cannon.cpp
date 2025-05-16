#include "../../Manager/System/ResourceManager.h"
#include "../../Manager/System/InputManager.h"
#include "../../Manager/System/SceneManager.h"
#include "../../Utility/Utility.h"
#include "../../FpsControl/FpsControl.h"
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

	//ステータス初期化
	size_ = MAP_SIZE;
	status_.isBreak = true;
	status_.isGravity = false;
	status_.effType = EFFECT_TYPE::INSTALLATION;

	//砲身を砲台に合わせておく
	barrelTrans_.pos = VAdd(trans_.pos,BARREL_LOCAL_POS);
	//角度もまっすぐに
	barrelTrans_.quaRotLocal = Quaternion::Euler(
		Utility::Deg2RadF(BARREL_LOCAL_ROT.x), 
		Utility::Deg2RadF(BARREL_LOCAL_ROT.y),
		Utility::Deg2RadF(BARREL_LOCAL_ROT.z));
	
	//砲身のモデル設定
	barrelTrans_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::CANNON_BARREL));
}

void Cannon::Update(void)
{
	//弾の削除処理
	DeleteShot();

	//弾の生成
	CreateShot();

	auto& ins = InputManager::GetInstance();
	if (ins.IsNew(KEY_INPUT_UP))targetPos_.z++;
	if (ins.IsNew(KEY_INPUT_RIGHT))targetPos_.x++;
	if (ins.IsNew(KEY_INPUT_DOWN))targetPos_.z--;
	if (ins.IsNew(KEY_INPUT_LEFT))targetPos_.x--;
	if (ins.IsNew(KEY_INPUT_RSHIFT))targetPos_.y++;
	if (ins.IsNew(KEY_INPUT_RCONTROL))targetPos_.y--;


	//対象を狙う
	Aim();

	//砲台の回転
	RotateTurret();

	//砲身の回転
	RotateBarrel();

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

	DrawLine3D(barrelTrans_.pos, targetPos_, 0x666666);
	DrawLine3D(barrelTrans_.pos, VScale(barrelTrans_.quaRot.Mult(barrelTrans_.quaRotLocal).GetForward(),1000.0f), 0x666666);

	DrawSphere3D(trans_.pos, AIM_RADIUS, 5, 0xffffff, 0xffffff, false);

	//弾関係
	for (auto& shot : shots_)
	{
		//弾がないならスキップ
		if (shot == nullptr)continue;

		//弾の描画
		shot->Draw();
	}
}

void Cannon::Aim(void)
{
	//対象への方向ベクトル取得
	targetVec_ = Utility::GetMoveVec(barrelTrans_.pos, targetPos_);
}

void Cannon::RotateTurret(void)
{
	//砲台と標的のベクトルの差異
	VECTOR turretVecDiff = VSub(targetVec_, trans_.pos);

	//正規化
	turretVecDiff = VNorm(turretVecDiff);

	//横回転なので、Yベクトルは使わない
	turretVecDiff.y = 0.0f;

	//砲台の回転量
	Quaternion turQuaRot = Quaternion::LookRotation(turretVecDiff);

	//VECTOR変換
	turretAddRot_ = turQuaRot.ToEuler();

	//砲台回転
	Rotate(trans_, turretAddRot_);
}

void Cannon::RotateBarrel(void)
{
	//砲身と標的のベクトルの差異
	VECTOR barrelVecDiff = VSub(targetVec_, barrelTrans_.pos);

	//正規化
	barrelVecDiff = VNorm(barrelVecDiff);

	//縦回転なので、XZベクトルは使わない
	barrelVecDiff.x = 0.0f;
	barrelVecDiff.z = 0.0f;

	//砲身の回転量
	Quaternion barQuaRot = Quaternion::LookRotation(barrelVecDiff);

	//距離で補正
	float distance = Utility::Distance(targetVec_, barrelTrans_.pos);
	barQuaRot = barQuaRot.AngleAxis(distance, Utility::AXIS_X);

	//VECTOR変換
	barrelAddRot_ = barQuaRot.ToEuler();

	//砲身回転
	Rotate(barrelTrans_, VAdd(barrelAddRot_, turretAddRot_));
}

void Cannon::Rotate(Transform& _trans, const VECTOR _addAxis, const VECTOR _relativePos)const
{
	//回転
	Quaternion rot = Quaternion::Identity();

	//回転を加える
	rot = rot.Mult(Quaternion::Euler(_addAxis));

	//回転用に座標を相対座標から0,0,0にそろえる
	_trans.pos = VSub(_trans.pos,_trans.quaRot.PosAxis(_relativePos));
	
	//反映
	_trans.quaRot = rot;
	
	//座標を元に戻す
	_trans.pos = VAdd(_trans.pos, _trans.quaRot.PosAxis(_relativePos));

	//基本情報更新
	_trans.Update();
}

void Cannon::CreateShot(void)
{
	//デルタタイム取得
	auto delta = SceneManager::GetInstance().GetDeltaTime();

	//生成間隔カウント
	shotCreateCnt_ += delta;

	//弾が最大数生成されている　又は　生成間隔を達していないなら生成処理をしない
	if (shotNum_ >= SHOT_MAX || shotCreateCnt_ < SHOT_INTERVAL)return;

	//砲身の全回転
	Quaternion barrelAllRot = barrelTrans_.quaRot.Mult(barrelTrans_.quaRotLocal);

	//弾の生成
	std::unique_ptr<CannonShot> shot = std::make_unique<CannonShot>(barrelTrans_.pos, barrelAllRot);

	//弾の初期化
	shot->Init();

	//配列にセット
	shots_[shotNum_] = std::move(shot);

	//生成数カウント増加
	shotNum_++;

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
