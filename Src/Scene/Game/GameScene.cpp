#include <DxLib.h>
#include "../../Application.h"
#include "../Common/FontRegistry.h"
#include "../../Utility/Utility.h"
#include "../../Manager/System/SceneManager.h"
#include "../../Manager/System/ResourceManager.h"
#include "../../Manager/System/Camera.h"
#include "../../Manager/System/InputManager.h"
#include "../../Manager/System/DateBank.h"
#include "../../Manager/Game/CollisionManager.h"
#include "../../Manager/Game/ItemManager.h"
#include "../../Manager/Game/MapEditer.h"
#include "../../Manager/Game/GravityManager.h"
#include "../../Manager/Game/PlayerManager.h"
#include "../../Object/Player/Player.h"
#include "../../Object/Editor/Palette/EditorPaletteBase.h"
#include "../../Object/Editor/MapDataIO.h"
#include "../../Object/Grid.h"
#include "../../Object/SkyDome/SkyDome.h"
#include "../../Object/System/GameClear.h"
#include "GameScene.h"

GameScene::GameScene(void)
{
	//更新関数のセット
	func_.updataFunc_ = std::bind(&GameScene::LoadingUpdate, this);
	//描画関数のセット
	func_.drawFunc_ = std::bind(&GameScene::LoadingDraw, this);

	sky_ = nullptr;
	palette_ = nullptr;

	//フェーズの状態遷移処理を登録
	phaseChanges_.emplace(PHASE::EDIT_PHASE, std::bind(&GameScene::ChangePhaseEdit, this));
	phaseChanges_.emplace(PHASE::ACTION_PHASE, std::bind(&GameScene::ChangePhaseAction, this));
	phaseChanges_.emplace(PHASE::CLEAR_PHASE, std::bind(&GameScene::ChangePhaseClear, this));
}

GameScene::~GameScene(void)
{
	//インスタンスの削除
	PlayerManager::GetInstance().Destroy();
	ItemManager::GetInstance().Destroy();
	MapEditer::GetInstance().Destroy();
	GravityManager::GetInstance().Destroy();
	DeleteFontToHandle(buttnFontHandle_);
	phaseChanges_.clear();

}

void GameScene::Load(void)
{
	int playerNum = DateBank::GetInstance().GetPlayerNum();

	//フォントの生成
	buttnFontHandle_ = CreateFontToHandle(FontRegistry::DOT.c_str(), FONT_SIZE, 0);

	//プレイヤー管理クラス
	PlayerManager::CreateInstance();
	PlayerManager::GetInstance().Load();

	//マップエディター
	MapEditer::CreateInstance();

	//重力管理クラスを生成
	GravityManager::CreateInstance();
	
	//アイテムマネージャーの生成
	ItemManager::CreateInstance();
	//エディットコントローラーの生成
	for (int i = 0; i < playerNum; i++)
	{
		editControllers_.push_back(std::make_unique<EditController>(i));
	}

	//スカイドームの生成
	sky_ = std::make_unique<SkyDome>();
	sky_->Load();

	//ゲームクリアの生成
	gameClear_ = std::make_unique<GameClear>();
	gameClear_->Load();	

	//マップデータの入出力
	mapIO_ = std::make_unique<MapDataIO>();
	mapIO_->Load();
}

void GameScene::Init(void)
{
	//各クラスの初期化
	for (auto& controller : editControllers_) { controller->Init(); }
	sky_->Init();
	gameClear_->Init();
}

void GameScene::NormalUpdate(void)
{
	//フェーズ別の処理
	phaseUpdate_();

	//スカイドームは常に更新
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

	//フェーズの描画
	phaseDraw_();
}

void GameScene::ChangeNormal(void)
{
	//処理変更
	func_.updataFunc_ = std::bind(&GameScene::NormalUpdate, this);
	func_.drawFunc_ = std::bind(&GameScene::NormalDraw, this);
}

void GameScene::DebagUpdate(void)
{
	// シーン遷移
	KeyConfig& ins = KeyConfig::GetInstance();
	if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::DEBUG_CHENGE_TITLE, KeyConfig::JOYPAD_NO::PAD1))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

	//フェーズ遷移は各アップデートに作ったのでここは消し
	else if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::DEBUG_CHENGE_CLEAR, KeyConfig::JOYPAD_NO::PAD1))
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
void GameScene::ChangePhase(const PHASE phase)
{
	if (phase == PHASE::ACTION_PHASE)
	{
		if (!ItemManager::GetInstance().AllDummyItemAddItems())
		{
			return;
		}
	}
	phase_ = phase;

	phaseChanges_[phase_]();
}

