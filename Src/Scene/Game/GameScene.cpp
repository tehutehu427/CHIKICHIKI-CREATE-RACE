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
#include "../../Object/Editor/MapDataIO.h"
#include "../../Object/Grid.h"
#include "../../Object/SkyDome/SkyDome.h"
#include "../../Object/System/GameClear.h"
#include "../../Object/UI/EditorUi.h"
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
	ItemManager::GetInstance().Destroy();
	MapEditer::GetInstance().Destroy();
	GravityManager::Destroy();
	DeleteFontToHandle(buttnFontHandle_);
	phaseChanges_.clear();

}

void GameScene::Load(void)
{
	//フォントの生成
	buttnFontHandle_ = CreateFontToHandle(FontRegistry::DOT.c_str(), FONT_SIZE, 0);

	PlayerManager::CreateInstance(PLAYER_NUM);
	PlayerManager::GetInstance().Load();

	MapEditer::CreateInstance();

	GravityManager::CreateInstance();
	//player_ = std::make_unique<Player>();
	//player_->Load();

	//for (int i = 0; i < DateBank::GetInstance().GetPlayerNum();i++)
	//{
	// 
	//	editController_ = std::make_shared<EditController>(i);
	//}	
	
	//アイテムマネージャーの生成
	ItemManager::CreateInstance();
	
	//エディットコントローラーの生成
	editControllers_.push_back(std::make_unique<EditController>(0));

	//パレットの生成
	palette_ = std::make_unique<EditorPaletteBase>(editControllers_);
	palette_->Load();

	//スカイドームの生成
	sky_ = std::make_unique<SkyDome>();
	sky_->Load();

	//ゲームクリアの生成
	gameClear_ = std::make_unique<GameClear>();
	gameClear_->Load();	

	//マップデータの入出力
	mapIO_ = std::make_unique<MapDataIO>();
	mapIO_->Load();

	//エディターモード
	editorUi_ = std::make_unique<EditorUi>();
	editorUi_->Load();
}

void GameScene::Init(void)
{
	//初期化
	palette_->Init();
	for (auto& controller : editControllers_) { controller->Init(); }
	sky_->Init();
	gameClear_->Init();
	editorUi_->Init();

	

	//アイテム生成
	//ItemManager::GetInstance().AddItem({ 0,0,0 }, Quaternion(), ItemBase::ITEM_TYPE::CANNON);
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

	//フェーズの描画
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
	if (ins.IsTrgDown(KEY_INPUT_RSHIFT))
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
	if (!ItemManager::GetInstance().AllDummyItemAddItems())
	{
		return;
	}

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
	//pos = { 0.0f,250.0f,-500.0f };
	SceneManager::GetInstance().GetCamera().lock()->SetPos(pos);
}

void GameScene::ChangePhaseAction(void)
{
	phaseUpdate_ = std::bind(&GameScene::UpdateAction, this);
	phaseDraw_ = std::bind(&GameScene::DrawAction, this);

	SceneManager::GetInstance().GetCamera().lock()->ChangeMode(Camera::MODE::SELF_SHOT);
	SceneManager::GetInstance().GetCamera().lock()->SetFollow(&PlayerManager::GetInstance().GetPlayer(0).GetTransform());
	PlayerManager::GetInstance().SetInitPos(ItemManager::GetInstance().GetStartWorldPos());
	PlayerManager::GetInstance().Init();
	//VECTOR pos;
	//IntVector3 mPos = MapEditer::MAP_SIZE;
	//pos = { static_cast<float>(mPos.x * MapEditer::GRID_SIZE) / 2,static_cast<float>(mPos.y * MapEditer::GRID_SIZE) * 8.5f,static_cast<float>(mPos.z * MapEditer::GRID_SIZE) / 2 };
	//SceneManager::GetInstance().GetCamera().lock()->SetPos(pos);
	//VECTOR angles = {};
	//angles.x = Utility::Deg2RadF(90.0);
	//SceneManager::GetInstance().GetCamera().lock()->SetAngles(angles);

	VECTOR pos = ACTION_CAMERA_POS;
	SceneManager::GetInstance().GetCamera().lock()->SetPos(pos);
	VECTOR angles = {};
	angles = Quaternion::FromToRotation(pos, Camera::FIXED_DIAGONAL_TARGET_POS).ToEuler();
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
	for (auto& controller : editControllers_) { controller->Update(); }
}

