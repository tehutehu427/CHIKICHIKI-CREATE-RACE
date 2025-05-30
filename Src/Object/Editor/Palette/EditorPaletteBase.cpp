#include <DxLib.h>
#include "../../../Manager/System/SceneManager.h"
#include "../../../Manager/System/Camera.h"
#include "../../../Manager/System/ResourceManager.h"
#include "../../../Manager/System/InputManager.h"
#include "../../../Manager/Game/ItemManager.h"
#include "../../../Manager/Game/MapEditer.h"
#include "../../../Common/IntVector3.h"
#include "../../../Utility/Utility.h"
#include "../EditController.h"
#include "EditorPaletteBase.h"
#include "Icon/PaletteIcon.h"
#include "Icon/FreePaletteIcon.h"
#include "Icon/SoloPaletteIcon.h"

EditorPaletteBase::EditorPaletteBase(EditController& _controller) 
	: ediCon_(_controller)
{
	stateChanges_.emplace(STATE::NONE, std::bind(&EditorPaletteBase::ChangeStateNone, this));
	stateChanges_.emplace(STATE::WAIT, std::bind(&EditorPaletteBase::ChangeStateWait, this));
	stateChanges_.emplace(STATE::CLOSE, std::bind(&EditorPaletteBase::ChangeStateClose, this));
	stateChanges_.emplace(STATE::OPEN, std::bind(&EditorPaletteBase::ChangeStateOpen, this));
	stateChanges_.emplace(STATE::SELECT, std::bind(&EditorPaletteBase::ChangeStateSelect, this));

	imgPalette_ = -1;
	pal_ = {};
	palIcon_ = nullptr;
	state_ = STATE::NONE;
}

EditorPaletteBase::~EditorPaletteBase()
{
}

void EditorPaletteBase::Load()
{
	//リソースの読み込み
	ResourceManager& res = ResourceManager::GetInstance();
	imgPalette_ = res.Load(ResourceManager::SRC::PALETTE).handleId_;

	//パレットアイコン
	SceneManager::SCENE_ID sceneId = SceneManager::GetInstance().GetSceneID();
	switch (sceneId)
	{
	case SceneManager::SCENE_ID::FREE:
		palIcon_ = std::make_unique<FreePaletteIcon>();
		break;
	case SceneManager::SCENE_ID::SOLO:
		palIcon_ = std::make_unique<SoloPaletteIcon>();
		break;
	default:
		break;
	}
	palIcon_->Load();
}

void EditorPaletteBase::Init()
{
	//初期化
	pal_.pos = { CLOSE_POS_X,CLOSE_POS_Y };
	pal_.rate = PALETTE_RATE;
	pal_.size = { 
		static_cast<int>(static_cast<float>(PALETTE_SIZE_X) * pal_.rate),
		static_cast<int>(static_cast<float>(PALETTE_SIZE_Y) * pal_.rate) };

	palIcon_->Init();

	//初期状態
	ChangeState(STATE::WAIT);
}

void EditorPaletteBase::Update()
{
	stateUpdate_();
}

void EditorPaletteBase::Draw()
{
	//描画処理
	DrawRotaGraph(
		pal_.pos.x,
		pal_.pos.y,
		pal_.rate,
		pal_.angle,
		imgPalette_,
		true,
		false);

	//選択描画
	palIcon_->Draw();
}

void EditorPaletteBase::DebagDraw()
{
#ifdef _DEBUG
	Vector2 leftTop = { pal_.pos.x - pal_.size.x / 2, pal_.pos.y - pal_.size.y / 2 };
	Vector2 rightBotm = { pal_.pos.x + pal_.size.x / 2, pal_.pos.y + pal_.size.y / 2 };

	DrawBox(
		leftTop.x,
		leftTop.y,
		rightBotm.x,
		rightBotm.y,
		Utility::RED,
		false
	);

	palIcon_->DebagDraw();
#endif 
}

void EditorPaletteBase::ChangeState(const STATE _state)
{
	//同じ状態だったら処理しない
	if (state_ == _state)return;

	//状態変更
	state_ = _state;

	//各状態への初期処理
	stateChanges_[state_]();
}

void EditorPaletteBase::ChangeStateNone()
{
	stateUpdate_ = std::bind(&EditorPaletteBase::UpdateNone, this);
}

void EditorPaletteBase::ChangeStateWait()
{
	stateUpdate_ = std::bind(&EditorPaletteBase::UpdateWait, this);
}

void EditorPaletteBase::ChangeStateClose()
{
	stateUpdate_ = std::bind(&EditorPaletteBase::UpdateClose, this);

	//パレットアイコンの状態をNONEにする
	palIcon_->ChangeState(PaletteIcon::STATE::NONE);
}

void EditorPaletteBase::ChangeStateOpen()
{
	stateUpdate_ = std::bind(&EditorPaletteBase::UpdateOpen, this);
}

void EditorPaletteBase::ChangeStateSelect()
{
	stateUpdate_ = std::bind(&EditorPaletteBase::UpdateSelect, this);

	//パレットアイコンの状態をSELECTにする
	palIcon_->ChangeState(PaletteIcon::STATE::SELCT);
}

void EditorPaletteBase::UpdateNone()
{
}

void EditorPaletteBase::UpdateWait()
{
	InputManager& ins = InputManager::GetInstance();
	Vector2 leftTop = { pal_.pos.x - pal_.size.x / 2, pal_.pos.y - pal_.size.y / 2 };
	Vector2 rightBotm = { pal_.pos.x + pal_.size.x / 2, pal_.pos.y + pal_.size.y / 2 };

	if(ins.IsTrgDownMouseLeft() &&
		Utility::IsPointInRect(ins.GetMousePos(), leftTop, rightBotm))
	{
		ChangeState(STATE::OPEN);
	}
}

void EditorPaletteBase::UpdateClose()
{
	pal_.pos.x += PALETTE_MOVE;

	//定位置に来たら
	if (pal_.pos.x >= CLOSE_POS_X)
	{
		ChangeState(STATE::WAIT);
	}
}

void EditorPaletteBase::UpdateOpen()
{
	pal_.pos.x -= PALETTE_MOVE;

	//中心に来たら
	if (pal_.pos.x <= OPEN_POS_X)
	{
		ChangeState(STATE::SELECT);
	}
}

void EditorPaletteBase::UpdateSelect()
{	
	InputManager& ins = InputManager::GetInstance();
	ItemManager & itemMng = ItemManager::GetInstance();
	auto camera = SceneManager::GetInstance().GetCamera();
	Vector2 leftTop = {};		//画像左上
	Vector2 rightBotm = {};		//画像右下

	//パレット外をクリックしたときパレットを閉じる
	leftTop = { pal_.pos.x - pal_.size.x / 2, pal_.pos.y - pal_.size.y / 2 };
	rightBotm = { pal_.pos.x + pal_.size.x / 2, pal_.pos.y + pal_.size.y / 2 };
	if (ins.IsClickMouseLeft() &&
		!Utility::IsPointInRect(ins.GetMousePos(), leftTop, rightBotm)) {
		ChangeState(STATE::CLOSE);
	}

	//パレットアイコンに関する処理
	palIcon_->Update();

	//生成開始
	if (palIcon_->IsCreate())
	{
		//コントローラに設定
		ediCon_.SetItemType(palIcon_->GetSelectType());

		//状態変更
		ChangeState(STATE::CLOSE);
		palIcon_->ChangeState(PaletteIcon::STATE::NONE);
	}
}