void GameScene::ChangePhaseEdit(void)
{
	phaseUpdate_ = std::bind(&GameScene::UpdateEdit, this);
	phaseDraw_ = std::bind(&GameScene::DrawEdit, this);
	for (int i = 0; i < DateBank::GetInstance().GetPlayerNum(); i++)
	{
		SceneManager::GetInstance().GetCamera(i).lock()->ChangeMode(Camera::MODE::FREE_CONTROLL);
		VECTOR pos;
		IntVector3 mPos = MapEditer::MAP_SIZE;
		pos = { static_cast<float>(mPos.x * MapEditer::GRID_SIZE) / 2,static_cast<float>(mPos.y * MapEditer::GRID_SIZE) / 2,static_cast<float>(mPos.z * MapEditer::GRID_SIZE) / 2 };
		//pos = { 0.0f,250.0f,-500.0f };
		SceneManager::GetInstance().GetCamera(i).lock()->SetPos(pos);
		editControllers_[i]->Reset();
	}
	ItemManager::GetInstance().ResetItemValue();
}

void GameScene::ChangePhaseAction(void)
{

	phaseUpdate_ = std::bind(&GameScene::UpdateAction, this);
	phaseDraw_ = std::bind(&GameScene::DrawAction, this);

	//プレイヤー初期化
	PlayerManager::GetInstance().Init();

	//プレイヤーにスタートオブジェクトにする
	PlayerManager::GetInstance().SetInitPos(ItemManager::GetInstance().GetStartWorldPos());

	for (int i = 0; i < DateBank::GetInstance().GetPlayerNum(); i++)
	{
		//カメラをフォローモードにチェンジ
		SceneManager::GetInstance().GetCamera(i).lock()->ChangeMode(Camera::MODE::FOLLOW);
		//カメラの追従対象をプレイヤーに設定
		SceneManager::GetInstance().GetCamera(i).lock()->SetFollow(&PlayerManager::GetInstance().GetPlayerTransform(i));
	}
	ItemManager::GetInstance().ResetItemValue();
	//VECTOR pos;
	//IntVector3 mPos = MapEditer::MAP_SIZE;
	//pos = { static_cast<float>(mPos.x * MapEditer::GRID_SIZE) / 2,static_cast<float>(mPos.y * MapEditer::GRID_SIZE) * 8.5f,static_cast<float>(mPos.z * MapEditer::GRID_SIZE) / 2 };
	//SceneManager::GetInstance().GetCamera().lock()->SetPos(pos);
	//VECTOR angles = {};
	//angles.x = Utility::Deg2RadF(90.0);
	//SceneManager::GetInstance().GetCamera().lock()->SetAngles(angles);

	//VECTOR pos = ACTION_CAMERA_POS;
	//SceneManager::GetInstance().GetCamera().lock()->SetPos(pos);
	//VECTOR angles = {};
	//angles = Quaternion::FromToRotation(pos, Camera::FIXED_DIAGONAL_TARGET_POS).ToEuler();

	//angles.x = Utility::Deg2RadF(90.0);
	//SceneManager::GetInstance().GetCamera().lock()->SetAngles(angles);
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

	if (palette_->GetState() == EditorPaletteBase::STATE::WAIT)
	{
		for (auto& controller : editControllers_) { controller->Update(); }
	}
}

void GameScene::UpdateAction(void)
{
	ItemManager::GetInstance().Update();

	//プレイヤーの更新
	PlayerManager::GetInstance().Update();

	//終了した当たり判定の消去
	CollisionManager::GetInstance().Sweep();

	//更新はアクション中のみ
	CollisionManager::GetInstance().Update();

	ChangePlayerClearPhase();
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

	//エディットコントローラー
	//for (auto& controller : editControllers_) 
	//{ 
	auto screenIndex = SceneManager::GetInstance().GetScreenIndex();
	editControllers_[screenIndex]->Draw();
	//}

	//アイテム
	ItemManager::GetInstance().Draw();

	//パレット
	palette_->Draw();

	editControllers_[screenIndex]->DrawUI();
}

void GameScene::DrawAction(void)
{
	//プレイヤー
	PlayerManager::GetInstance().Draw();

	//アイテム
	ItemManager::GetInstance().Draw();
}

void GameScene::DrawClear()
{
	//プレイ画面を背景に描画
	DrawAction();

	//ゲームクリアの描画
	gameClear_->Draw();
}

void GameScene::ChangePlayerClearPhase(void)
{
	if (PlayerManager::GetInstance().IsPlayersEnd())
	{
		ChangePhase(PHASE::CLEAR_PHASE);
	}
}