void GameScene::UpdateAction(void)
{
	ItemManager::GetInstance().Update();
	PlayerManager::GetInstance().Update();
	ChangePlayerClearPhase();
	Collision();


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
	for (auto& controller : editControllers_) { controller->Draw(); }

	//アイテム
	ItemManager::GetInstance().Draw();
	
	//パレット
	palette_->Draw();

	//エディターモード用のUI
	editorUi_->Draw();
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

void GameScene::Collision(void)
{
	PlayerManager& pMng = PlayerManager::GetInstance();

	for (auto& player : pMng.GetPlayers())
	{
		CheckHitCol(*player);
	}
}

void GameScene::CheckHitCol(Player& _player)
{
	VECTOR pMovedPos = _player.GetMovedPos();
	IntVector3 mapPos = MapEditer::GetInstance().WorldToMapPos(pMovedPos);
	for (int x = -Player::COL_RANGE; x <= Player::COL_RANGE; x++)
	{
		for (int y = -Player::COL_RANGE; y <= Player::COL_RANGE; y++)
		{
			for (int z = -Player::COL_RANGE; z <= Player::COL_RANGE; z++)
			{
				IntVector3 colPos = mapPos + IntVector3{ x, y, z };
				if (colPos.x < 0 || colPos.y < 0 || colPos.z < 0)continue;
				HitPlayerAndItem(_player,colPos);
			}
		}
	}
	itemLPos_.clear();
}

void GameScene::HitPlayerAndItem(Player& _player, const IntVector3 _colPos)
{
	MapEditer& mapEdit = MapEditer::GetInstance();
	ItemManager& itemMng = ItemManager::GetInstance();
	if (mapEdit.IsObjectAtMapPos(_colPos))
	{
		IntVector3 lPos = mapEdit.GetLeaderMapPos(_colPos);
		for (auto& iLPos : itemLPos_)
		{
			if (iLPos == lPos)return;
		}


		//アイテムタイプ取得
		ItemBase::ITEM_TYPE type = mapEdit.GetItemType(_colPos);


		//アイテムのTransform取得
		Transform itemTrans = itemMng.GetItemTransform(lPos, type);

		PlayerUpColl(_player,itemTrans);
		//ArroundColl(itemTrans);

		itemLPos_.push_back(lPos);
	}
}

void GameScene::PlayerUpColl(Player& _player,Transform _itemTrans)
{
	//移動後と移動前をとる(高速で落ちているときの当たり判定)
	VECTOR pMovedPos = _player.GetMovedPos();
	VECTOR prePos = _player.GetTransform().pos;
	VECTOR curPos = pMovedPos;

	VECTOR vec = VSub(curPos, prePos);

	col_ = ITEM_COL_INFO::NONE;

	//アイテムモデルとプレイヤーモデルの当たり判定
	auto hit = MV1CollCheck_Line(_itemTrans.modelId, -1, prePos, curPos);

	MapEditer& mapEdit = MapEditer::GetInstance();

	//当たったら
	if (hit.HitFlag > 0)
	{
		if (pMovedPos.y > hit.HitPosition.y)
		{
			col_ = ITEM_COL_INFO::UP;
		}
		else
		{
			col_ = ITEM_COL_INFO::DOWN;
		}
		return;
	}


	//プレイヤーの通常時(低速落下などの時)
	//Lineを引くための上と下の座標をとる
	VECTOR upPos = curPos;
	upPos.y += (Player::RADIUS);
	VECTOR downPos = curPos;
	downPos.y -= (Player::RADIUS + 10.0f);

	hit = MV1CollCheck_Line(_itemTrans.modelId, -1, upPos, downPos);

	//当たったら
	if (hit.HitFlag > 0)
	{
		if (pMovedPos.y > hit.HitPosition.y)
		{
			col_ = ITEM_COL_INFO::UP;
		}
		else
		{
			col_ = ITEM_COL_INFO::DOWN;
		}
	}
}

bool GameScene::ArroundColl(Player& _player,Transform _itemTrans)
{
	return false;
}


