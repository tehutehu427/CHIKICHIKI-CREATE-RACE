#include <DxLib.h>
#include "../../../Manager/System/ResourceManager.h"
#include "../../../Manager/System/InputManager.h"
#include "../../../Utility/Utility.h"
#include "EditerPaletteBase.h"

EditerPaletteBase::EditerPaletteBase()
{
	stateChanges_.emplace(STATE::NONE, std::bind(&EditerPaletteBase::ChangeStateNone, this));
	stateChanges_.emplace(STATE::WAIT, std::bind(&EditerPaletteBase::ChangeStateWait, this));
	stateChanges_.emplace(STATE::CLOSE, std::bind(&EditerPaletteBase::ChangeStateClose, this));
	stateChanges_.emplace(STATE::OPEN, std::bind(&EditerPaletteBase::ChangeStateOpen, this));
	stateChanges_.emplace(STATE::SELECT, std::bind(&EditerPaletteBase::ChangeStateSelect, this));

	pal_ = {};
	imgScrIcon_ = -1;
	imgIcons_ = -1;
	ic_ = {};
	//for (Icon& i : icons_) { i = {}; }
	state_ = STATE::NONE;
}

EditerPaletteBase::~EditerPaletteBase()
{
}

void EditerPaletteBase::Load()
{
	ResourceManager& res = ResourceManager::GetInstance();
	pal_.img = res.Load(ResourceManager::SRC::PALETTE).handleId_;
	imgScrIcon_ = res.Load(ResourceManager::SRC::SCROLL_ARROW_ICON).handleId_;
	imgIcons_ = res.Load(ResourceManager::SRC::PALETTE_ICONS).handleId_;
}

void EditerPaletteBase::Init()
{
	//初期化
	pal_.pos = { CLOSE_POS_X,CLOSE_POS_Y };
	pal_.rate = PALETTE_RATE;
	pal_.size = { 
		static_cast<int>(static_cast<float>(PALETTE_SIZE_X) * pal_.rate),
		static_cast<int>(static_cast<float>(PALETTE_SIZE_Y) * pal_.rate) };

	ic_.pos = {
		Application::SCREEN_HALF_X,
		Application::SCREEN_HALF_Y
	};
	ic_.rate = ICON_RATE;

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
		0.0f,
		pal_.img,
		true,
		false);

	if (state_ == STATE::SELECT) {
		DrawRotaGraph(
			ic_.pos.x,
			ic_.pos.y,
			ic_.rate,
			0.0f,
			imgIcons_,
			true,
			false
		);
	}
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
}

void EditerPaletteBase::ChangeStateOpen()
{
	stateUpdate_ = std::bind(&EditerPaletteBase::UpdateOpen, this);
}

void EditerPaletteBase::ChangeStateSelect()
{
	stateUpdate_ = std::bind(&EditerPaletteBase::UpdateSelect, this);
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
	//使うアイテムを選ぶ処理
	//アイテムをクリックしたときそのアイテムのアイコンを発光させる
	//使うアイテムをダブルクリックしたらパレットを閉じる
	
	//パレット外をクリックしたときパレットを閉じる
	InputManager& ins = InputManager::GetInstance();
	Vector2 leftTop = { pal_.pos.x - pal_.size.x / 2, pal_.pos.y - pal_.size.y / 2 };
	Vector2 rightBotm = { pal_.pos.x + pal_.size.x / 2, pal_.pos.y + pal_.size.y / 2 };

	if (ins.IsClickMouseLeft() &&
		!Utility::IsPointInRect(ins.GetMousePos(), leftTop, rightBotm))
	{
		ChangeState(STATE::CLOSE);
	}
}
