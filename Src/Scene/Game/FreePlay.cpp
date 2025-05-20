#include "FreePlay.h"
#include "../../Object/Editor/MapDataIO.h"

FreePlay::FreePlay(void)
{
	mapIO_ = nullptr;
}

FreePlay::~FreePlay(void)
{
}

void FreePlay::Load(void)
{
	//親クラスの読み込み
	GameScene::Load();

	//マップデータの入出力
	mapIO_ = std::make_unique<MapDataIO>();
	mapIO_->Load();
}

void FreePlay::Init(void)
{
	//親クラスの初期化
	GameScene::Init();

	//マップデータの初期化
	mapIO_->Init();
}

void FreePlay::UpdateAction(void)
{
	//親クラスの更新
	GameScene::UpdateAction();
}

void FreePlay::UpdateEdit(void)
{
	//親クラスの更新
	GameScene::UpdateEdit();

	//マップデータの更新
	mapIO_->Update();
}

void FreePlay::NormalDraw(void)
{
	//親クラスの描画
	GameScene::NormalDraw();

	//マップデータの描画
	mapIO_->Draw();
}