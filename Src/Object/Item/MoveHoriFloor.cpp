#include "../Utility/Utility.h"
#include "../Common/Quaternion.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/System/ResourceManager.h"
#include "MoveHoriFloor.h"

MoveHoriFloor::MoveHoriFloor()
{
	routeNum_ = 0;
	for (auto& route : route_)
	{
		route = Utility::VECTOR_ZERO;
	}
	speed_ = 0.0f;
	cnt_ = 0.0f;
	distance_ = 0.0;
}

MoveHoriFloor::~MoveHoriFloor()
{
}

void MoveHoriFloor::Load(void)
{
}

void MoveHoriFloor::Init(void)
{
	//モデルの基本設定
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::MOVE_FLOOR));

	//個々の設定
	SetParam();

	//ルート設定
	SetRoute();

	//モデルの更新
	transform_.Update();
}

void MoveHoriFloor::SetParam(void)
{
	size_ = MAP_SIZE;
	status_.isBreak = true;
	status_.isGravity = false;
	status_.itemType = ITEM_TYPE::FLOOR;
	status_.effType = EFFECT_TYPE::INSTALLATION;
}

void MoveHoriFloor::Update(void)
{
	//移動処理
	Move();

	//モデルの更新
	transform_.Update();
}

void MoveHoriFloor::Draw(void)
{
	DrawLine3D(route_[0], route_[1], 0xffffff);
	DrawSphere3D(transform_.pos, 50, 10, 0xffffff, 0xffffff, true);
	MV1DrawModel(transform_.modelId);
}

void MoveHoriFloor::Release(void)
{
}

void MoveHoriFloor::Move(void)
{
	//カウンタ
	cnt_ += SceneManager::GetInstance().GetDeltaTime();

	//指定秒数経ったか
	if (cnt_ >= ONE_POINT_SEC)
	{
		//地点用ナンバー増加
		routeNum_++;

		if (routeNum_ >= ROUTE)routeNum_ = 0;

		//初期化
		cnt_ = 0.0f;
	}

	//開始地点
	VECTOR startRoute = route_[routeNum_];
	//終了地点のナンバー
	int goalRouteNum = routeNum_ + 1;
	if(goalRouteNum >= ROUTE)goalRouteNum = 0;

	//終了地点
	VECTOR goalRoute = route_[goalRouteNum];

	//移動ベクトル
	VECTOR movePow = Utility::GetMoveVec(startRoute, goalRoute, speed_);

	//移動
	transform_.pos = VAdd(transform_.pos, movePow);
}

void MoveHoriFloor::SetRoute(void)
{
	//初期位置保存
	route_[routeNum_] = transform_.pos;

	VECTOR movePos = transform_.quaRot.PosAxis(VGet(size_.x * MOVE_X * 100, 0.0f, 0.0f));
	VECTOR goalPos = VAdd(route_[routeNum_], movePos);

	//次の位置保存
	route_[routeNum_ + 1] = goalPos;

	//距離を取得
	distance_ = Utility::Distance(route_[routeNum_], route_[routeNum_ + 1]);

	//速度設定
	speed_ = distance_ / ONE_POINT_SEC * SceneManager::GetInstance().GetDeltaTime();
}
