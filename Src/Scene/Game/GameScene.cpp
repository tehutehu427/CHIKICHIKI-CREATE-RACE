#include <DxLib.h>
#include "../../Application.h"
#include "../Common/FontRegistry.h"
#include "../../Utility/Utility.h"
#include "../../Manager/System/SceneManager.h"
#include "../../Manager/System/ResourceManager.h"
#include "../../Manager/System/Camera.h"
#include "../../Manager/System/InputManager.h"
#include "../../Manager/System/DateBank.h"
#include "../../Manager/Game/ItemManager.h"
#include "../../Manager/Game/MapEditer.h"
#include "../../Manager/Game/GravityManager.h"
#include "../../Manager/Game/PlayerManager.h"
#include "../../Object/Player/Player.h"
#include "../../Object/Editor/Palette/EditorPaletteBase.h"
#include "../../Object/Grid.h"
#include "../../Object/SkyDome/SkyDome.h"
#include "../../Object/System/GameClear.h"
#include "GameScene.h"

GameScene::GameScene(void)
{
	//更新関数のセット
	updataFunc_ = std::bind(&GameScene::LoadingUpdate, this);
	//描画関数のセット
	drawFunc_ = std::bind(&GameScene::LoadingDraw, this);

	sky_ = nullptr;
	palette_ = nullptr;
	phaseChanges_.emplace(PHASE::EDIT_PHASE, std::bind(&GameScene::ChangePhaseEdit, this));
	phaseChanges_.emplace(PHASE::ACTION_PHASE, std::bind(&GameScene::ChangePhaseAction, this));
	phaseChanges_.emplace(PHASE::CLEAR_PHASE, std::bind(&GameScene::ChangePhaseClear, this));
}

GameScene::~GameScene(void)
{
	//インスタンスの削除
	PlayerManager::GetInstance().Destroy();
}

void GameScene::Load(void)
{
	//フォントの生成
	buttnFontHandle_ = CreateFontToHandle(FontRegistry::DOT.c_str(), FONT_SIZE, 0);

	//player_ = std::make_unique<Player>();
	//player_->Load();

	//for (int i = 0; i < DateBank::GetInstance().GetPlayerNum();i++)
	//{
	// 
	//	editController_ = std::make_shared<EditController>(i);
	//}
	editController_ = std::make_shared<EditController>(0);

	palette_ = std::make_unique<EditorPaletteBase>(*editController_);
	palette_->Load();

	sky_ = std::make_unique<SkyDome>();
	sky_->Load();

	gameClear_ = std::make_unique<GameClear>();
	gameClear_->Load();
}

void GameScene::Init(void)
{
	palette_->Init();
	editController_->Init();
	sky_->Init();
	gameClear_->Init();
	MapEditer::CreateInstance();
	ItemManager::CreateInstance();
	GravityManager::CreateInstance();

	PlayerManager::CreateInstance(3);

	//アイテム生成
	ItemManager::GetInstance().AddItem({ 0,0,0 }, Quaternion(), ItemBase::ITEM_TYPE::CANNON);
	//ItemManager::GetInstance().AddItem({ 3,2,3 }, Quaternion(), ItemBase::ITEM_TYPE::FLOOR);
	//ItemManager::GetInstance().AddItem({ 8,2,8 }, Quaternion(), ItemBase::ITEM_TYPE::FLOOR);
	//ItemManager::GetInstance().AddItem({ 10,3,20 }, Quaternion(), ItemBase::ITEM_TYPE::MOVE_HORI_FLOOR);
	//ItemManager::GetInstance().AddItem({ 15,3,20 }, Quaternion(), ItemBase::ITEM_TYPE::MOVE_VER_FLOOR);
	ChangePhase(PHASE::EDIT_PHASE);
}

void GameScene::NormalUpdate(void)
{
	//プレイヤー
	//player_->Update();



	PlayerManager::GetInstance().Update();

	phaseUpdate_();


	sky_->Update();

	//デバッグ処理
	DebagUpdate();
}

void GameScene::NormalDraw(void)
{
	//モデルを先に描画させて、UIや2D系の描画を後から描画する
	//デバッグ描画
	DebagDraw();

	//スカイドーム
	sky_->Draw();

	//アイテム
	ItemManager::GetInstance().Draw();

	phaseDraw_();
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
	else if (ins.IsTrgDown(KEY_INPUT_C))
	{
		ChangePhase(PHASE::CLEAR_PHASE);
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
	phaseDraw_ = std::bind(&GameScene::DrawEdit, this);
	SceneManager::GetInstance().GetCamera().lock()->ChangeMode(Camera::MODE::FREE_CONTROLL);
	VECTOR pos;
	IntVector3 mPos = MapEditer::MAP_SIZE;
	pos = { static_cast<float>(mPos.x * MapEditer::GRID_SIZE) / 2,static_cast<float>(mPos.y * MapEditer::GRID_SIZE) / 2,static_cast<float>(mPos.z * MapEditer::GRID_SIZE) / 2 };
	pos = { 0.0f,250.0f,-500.0f };
	//SceneManager::GetInstance().GetCamera().lock()->SetPos(pos);
}

void GameScene::ChangePhaseAction(void)
{
	phaseUpdate_ = std::bind(&GameScene::UpdateAction, this);
	phaseDraw_ = std::bind(&GameScene::DrawAction, this);
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

void GameScene::ChangePhaseClear(void)
{
	phaseUpdate_ = std::bind(&GameScene::UpdateClear, this);
	phaseDraw_ = std::bind(&GameScene::DrawClear, this);
}

void GameScene::UpdateEdit(void)
{
	//パレット
	palette_->Update();
	editController_->Update();
}

void GameScene::UpdateAction(void)
{
	ItemManager::GetInstance().Update();
}

void GameScene::UpdateClear(void)
{
	//プレイヤーにアニメーションをさせたりする
	//エフェクトなどを表示させる

	gameClear_->Update();
}

void GameScene::DrawEdit(void)
{
	//グリッド
	grid_->Draw();

	//パレット
	palette_->Draw();


	//エディットコントローラー
	editController_->Draw();
}

void GameScene::DrawAction(void)
{
	//プレイヤー
	PlayerManager::GetInstance().Draw();
}

void GameScene::DrawClear()
{
	//プレイ画面を背景に描画
	DrawAction();

	//ゲームクリアの描画
	gameClear_->Draw();
}
