#include <cassert>
#include "../../../Manager/System/ResourceManager.h"
#include "../../../Manager/System/InputManager.h"
#include "../../../Utility/Utility.h"
#include "PaletteIcon.h"

PaletteIcon::PaletteIcon()
{
	stateChanges_.emplace(STATE::NONE, std::bind(&PaletteIcon::ChangeStateNone, this));
	stateChanges_.emplace(STATE::SCR_UP, std::bind(&PaletteIcon::ChangeStateScrollUp, this));
	stateChanges_.emplace(STATE::SCR_DOWN, std::bind(&PaletteIcon::ChangeStateScrollDown, this));
	stateChanges_.emplace(STATE::SELCT, std::bind(&PaletteIcon::ChangeStateSelect, this));

	state_ = STATE::NONE;
	imgScrIcon_ = -1;
	imgIcons_ = -1;
	mskPal_ = -1;
	scrLimitLine_ = -1;
	prePos_ = {};
	for (EditerPaletteBase::ImgInfo& i : icons_) { i = {}; }
	for (EditerPaletteBase::ImgInfo& s : scrIcon_) { s = {}; }
}

PaletteIcon::~PaletteIcon()
{
	DeleteMaskScreen();
}

void PaletteIcon::Load()
{
	//リソースの読みこみ
	ResourceManager & res = ResourceManager::GetInstance();
	imgScrIcon_ = res.Load(ResourceManager::SRC::SCROLL_ARROW_ICON).handleId_;
	imgIcons_ = res.Load(ResourceManager::SRC::PALETTE_ICONS).handleId_;
	mskPal_ = res.Load(ResourceManager::SRC::PALETTE_MASK).handleId_;
}

void PaletteIcon::Init()
{
	//初期化
	for (int i = 0; i < ICON_NUM; i++) {
		icons_[i].rate = ICON_RATE;
		icons_[i].pos = {
			ICON_POS_X + (i % COL) * INTERVAL_X,
			ICON_POS_Y + (i / COL) * INTERVAL_Y };
		icons_[i].size = { 
			static_cast<int>(ICON_SIZE * icons_[i].rate),
			static_cast<int>(ICON_SIZE * icons_[i].rate) };
	}

	for (int i = 0; i < SCROLL_ICON_NUM; i++) {
		scrIcon_[i].rate = ICON_RATE;
		scrIcon_[i].pos = { SCR_ICON_POS_X, SCR_ICON_POS_Y[i] };
		scrIcon_[i].size = {
			static_cast<int>(SCR_ICON_SIZE * scrIcon_[i].rate), 
			static_cast<int>(SCR_ICON_SIZE * scrIcon_[i].rate) };
	}

	//下のほうのみ角度を変える
	scrIcon_[1].angle = Utility::Deg2RadF(SCR_ICON_DEG);

	//スクロール制限初期値
	scrLimitLine_ = 0;

	//マスクスクリーンの作成
	int ret = CreateMaskScreen();
	assert(ret != -1);

	//マスク領域の描画
	DrawMask(0, 0, mskPal_, DX_MASKTRANS_BLACK);

	//マスク無効
	SetUseMaskScreenFlag(false);

	//状態変更
	ChangeState(STATE::NONE);
}

void PaletteIcon::Update()
{
	stateUpdate_();
}

void PaletteIcon::Draw()
{
	if (state_ == STATE::NONE) { return; }

	//アイコンの描画のみマスク処理
	SetUseMaskScreenFlag(true);
	for (EditerPaletteBase::ImgInfo& i : icons_)
	{
		DrawRotaGraph(
			i.pos.x,
			i.pos.y,
			i.rate,
			i.angle,
			imgIcons_,
			true,
			false);
	}
	SetUseMaskScreenFlag(false);

	//スクロール用アイコン
	for (EditerPaletteBase::ImgInfo& s : scrIcon_)
	{
		DrawRotaGraph(
			s.pos.x,
			s.pos.y,
			s.rate,
			s.angle,
			imgScrIcon_,
			true,
			false);
	}
}

