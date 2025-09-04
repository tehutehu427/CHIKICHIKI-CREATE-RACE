#include <DxLib.h>
#include <cmath>
#include "../../Application.h"
#include "../Common/FontRegistry.h"
#include "../../Utility/Utility.h"
#include "../../Manager/System/SceneManager.h"
#include "../../Manager/System/ResourceManager.h"
#include "../../Manager/System/SoundManager.h"
#include "../../Manager/System/Camera.h"
#include "../../Manager/System/InputManager.h"
#include "../../Manager/System/DateBank.h"
#include "../../Manager/Game/CollisionManager.h"
#include "../../Manager/Game/ItemManager.h"
#include "../../Manager/Game/MapEditer.h"
#include "../../Manager/Game/GravityManager.h"
#include "../../Manager/Game/PlayerManager.h"
#include "../../Object/Editor/Palette/EditorPaletteBase.h"
#include "../../Object/Editor/EditController.h"
#include "../../Object/Editor/MapDataIO.h"
#include "../../Object/Grid.h"
#include "../../Object/SkyDome/SkyDome.h"
#include "../../Object/System/GameClear.h"
#include "GameScene.h"

GameScene::GameScene(void)
{
	SetMouseDispFlag(false);	//マウスカーソルを非表示にする
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
	SetMouseDispFlag(true);	//マウスカーソルを非表示にする
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

	//ローディング中のリソースを取得
	LoadRandomLoadingMessage();

	//プレイヤー管理クラス
	PlayerManager::CreateInstance();
	PlayerManager::GetInstance().Load();

	//マップエディター
	MapEditer::CreateInstance();

	//重力管理クラスを生成
	GravityManager::CreateInstance();
	
	//アイテムマネージャーの生成
	ItemManager::CreateInstance();
	for (int i = 0; i < playerNum; i++)
	{
		//エディットコントローラーの生成
		editControllers_.push_back(std::make_unique<EditController>(i));
		//グリッド表示をするかを初期化
		isGrid_.push_back(true);
	}

	//スカイドームの生成
	sky_ = std::make_unique<SkyDome>();
	sky_->Load();

	//ゲームクリアの生成
	gameClear_ = std::make_unique<GameClear>();
	gameClear_->Load();	

	//マップデータの入出力
	mapIO_ = std::make_unique<MapDataIO>(editControllers_[0]->GetCursorPos());
	mapIO_->Load();

	//音源の読み込み
	LoadSound();
}

void GameScene::Init(void)
{
	//各クラスの初期化
	for (auto& controller : editControllers_) { controller->Init(); }
	sky_->Init();
	gameClear_->Init();
	actionStartTime_ = 0.0f;
}

void GameScene::NormalUpdate(void)
{
	//フェーズ別の処理
	phaseUpdate_();
	//終了した当たり判定の消去
	CollisionManager::GetInstance().Sweep();

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
	if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::DEBUG_CHANGE_TITLE, KeyConfig::JOYPAD_NO::PAD1))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

	//フェーズ遷移は各アップデートに作ったのでここは消し
	else if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::DEBUG_CHANGE_CLEAR, KeyConfig::JOYPAD_NO::PAD1))
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

void GameScene::Reset()
{
	//アクションを最初から遊ぶ
	ChangePhase(PHASE::ACTION_PHASE);
}

void GameScene::ChangePhaseEdit(void)
{
	phaseUpdate_ = std::bind(&GameScene::UpdateEdit, this);
	phaseDraw_ = std::bind(&GameScene::DrawEdit, this);
	int playerNum = DateBank::GetInstance().GetPlayerNum();
	for (int i = 0; i < playerNum; i++)
	{
		auto camera = SceneManager::GetInstance().GetCamera(i).lock();
		camera->ChangeMode(Camera::MODE::FREE_CONTROLL);
		VECTOR pos;
		IntVector3 mPos = MapEditer::MAP_SIZE;
		//pos = { static_cast<float>(mPos.x * MapEditer::GRID_SIZE) / 2,static_cast<float>(mPos.y * MapEditer::GRID_SIZE) / 2,static_cast<float>(mPos.z * MapEditer::GRID_SIZE) / 2 };
		pos = { 0.0f,400.0f,-700.0f };
		camera->SetPos(pos);

		if (playerNum == 1)
		{
			editControllers_[i]->Reset();
		}
	}

	//アイテムの値リセット
	ItemManager::GetInstance().ResetItemValue();

	//アイテムのエフェクトを停止する
	//ItemManager::GetInstance().ItemEffectStop();
}

void GameScene::ChangePhaseAction(void)
{

	phaseUpdate_ = std::bind(&GameScene::UpdateAction, this);
	phaseDraw_ = std::bind(&GameScene::DrawAction, this);

	//プレイヤー初期化
	PlayerManager::GetInstance().Init();

	//プレイヤーにスタートオブジェクトにする
	PlayerManager::GetInstance().SetInitPos(ItemManager::GetInstance().GetStartWorldPos());

	//アイテムの値リセット
	ItemManager::GetInstance().ResetItemValue();

	for (int i = 0; i < DateBank::GetInstance().GetPlayerNum(); i++)
	{
		//カメラ
		auto camera = SceneManager::GetInstance().GetCamera(i).lock();
		//カメラをフォローモードにチェンジ
		camera->ChangeMode(Camera::MODE::FOLLOW);
		//カメラの追従対象をプレイヤーに設定
		camera->SetFollow(&PlayerManager::GetInstance().GetPlayerTransform(i));

		//ゴール座標
		VECTOR goalPos = ItemManager::GetInstance().GetGoalObjectPos();
		//ゴールまでのベクトル
		VECTOR vec = Utility::GetMoveVec(PlayerManager::GetInstance().GetPlayerTransform(i).pos, goalPos);
		//ベクトルをアングルに変換
		camera->SetAngles(Quaternion::AngleAxis(atan2(vec.x, vec.z), Utility::AXIS_Y).ToEuler());
	}
	
	actionStartTime_ = ACTION_START_TIME;

	//クリアの初期化
	gameClear_->Init();
}

