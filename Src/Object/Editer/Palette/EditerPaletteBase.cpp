#include <DxLib.h>
#include "../../../Manager/System/ResourceManager.h"
#include "EditerPaletteBase.h"

EditerPaletteBase::EditerPaletteBase()
{
	stateChanges_.emplace(STATE::NONE, std::bind(&EditerPaletteBase::ChangeStateNone, this));
	stateChanges_.emplace(STATE::CLOSE, std::bind(&EditerPaletteBase::ChangeStateClose, this));
	stateChanges_.emplace(STATE::OPEN, std::bind(&EditerPaletteBase::ChangeStateOpen, this));
	stateChanges_.emplace(STATE::OPEN, std::bind(&EditerPaletteBase::ChangeStateSelect, this));

	pal_ = {};
	state_ = STATE::NONE;
}

EditerPaletteBase::~EditerPaletteBase()
{
}

void EditerPaletteBase::Load()
{
}

void EditerPaletteBase::Init()
{
	//初期化
	pal_.pos = { CLOSE_POS_X,CLOSE_POS_Y };
	pal_.size = { PALETTE_SIZE_X, PALETTE_SIZE_Y };

	//初期状態
	ChangeState(STATE::CLOSE);
}

void EditerPaletteBase::Update()
{
	stateUpdate_();
}

void EditerPaletteBase::Draw()
{
	//描画処理
	//DrawExtendGraph();
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
	//パレットをクリックしたら
	//OPENへ
}

void EditerPaletteBase::UpdateClose()
{
	//パレットを閉じる処理
	//閉じた後はNONEへ
}

void EditerPaletteBase::UpdateOpen()
{
	//パレットを開ける処理
	//開けた後はSELECTへ
}

void EditerPaletteBase::UpdateSelect()
{
	//使うアイテムを選ぶ処理
	//アイテムをクリックしたときそのアイテムのアイコンを発光させる
	//使うアイテムをダブルクリックしたらパレットを閉じる
	//パレット外をクリックしたときパレットを閉じる
}
