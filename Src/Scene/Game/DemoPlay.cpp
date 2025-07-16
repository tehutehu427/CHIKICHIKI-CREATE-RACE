#include "../Manager/System/Camera.h"
#include "../Manager/System/DateBank.h"
#include "../Manager/Game/CollisionManager.h"
#include "../Manager/Game/ItemManager.h"
#include "../Manager/Game/GravityManager.h"
#include "../Manager/Game/PlayerManager.h"
#include "../Manager/Game/MapEditer.h"
#include "../Utility/Utility.h"
#include "DemoPlay.h"

DemoPlay::DemoPlay(void)
{
	step_ = 0.0f;
}

DemoPlay::~DemoPlay(void)
{
	//各インスタンスの削除
	ItemManager::GetInstance().Destroy();
	GravityManager::GetInstance().Destroy();
	PlayerManager::GetInstance().Destroy();
}

void DemoPlay::Load(void)
{
	//プレイヤーの人数
	DateBank::GetInstance().SetPlayerNum(4);

	//各インスタンスの生成
	ItemManager::CreateInstance();
	GravityManager::CreateInstance();
	PlayerManager::CreateInstance();
	PlayerManager::GetInstance().Load();
}

void DemoPlay::Init(void)
{
	//インスタンス取得
	auto& scnMng = SceneManager::GetInstance();
	auto camera = scnMng.GetCamera(0).lock();
	auto& itemMng = ItemManager::GetInstance();
	auto& plMng = PlayerManager::GetInstance();

	//初期化
	itemMng.Init();
	plMng.Init();

	//カメラ変更

	//固定
	camera->ChangeMode(Camera::MODE::FIXED_POINT);

	//位置
	camera->SetPos(CAMERA_POS);

	//角度
	camera->SetTargetPos(CAMERA_TARGET_POS);

	//デモステージ生成
	CreateDemoStage();
}

void DemoPlay::Update(void)
{
	//デルタタイム
	float delta = SceneManager::GetInstance().GetDeltaTime();

	//インスタンス取得
	auto& itemMng = ItemManager::GetInstance();
	auto& plMng = PlayerManager::GetInstance();
	auto& colMng = CollisionManager::GetInstance();

	//カウンタ
	step_ += delta;

	//アイテム更新
	itemMng.Update();

	//プレイヤー更新
	plMng.Update();

	//終了した当たり判定の消去
	colMng.Sweep();

	//当たり判定更新
	colMng.Update();

	//プレイヤー
	for (auto& player : plMng.GetPlayers())
	{
		//死んだら
		if (player->IsDeath())
		{
			//復活
			player->Init();
			player->SetPos(revivalPos_[GetRand(1)]);
		}
	}
}

void DemoPlay::Draw(void)
{
	//インスタンス取得
	auto& itemMng = ItemManager::GetInstance();
	auto& plMng = PlayerManager::GetInstance();

	//アイテム描画
	itemMng.Draw();

	//プレイヤー描画
	plMng.Draw();
}

void DemoPlay::CreateDemoStage(void)
{
	//インスタンス取得
	auto& itemMng = ItemManager::GetInstance();
	auto& plMng = PlayerManager::GetInstance();

	//プレイヤーの初期座標
	VECTOR initPos = { 200.0f,200.0f,200.0f };
	plMng.SetInitPos(initPos);
	revivalPos_.emplace_back(VGet(initPos.x, initPos.y + 2000.0f, initPos.z));

	//生成座標
	IntVector3 mapPos = { 0,0,0 };
	//生成角度
	Quaternion rot = Quaternion();
	float rotY = 0.0f;

	//生成
	itemMng.AddItem(mapPos, rot, ItemBase::ITEM_TYPE::FLOOR, rotY);

	//座標変更
	mapPos = { 4,0,0 };

	//生成
	itemMng.AddItem(mapPos, rot, ItemBase::ITEM_TYPE::MOVE_HORI_FLOOR, rotY);

	//座標変更
	mapPos = { 8,0,4 };
	VECTOR revPos = VAdd(initPos, MapEditer::GetInstance().MapToWorldPos(mapPos));
	revivalPos_.emplace_back(VGet(revPos.x, revPos.y + 2000.0f, revPos.z));

	//生成
	itemMng.AddItem(mapPos, rot, ItemBase::ITEM_TYPE::FLOOR, rotY);

	//座標変更
	mapPos = { 4,0,4 };
	rotY = 180.0f;
	rot = rot.AngleAxis(Utility::Deg2RadF(rotY), Utility::AXIS_Y);

	//生成
	itemMng.AddItem(mapPos, rot, ItemBase::ITEM_TYPE::MOVE_HORI_FLOOR, rotY);
}