void GameScene::ChangePhaseClear(void)
{
	phaseUpdate_ = std::bind(&GameScene::UpdateClear, this);
	phaseDraw_ = std::bind(&GameScene::DrawClear, this);
}

void GameScene::UpdateEdit(void)
{
	//パレット
	if (mapIO_->IsEdit()) { palette_->Update(); }
	for (int i = 0; i < DateBank::GetInstance().GetPlayerNum(); i++)
	{
		KeyConfig& ins = KeyConfig::GetInstance();
		auto keyType = DateBank::GetInstance().GetPlayerNum() == 1 ? KeyConfig::TYPE::ALL : KeyConfig::TYPE::PAD;
		if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::EDIT_GRID_ON_OFF, static_cast<KeyConfig::JOYPAD_NO>(i + 1), keyType))
		{
			isGrid_[i] = isGrid_[i] ? false : true;
		}
	}

	if (palette_->GetState() == EditorPaletteBase::STATE::WAIT && 
		mapIO_->IsEdit())
	{
		for (auto& controller : editControllers_) { controller->Update(); }
	}
	else
	{
		for (auto& controller : editControllers_) 
		{ 
			controller->UpdateCursor(); 
			controller->UpdateError();
		}
	}
}

void GameScene::UpdateAction(void)
{
	int beforTime = static_cast<int>(std::floor(actionStartTime_));
	actionStartTime_ -= SceneManager::GetInstance().GetDeltaTime();
	int afterTime = static_cast<int>(std::floor(actionStartTime_));
	if (afterTime >= 0)
	{
		if (beforTime != afterTime)
		{
			SoundManager::GetInstance().Play(SoundManager::SRC::COUNTDOWN_SE, SoundManager::PLAYTYPE::BACK);
		}
		return;
	}

	if (beforTime != afterTime && afterTime >= -1)
	{
		SoundManager::GetInstance().Play(SoundManager::SRC::CHICKEN_SE, SoundManager::PLAYTYPE::BACK);
	}

	ItemManager::GetInstance().Update();

	//プレイヤーの更新
	PlayerManager::GetInstance().Update();

	//終了した当たり判定の消去
	CollisionManager::GetInstance().Sweep();

	//更新はアクション中のみ
	CollisionManager::GetInstance().Update();

	CheckPlayerFinish();
}

void GameScene::UpdateClear(void)
{
	//プレイヤーにアニメーションをさせたりする
	//エフェクトなどを表示させる

	gameClear_->Update(*this);
}

void GameScene::DrawEdit(void)
{
	auto screenIndex = SceneManager::GetInstance().GetScreenIndex();

	if (isGrid_[screenIndex])
	{
		//グリッド
		grid_->Draw();
	}
	//エディットコントローラー
	//for (auto& controller : editControllers_) 
	//{ 
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
	auto screenIndex = SceneManager::GetInstance().GetScreenIndex();
	//アイテム
	ItemManager::GetInstance().Draw();
	//プレイヤー
	PlayerManager::GetInstance().Draw();

	if (PlayerManager::GetInstance().IsPlayerDeath(screenIndex))
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 64);
		DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, Utility::BLACK, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	if (actionStartTime_ < 0)
	{
		return;
	}
	int playerNum = DateBank::GetInstance().GetPlayerNum();
	int screenX = playerNum == 1 ? Application::SCREEN_SIZE_X : Application::SCREEN_HALF_X;
	int screenY = playerNum > 2 ? Application::SCREEN_HALF_Y : Application::SCREEN_SIZE_Y;
	int numHandle = ResourceManager::GetInstance().Load(ResourceManager::SRC::NUMBERS).handleIds_[static_cast<int>(actionStartTime_) + 1];
	DrawRotaGraph(screenX / 2, screenY / 2, 1.0f, 0.0f, numHandle, true);
	
}

void GameScene::DrawClear()
{
	//プレイ画面を背景に描画
	DrawAction();

	//ゲームクリアの描画
	gameClear_->Draw();
}

void GameScene::LoadSound(void)
{
	sndMng_.LoadResource(SoundManager::SRC::CHICKEN_SE);
	sndMng_.LoadResource(SoundManager::SRC::COUNTDOWN_SE);
	sndMng_.LoadResource(SoundManager::SRC::OK);
	sndMng_.LoadResource(SoundManager::SRC::CANCEL);
	sndMng_.LoadResource(SoundManager::SRC::BOMB_SE);
}

void GameScene::CheckPlayerFinish(void)
{
	if (PlayerManager::GetInstance().IsPlayerGoal(0))
	{
		//フェーズ設定
		gameClear_->SetGameResultPhase(true);

		//フェーズ遷移
		ChangePhase(PHASE::CLEAR_PHASE);

		return;
	}
	else if (PlayerManager::GetInstance().IsPlayerDeath(0))
	{
		//フェーズ設定
		gameClear_->SetGameResultPhase(false);

		//フェーズ遷移
		ChangePhase(PHASE::CLEAR_PHASE);

		return;
	}
}