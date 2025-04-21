#include <DxLib.h>
#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/System/ResourceManager.h"
#include "../Manager/System/Camera.h"
#include "../Manager/System/InputManager.h"
#include "../Manager/Game/ItemManager.h"
#include "../Manager/Game/MapEditer.h"
#include "../Manager/Game/GravityManager.h"
#include "../Object/Player/Player.h"
#include "../Object/Editer/Palette/EditerPaletteBase.h"
#include "GameScene.h"

GameScene::GameScene(void)
{
	//更新関数のセット
	updataFunc_ = std::bind(&GameScene::LoadingUpdate, this);
	//描画関数のセット
	drawFunc_ = std::bind(&GameScene::LoadingDraw, this);

	palette_ = nullptr;
}

GameScene::~GameScene(void)
{
	//インスタンスの削除
}

void GameScene::Load(void)
{
	//フォントの登録
	resMng_.Load(ResourceManager::SRC::DOT_FONT);
	buttnFontHandle_ = CreateFontToHandle("ベストテンDOT", FONT_SIZE, 0);

	player_ = std::make_unique<Player>();
	player_->Load();

	palette_ = std::make_unique<EditerPaletteBase>();
	palette_->Load();
}

void GameScene::Init(void)
{
	player_->Init();
	palette_->Init();
	MapEditer::CreateInstance();
	ItemManager::CreateInstance();
	GravityManager::CreateInstance();
}

void GameScene::NormalUpdate(void)
{
	//プレイヤー
	player_->Update();

	//パレット
	palette_->Update();

	//デバッグ処理
	DebagUpdate();
}

void GameScene::NormalDraw(void)
{
	//デバッグ処理
	DebagDraw();

	//プレイヤー
	player_->Draw();

	palette_->Draw();
}

void GameScene::ChangeNormal(void)
{
	//処理変更
	updataFunc_ = std::bind(&GameScene::NormalUpdate, this);
	drawFunc_ = std::bind(&GameScene::NormalDraw, this);
}

void GameScene::DebagUpdate(void)
{
	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
}

void GameScene::DebagDraw(void)
{
	DrawBox(
		0,
		0,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		0x00ff00,
		true
	);

	DrawFormatString(
		0, 0,
		0x000000,
		"GameScene"
	);

	palette_->DebagDraw();
}