void PaletteIcon::DebagDraw()
{
}

void PaletteIcon::ChangeState(const STATE _state)
{
	//同じ状態だったら処理しない
	if (state_ == _state)return;

	//状態変更
	state_ = _state;

	//各状態への初期処理
	stateChanges_[state_]();
}

void PaletteIcon::ChangeStateNone()
{
	stateUpdate_ = std::bind(&PaletteIcon::UpdateNone, this);

	//座標を初期位置に指定
	for (int i = 0; i < ICON_NUM; i++) { icons_[i].pos = { ICON_POS_X + (i % COL) * INTERVAL_X,ICON_POS_Y + (i / COL) * INTERVAL_Y }; }
	for (int i = 0; i < SCROLL_ICON_NUM; i++) { scrIcon_[i].pos = { SCR_ICON_POS_X, SCR_ICON_POS_Y[i] }; }

	//制限も初期化しておく
	scrLimitLine_ = 0;
}

void PaletteIcon::ChangeStateScrollUp()
{
	stateUpdate_ = std::bind(&PaletteIcon::UpdateScrollUp, this);

	//座標のバックアップ(１つだけ参照)
	prePos_ = icons_[0].pos;
}

void PaletteIcon::ChangeStateScrollDown()
{
	stateUpdate_ = std::bind(&PaletteIcon::UpdateScrollDown, this);

	//座標のバックアップ(１つだけ参照)
	prePos_ = icons_[0].pos;
}

void PaletteIcon::ChangeStateSelect()
{
	stateUpdate_ = std::bind(&PaletteIcon::UpdateSelect, this);
}

void PaletteIcon::UpdateNone()
{
}

void PaletteIcon::UpdateScrollUp()
{
	for (EditerPaletteBase::ImgInfo& i : icons_)
	{
		i.pos.y -= ICONS_MOVE;
	}

	//全員に加える移動量が同じため
	//1つだけ参照する
	if (icons_[0].pos.y <= prePos_.y - INTERVAL_Y) {
		ChangeState(STATE::SELCT);
	}
}

void PaletteIcon::UpdateScrollDown()
{
	for (EditerPaletteBase::ImgInfo& i : icons_)
	{
		i.pos.y += ICONS_MOVE;
	}

	//全員に加える移動量が同じため
	//1つだけ参照する
	if (icons_[0].pos.y >= prePos_.y + INTERVAL_Y) {
		ChangeState(STATE::SELCT);
	}
}

void PaletteIcon::UpdateSelect()
{
	InputManager& ins = InputManager::GetInstance();

	//クリックしたか調べる
	if (ins.IsClickMouseLeft()) {
		//特定の範囲をクリックしたか調べる
		CheackClickPosition(ins.GetMousePos());
	}
}

void PaletteIcon::CheackClickPosition(const Vector2 _mPos)
{	
	Vector2 leftTop = {};		//画像左上
	Vector2 rightBotm = {};		//画像右下

	//アイテムパレットをスクロールする処理
	for (int i = 0; i < SCROLL_ICON_NUM; i ++)
	{
		EditerPaletteBase::ImgInfo& s = scrIcon_[i];
		leftTop = { s.pos.x - s.size.x / 2, s.pos.y - s.size.y / 2 };
		rightBotm = { s.pos.x + s.size.x / 2, s.pos.y + s.size.y / 2 };
		//上
		if (Utility::IsPointInRect(_mPos, leftTop, rightBotm) &&
			scrLimitLine_ != 0 &&
			i == static_cast<int>(SCROLL::UP))
		{
			ChangeState(STATE::SCR_DOWN);
			scrLimitLine_--;
		}
		//下
		else if (Utility::IsPointInRect(_mPos, leftTop, rightBotm) &&
			scrLimitLine_ != SCROLL_LIMIT_LINE &&
			i == static_cast<int>(SCROLL::DOWWN))
		{
			ChangeState(STATE::SCR_UP);
			scrLimitLine_++;
		}
	}
}
