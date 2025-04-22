#include <DxLib.h>
#include "../../../Manager/System/ResourceManager.h"
#include "../../../Manager/System/InputManager.h"
#include "../../../Utility/Utility.h"
#include "EditerPaletteBase.h"
#include "PaletteIcon.h"

EditerPaletteBase::EditerPaletteBase()
{
	stateChanges_.emplace(STATE::NONE, std::bind(&EditerPaletteBase::ChangeStateNone, this));
	stateChanges_.emplace(STATE::WAIT, std::bind(&EditerPaletteBase::ChangeStateWait, this));
	stateChanges_.emplace(STATE::CLOSE, std::bind(&EditerPaletteBase::ChangeStateClose, this));
	stateChanges_.emplace(STATE::OPEN, std::bind(&EditerPaletteBase::ChangeStateOpen, this));
	stateChanges_.emplace(STATE::SELECT, std::bind(&EditerPaletteBase::ChangeStateSelect, this));

	imgPalette_ = -1;
	pal_ = {};
	palIcon_ = nullptr;
	state_ = STATE::NONE;
}

EditerPaletteBase::~EditerPaletteBase()
{
}

void EditerPaletteBase::Load()
{
	//リソースの読み込み
	ResourceManager& res = ResourceManager::GetInstance();
	imgPalette_ = res.Load(ResourceManager::SRC::PALETTE).handleId_;

	//パレットアイコン
	palIcon_ = std::make_unique<PaletteIcon>();
	palIcon_->Load();
}

void EditerPaletteBase::Init()
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

void EditerPaletteBase::Update()
{
	stateUpdate_();
}

void EditerPaletteBase::Draw()
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

void EditerPaletteBase::DebagDraw()
{
#ifdef _DEBUG
	Vector2 leftTop = { pal_.pos.x - pal_.size.x / 2, pal_.pos.y - pal_.size.y / 2 };
	Vector2 rightBotm = { pal_.pos.x + pal_.size.x / 2, pal_.pos.y + pal_.size.y / 2 };

	DrawBox(
		leftTop.x,
		leftTop.y,
		rightBotm.x,
		rightBotm.y,
		0x00ffff,
		false
	);
#endif 
}

void EditerPaletteBase::ChangeState(const STATE _state)
{
	//同じ状態だったら処理しない
	if (state_ == _state)return;

	//状態変更
	state_ = _state;

	//各状態への初期処理
	stateChanges_[state_]();
}

void EditerPaletteBase::ChangeStateNone()
{
	stateUpdate_ = std::bind(&EditerPaletteBase::UpdateNone, this);
}

void EditerPaletteBase::ChangeStateWait()
{
	stateUpdate_ = std::bind(&EditerPaletteBase::UpdateWait, this);
}

void EditerPaletteBase::ChangeStateClose()
{
	stateUpdate_ = std::bind(&EditerPaletteBase::UpdateClose, this);

	//パレットアイコンの状態をNONEにする
	palIcon_->ChangeState(PaletteIcon::STATE::NONE);
}

void EditerPaletteBase::ChangeStateOpen()
{
	stateUpdate_ = std::bind(&EditerPaletteBase::UpdateOpen, this);
}

void EditerPaletteBase::ChangeStateSelect()
{
	stateUpdate_ = std::bind(&EditerPaletteBase::UpdateSelect, this);

	//パレットアイコンの状態をSELECTにする
	palIcon_->ChangeState(PaletteIcon::STATE::SELCT);
}

void EditerPaletteBase::UpdateNone()
{
}

void EditerPaletteBase::UpdateWait()
{
	InputManager& ins = InputManager::GetInstance();
	Vector2 leftTop = { pal_.pos.x - pal_.size.x / 2, pal_.pos.y - pal_.size.y / 2 };
	Vector2 rightBotm = { pal_.pos.x + pal_.size.x / 2, pal_.pos.y + pal_.size.y / 2 };

	if(ins.IsClickMouseLeft() &&
		Utility::IsPointInRect(ins.GetMousePos(), leftTop, rightBotm))
	{
		ChangeState(STATE::OPEN);
	}
}

void EditerPaletteBase::UpdateClose()
{
	pal_.pos.x += PALETTE_MOVE;

	//定位置に来たら
	if (pal_.pos.x >= CLOSE_POS_X)
	{
		ChangeState(STATE::WAIT);
	}
}

void EditerPaletteBase::UpdateOpen()
{
	pal_.pos.x -= PALETTE_MOVE;

	//中心に来たら
	if (pal_.pos.x <= OPEN_POS_X)
	{
		ChangeState(STATE::SELECT);
	}
}

void EditerPaletteBase::UpdateSelect()
{	
	InputManager& ins = InputManager::GetInstance();
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
}