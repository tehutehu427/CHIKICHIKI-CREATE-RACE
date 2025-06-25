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
#include "Palette.h"
#include "Icon/PaletteIcon.h"
#include "Icon/MultiPaletteIcon.h"

EditorPaletteBase::EditorPaletteBase(std::vector<std::unique_ptr<EditController>>& _editControllers)
	: editControllers_(_editControllers)
{
	stateChanges_.emplace(STATE::NONE, std::bind(&EditorPaletteBase::ChangeStateNone, this));
	stateChanges_.emplace(STATE::WAIT, std::bind(&EditorPaletteBase::ChangeStateWait, this));
	stateChanges_.emplace(STATE::SELECT, std::bind(&EditorPaletteBase::ChangeStateSelect, this));
	stateChanges_.emplace(STATE::PALETTE_ANIM, std::bind(&EditorPaletteBase::ChangeStatePaletteAnim, this));

	imgPalette_ = -1;
	pal_ = nullptr;
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
	palIcon_ = std::make_unique<PaletteIcon>();
	palIcon_->Load();

	//パレット
	pal_ = std::make_unique<Palette>();
	pal_->Load();
}

void EditorPaletteBase::Init()
{
	//パレット
	pal_->Init();
	pal_->ChangeState(Palette::STATE::EDGE);

	//アイコン
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
	//パレット
	pal_->Draw();

	//アイコン
	palIcon_->Draw();
}

void EditorPaletteBase::DebagDraw()
{
#ifdef _DEBUG
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

void EditorPaletteBase::ChangeStateSelect()
{
	stateUpdate_ = std::bind(&EditorPaletteBase::UpdateSelect, this);

	//パレットアイコンの状態をSELECTにする
	palIcon_->ChangeState(PaletteIcon::STATE::SELCT);
}

void EditorPaletteBase::ChangeStatePaletteAnim()
{
	stateUpdate_ = std::bind(&EditorPaletteBase::UpdatePaletteAnim, this);

	//アイコンの状態変更
	palIcon_->ChangeState(PaletteIcon::STATE::NONE);
}

void EditorPaletteBase::UpdateNone()
{
	//処理はなし
}

void EditorPaletteBase::UpdateWait()
{
	KeyConfig& ins = KeyConfig::GetInstance();
	
	if(ins.IsTrgDown(KeyConfig::CONTROL_TYPE::PALETTE_CURSOR_SELECT,KeyConfig::JOYPAD_NO::PAD1) &&
		pal_->IsInsidePalette(ins.GetMousePos()))
	{
		//状態遷移
		ChangeState(STATE::PALETTE_ANIM);
		
		//パレットの状態遷移
		pal_->ChangeState(Palette::STATE::ADMISSION_EDGE);
	}
}

void EditorPaletteBase::UpdateSelect()
{	
	KeyConfig& ins = KeyConfig::GetInstance();

	//パレット外をクリックしたときパレットを閉じる
	if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::PALETTE_CURSOR_SELECT, KeyConfig::JOYPAD_NO::PAD1) &&
		!pal_->IsInsidePalette(ins.GetMousePos()))
	{
		//状態遷移
		ChangeState(STATE::PALETTE_ANIM);

		//パレットの状態遷移
		pal_->ChangeState(Palette::STATE::EXIT_EDGE);

		//アイコンの状態変更
		palIcon_->ChangeState(PaletteIcon::STATE::NONE);

		//処理終了
		return;
	}

	//パレットアイコンに関する処理
	palIcon_->Update();

	//生成開始
	if (palIcon_->IsCreate())
	{
		//コントローラに設定(ソロ)
		editControllers_[0]->SetItemType(palIcon_->GetSelectType());

		//状態変更
		ChangeState(STATE::PALETTE_ANIM);

		//パレットの状態変更
		pal_->ChangeState(Palette::STATE::EXIT_EDGE);

		//アイコンの状態変更
		palIcon_->ChangeState(PaletteIcon::STATE::NONE);
	}
}

void EditorPaletteBase::UpdatePaletteAnim()
{
	//前状態を保持
	Palette::STATE preState = pal_->GetState();

	//更新処理
	pal_->Update();

	//更新後の状態と前の状態を比較して異なるとき
	if (pal_->GetState() != preState)
	{
		//前の状態ごとに状態遷移
		switch (preState)
		{
		case Palette::STATE::EXPANSION:
		case Palette::STATE::ADMISSION_EDGE:
			ChangeState(STATE::SELECT);
			break;

		case Palette::STATE::EXIT_EDGE:
			ChangeState(STATE::WAIT);
			break;

		case Palette::STATE::REDUCTION:
			ChangeState(STATE::NONE);

		default:
			break;
		}
	}
}
