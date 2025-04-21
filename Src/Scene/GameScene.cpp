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
#include "../Manager/Game/PlayerManager.h"
#include "../Object/Player/Player.h"
#include "../Object/Editer/Palette/EditerPaletteBase.h"
#include "../Object/Grid.h"
#include "GameScene.h"

GameScene::GameScene(void)
{
	//更新関数のセット
	updataFunc_ = std::bind(&GameScene::LoadingUpdate, this);
	//描画関数のセット
	drawFunc_ = std::bind(&GameScene::LoadingDraw, this);

	palette_ = nullptr;
	phaseChanges_.emplace(PHASE::EDIT_PHASE, std::bind(&GameScene::ChangePhaseEdit, this));
	phaseChanges_.emplace(PHASE::ACTION_PHASE, std::bind(&GameScene::ChangePhaseAction, this));
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

	//player_ = std::make_unique<Player>();
	//player_->Load();

	//palette_ = std::make_unique<EditerPaletteBase>();
	//palette_->Load();
}

void GameScene::Init(void)
{
	//palette_->Init();
	MapEditer::CreateInstance();
	ItemManager::CreateInstance();
	GravityManager::CreateInstance();
	PlayerManager::CreateInstance(2);

	//アイテム生成
	ItemManager::GetInstance().AddItem({ 0,0,0 }, Quaternion(), ItemBase::ITEM_TYPE::CANNON);
	ChangePhase(PHASE::EDIT_PHASE);
}

void GameScene::NormalUpdate(void)
{
	//プレイヤー
	//player_->Update();

	//パレット
	//palette_->Update();

	PlayerManager::GetInstance().Update();

	phaseUpdate_();

	ItemManager::GetInstance().Update();
	//デバッグ処理
	DebagUpdate();
}

void GameScene::NormalDraw(void)
{
	//デバッグ処理
	DebagDraw();

	grid_->Draw();
	//プレイヤー

	//palette_->Draw();

	PlayerManager::GetInstance().Draw();

	ItemManager::GetInstance().Draw();
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
	if (ins.IsTrgDown(KEY_INPUT_Z))
	{
		ChangePhase(phase_ == PHASE::ACTION_PHASE ? PHASE::EDIT_PHASE : PHASE::ACTION_PHASE);
	}
}

void GameScene::DebagDraw(void)
{
	DrawBox(
		0,
		0,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		0x000000,
		true
	);

	DrawFormatString(
		0, 0,
		0xff0000,
		"GameScene"
	);

	//palette_->DebagDraw();
}
void GameScene::ChangePhase(PHASE phase)
{
	phase_ = phase;

	phaseChanges_[phase_]();
}

void GameScene::ChangePhaseEdit(void)
{
	phaseUpdate_ = std::bind(&GameScene::UpdateEdit, this);
	SceneManager::GetInstance().GetCamera().lock()->ChangeMode(Camera::MODE::FREE_CONTROLL);
	VECTOR pos;
	IntVector3 mPos = MapEditer::MAP_SIZE;
	pos = { static_cast<float>(mPos.x * MapEditer::GRID_SIZE) / 2,static_cast<float>(mPos.y * MapEditer::GRID_SIZE) / 2,static_cast<float>(mPos.z * MapEditer::GRID_SIZE) / 2 };
	pos = { 0.0f,250.0f,-500.0f };
	SceneManager::GetInstance().GetCamera().lock()->SetPos(pos);
}

void GameScene::ChangePhaseAction(void)
{
	phaseUpdate_ = std::bind(&GameScene::UpdateAction, this);
	SceneManager::GetInstance().GetCamera().lock()->ChangeMode(Camera::MODE::FIXED_UP);
	VECTOR pos;
	IntVector3 mPos = MapEditer::MAP_SIZE;
	pos = { static_cast<float>(mPos.x * MapEditer::GRID_SIZE) / 2,static_cast<float>(mPos.y * MapEditer::GRID_SIZE) * 8.5f,static_cast<float>(mPos.z * MapEditer::GRID_SIZE) / 2 };
	SceneManager::GetInstance().GetCamera().lock()->SetPos(pos);
	VECTOR angles = {};
	angles.x = Utility::Deg2RadF(90.0);
	SceneManager::GetInstance().GetCamera().lock()->SetAngles(angles);
	//SceneManager::GetInstance().GetCamera().lock()->SetTargetPos({ static_cast<float>(mPos.x * MapEditer::GRID_SIZE) / 2, 0.0f, static_cast<float>(mPos.z * MapEditer::GRID_SIZE) / 2 });
}

void GameScene::UpdateEdit(void)
{
}

void GameScene::UpdateAction(void)
{
}
