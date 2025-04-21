#include "../Utility/Utility.h"
#include "../Common/Quaternion.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/System/ResourceManager.h"
#include "MoveVerFloor.h"

MoveVerFloor::MoveVerFloor()
{
	routeNum_ = 0;
	for (auto& route : route_)
	{
		route = Utility::VECTOR_ZERO;
	}
	startRoute_ = Utility::VECTOR_ZERO;
	goalRoute_ = Utility::VECTOR_ZERO;
	speed_ = 0.0f;
	distance_ = 0.0;
	moveVec_ = Utility::VECTOR_ZERO;
	movePow_ = Utility::VECTOR_ZERO;
}

MoveVerFloor::~MoveVerFloor()
{
}

void MoveVerFloor::SetParam(void)
{
	//モデルの基本設定
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::MOVE_FLOOR));

	size_ = MAP_SIZE;
	status_.isBreak = true;
	status_.isGravity = false;
	status_.effType = EFFECT_TYPE::INSTALLATION;

	//ルート設定
	InitRoute();
}

void MoveVerFloor::Update(void)
{
	//移動処理
	Move();

	//モデルの更新
	transform_.Update();
}

void MoveVerFloor::Draw(void)
{
	DrawLine3D(route_[0], route_[1], 0xffffff);
	MV1DrawModel(transform_.modelId);
}

void MoveVerFloor::Release(void)
{
}

void MoveVerFloor::Move(void)
{
	//指定ルートを超えたか
	if (IsBeyondRoute())
	{
		//現在位置の補正
		transform_.pos = route_[routeNum_];

		//ルートの再設定
		SetRoute();
	}

	//移動
	transform_.pos = VAdd(transform_.pos, movePow_);
}

void MoveVerFloor::InitRoute(void)
{
	//初期位置保存
	route_[routeNum_] = transform_.pos;

	VECTOR movePos = transform_.quaRot.PosAxis(VGet(0.0f, size_.y * MOVE_Y * 100, 0.0f));
	VECTOR goalPos = VAdd(route_[routeNum_], movePos);

	//次の位置保存
	route_[routeNum_ + 1] = goalPos;

	//距離を取得
	distance_ = Utility::Distance(route_[routeNum_], route_[routeNum_ + 1]);

	//速度設定
	speed_ = distance_ / ONE_POINT_SEC * SceneManager::GetInstance().GetDeltaTime();

	//初期ルート設定
	SetRoute();
}

void MoveVerFloor::SetRoute(void)
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

bool MoveVerFloor::IsBeyondRoute(void)
{
	bool beyondX;
	if (moveVec_.x >= 0.0f)beyondX = transform_.pos.x >= route_[routeNum_].x + moveVec_.x;
	else beyondX = transform_.pos.x < route_[routeNum_].x + moveVec_.x;

	bool beyondY;
	if (moveVec_.y >= 0.0f)beyondY = transform_.pos.y >= route_[routeNum_].y + moveVec_.y;
	else beyondY = transform_.pos.y < route_[routeNum_].y + moveVec_.y;

	bool beyondZ;
	if (moveVec_.z >= 0.0f)beyondZ = transform_.pos.z >= route_[routeNum_].z + moveVec_.z;
	else beyondZ = transform_.pos.z < route_[routeNum_].z + moveVec_.z;

	return beyondX && beyondY && beyondZ;
}

void MoveVerFloor::HitObject(Transform& _hitTrans)
{
	_hitTrans.pos = VAdd(_hitTrans.pos, movePow_);
}
