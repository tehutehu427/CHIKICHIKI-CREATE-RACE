#include "../Utility/Utility.h"
#include "../Common/Quaternion.h"
#include "../Manager/Game/MapEditer.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/System/ResourceManager.h"
#include "FerrisWheel.h"

FerrisWheel::FerrisWheel()
{
	routeNum_ = 0;
	for (int i = 0; i < ROUTE; i++)
	{
		route_[i] = Utility::VECTOR_ZERO;
	}
	startRoute_ = Utility::VECTOR_ZERO;
	goalRoute_ = Utility::VECTOR_ZERO;
	speed_ = 0.0f;
	distance_ = 0.0;
	moveVec_ = Utility::VECTOR_ZERO;
	movePow_ = Utility::VECTOR_ZERO;
}

FerrisWheel::~FerrisWheel()
{
}

void FerrisWheel::SetParam(void)
{
	//モデルの基本設定
	trans_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::MOVE_FLOOR));

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

	//ルート設定
	InitRoute();
}

void FerrisWheel::Update(void)
{
	//移動処理
	Move();

	//モデルの更新
	trans_.Update();
}

void FerrisWheel::Draw(void)
{
	DrawLine3D(VAdd(route_[0], MAP_LOCALPOS), VAdd(route_[1], MAP_LOCALPOS), Utility::BLACK);
	MV1DrawModel(trans_.modelId);
}

const IntVector3 FerrisWheel::GetSize(void) const
{
	return size_ + IntVector3(MOVE_X, MOVE_Y, 0);
}

void FerrisWheel::OnHit(const std::weak_ptr<Collider> _hitCol)
{
}

void FerrisWheel::Move(void)
{
	//指定ルートを超えたか
	if (IsBeyondRoute())
	{
		//現在位置の補正
		trans_.pos = route_[routeNum_];

		//ルートの再設定
		SetRoute();
	}

	//移動
	trans_.pos = VAdd(trans_.pos, movePow_);
}

void FerrisWheel::InitRoute(void)
{
	//初期位置保存
	route_[routeNum_] = trans_.pos;

	//マップ座標をワールド座標に
	VECTOR intPos = MapEditer::GetInstance().MapToWorldPos({ MOVE_X, 0, 0 });

	//移動量
	VECTOR movePos = trans_.quaRot.PosAxis(intPos);

	//目標地点
	VECTOR goalPos = VAdd(route_[routeNum_], movePos);

	//次の位置保存
	route_[routeNum_ + 1] = goalPos;

	//距離を取得
	distance_ = Utility::Distance(route_[routeNum_], route_[routeNum_ + 1]);

	//速度設定
	speed_ = static_cast<float>(distance_) / ONE_POINT_SEC * SceneManager::GetInstance().GetDeltaTime();

	//初期ルート設定
	SetRoute();
}

void FerrisWheel::SetRoute(void)
{
	//開始地点
	startRoute_ = route_[routeNum_];

	//地点用ナンバー増加
	routeNum_++;
	if (routeNum_ >= ROUTE)routeNum_ = 0;

	//終了地点
	goalRoute_ = route_[routeNum_];

	//方向ベクトル保存
	moveVec_ = Utility::GetMoveVec(startRoute_, goalRoute_);

	//移動量
	movePow_ = Utility::GetMoveVec(startRoute_, goalRoute_, speed_);
}

bool FerrisWheel::IsBeyondRoute(void)
{
	//Xの比較
	bool beyondX;
	if (moveVec_.x >= 0.0f)beyondX = trans_.pos.x >= route_[routeNum_].x + moveVec_.x;
	else beyondX = trans_.pos.x < route_[routeNum_].x + moveVec_.x;

	//Yの比較
	bool beyondY;
	if (moveVec_.y >= 0.0f)beyondY = trans_.pos.y >= route_[routeNum_].y + moveVec_.y;
	else beyondY = trans_.pos.y < route_[routeNum_].y + moveVec_.y;

	//Zの比較
	bool beyondZ;
	if (moveVec_.z >= 0.0f)beyondZ = trans_.pos.z >= route_[routeNum_].z + moveVec_.z;
	else beyondZ = trans_.pos.z < route_[routeNum_].z + moveVec_.z;

	return beyondX && beyondY && beyondZ;
}