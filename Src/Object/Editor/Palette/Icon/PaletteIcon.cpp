#include "PaletteIcon.h"
#include <cassert>
#include <algorithm>
#include "../../../../Manager/System/ResourceManager.h"
#include "../../../../Manager/System/InputManager.h"
#include "../../../../Manager/System/DateBank.h"
#include "../../../../Utility/Utility.h"
#include "../../../../Utility/UtilityTemplates.h"
#include "../../../../Common/FontRegistry.h"


PaletteIcon::PaletteIcon()
{
	stateChanges_.emplace(STATE::NONE, std::bind(&PaletteIcon::ChangeStateNone, this));
	stateChanges_.emplace(STATE::SCR_UP, std::bind(&PaletteIcon::ChangeStateScrollUp, this));
	stateChanges_.emplace(STATE::SCR_DOWN, std::bind(&PaletteIcon::ChangeStateScrollDown, this));
	stateChanges_.emplace(STATE::SELCT, std::bind(&PaletteIcon::ChangeStateSelect, this));

	int i = -1;
	imgIcons_ = &i;
	fontHandle_ = -1;	
	imgScrIcon_ = -1;
	mskPal_ = -1;
	scrLimitLine_ = -1;	
	prePos_ = {};
	sleCnt_.clear();
	selectTypes_.clear();
	icons_.clear();
	isCreate_ = false;
	state_ = STATE::NONE;
	for (EditorPaletteBase::ImgInfo& s : scrIcon_) { s = {}; }
}

PaletteIcon::~PaletteIcon()
{
	DeleteFontToHandle(fontHandle_);
	DeleteMaskScreen();
}

void PaletteIcon::Load()
{
	//リソースの読みこみ
	ResourceManager & res = ResourceManager::GetInstance();
	imgScrIcon_ = res.Load(ResourceManager::SRC::SCROLL_ARROW_ICON).handleId_;
	imgIcons_ = res.Load(ResourceManager::SRC::ITEM_ICONS).handleIds_;
	mskPal_ = res.Load(ResourceManager::SRC::PALETTE_MASK).handleId_;

	//フォント生成
	fontHandle_ = CreateFontToHandle(FontRegistry::BOKUTATI.c_str(), NAME_FONT_SIZE, NAME_FONT_THICK);
}

void PaletteIcon::Init()
{	
	//除外番号を除いたアイテム配列を生成
	SetExcludingItemTypeArray();

	//初期化
	for (int i = 0; i < candidates_.size(); i++)
	{
		EditorPaletteBase::ImgInfo info;
		info.num = candidates_[i];
		info.angle = 0.0f;
		info.rate = ICON_RATE;
		info.pos =
		{
			ICON_POS_X + (i % COL) * INTERVAL_X,
			ICON_POS_Y + (i / COL) * INTERVAL_Y
		};
		info.size =
		{
			static_cast<int>(ICON_SIZE * info.rate),
			static_cast<int>(ICON_SIZE * info.rate)
		};
		icons_.push_back(info);
	}

	for (int i = 0; i < SCROLL_ICON_NUM; i++)
	{
		scrIcon_[i].rate = ICON_RATE;
		scrIcon_[i].pos = { SCR_ICON_POS_X, SCR_ICON_POS_Y[i] };
		scrIcon_[i].size =
		{
			static_cast<int>(SCR_ICON_SIZE * scrIcon_[i].rate),
			static_cast<int>(SCR_ICON_SIZE * scrIcon_[i].rate)
		};
	}

	//初期選択アイテム
	selectTypes_.push_back(ItemBase::ITEM_TYPE::NONE);
	sleCnt_.push_back(-1);

	//下のほうのみ角度を変える
	scrIcon_[1].angle = Utility::Deg2RadF(SCR_ICON_DEG);

	//スクロール制限初期値
	scrLimitLine_ = 0;

	//マスクスクリーンの初期設定
	InitMaskScreen();

	//状態変更
	ChangeState(STATE::NONE);
}

void PaletteIcon::Update()
{
	stateUpdate_();
}

void PaletteIcon::Draw()
{
	//一定の状態の場合描画させない
	if (state_ == STATE::NONE) { return; }

	//アイテムアイコンの描画
	DrawItemIcon();

	//スクロールアイコンの描画
	DrawScrollIcon();

	//デバッグ描画
	DebagDraw();
}

void PaletteIcon::DebagDraw()
{
#ifdef _DEBUG
	
#endif 
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
	for (int i = 0; i < icons_.size(); i++) { icons_[i].pos = { ICON_POS_X + (i % COL) * INTERVAL_X,ICON_POS_Y + (i / COL) * INTERVAL_Y }; }
	for (int i = 0; i < SCROLL_ICON_NUM; i++) { scrIcon_[i].pos = { SCR_ICON_POS_X, SCR_ICON_POS_Y[i] }; }
	isCreate_ = false;
	for (auto& s : sleCnt_) { s = -1; }

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
	for (EditorPaletteBase::ImgInfo& i : icons_)
	{
		i.pos.y -= ICONS_MOVE;
	}

	//全員に加える移動量が同じため
	//1つだけ参照する
	if (icons_[0].pos.y <= prePos_.y - INTERVAL_Y) 
	{
		ChangeState(STATE::SELCT);
	}
}

void PaletteIcon::UpdateScrollDown()
{
	for (EditorPaletteBase::ImgInfo& i : icons_)
	{
		i.pos.y += ICONS_MOVE;
	}

	//全員に加える移動量が同じため
	//1つだけ参照する
	if (icons_[0].pos.y >= prePos_.y + INTERVAL_Y) 
	{
		ChangeState(STATE::SELCT);
	}
}

void PaletteIcon::UpdateSelect()
{
	InputManager& ins = InputManager::GetInstance();

	//クリックしたか調べる
	if (ins.IsMouseTrgDown(InputManager::MOUSE::CLICK_LEFT)) {
		//マウス位置を取得
		Vector2 mousePos = ins.GetMousePos();

		//スクロールをクリックしたか調べる（クリックしてた場合処理終了）
		if (CheckScrollIcon(mousePos)) { return; }

		//アイテムアイコンをクリックしたか判定を返す
		isCreate_ = CheckItemIcon(mousePos);
	}
}

void PaletteIcon::DrawItemIcon()
{
	//アイコンの描画のみマスク処理
	SetUseMaskScreenFlag(true);
	int index = 0;
	for (EditorPaletteBase::ImgInfo& i : icons_)
	{
		//アイコン
		DrawRotaGraph(
			i.pos.x,
			i.pos.y,
			i.rate,
			i.angle,
			imgIcons_[i.num],
			true,
			false);

		int nameColor = Utility::WHITE;		//デフォルトネームカラー
		std::string name = DateBank::GetInstance().GetItemName(static_cast<ItemBase::ITEM_TYPE>(i.num));	//名前を取得	

		//選択しているアイコンの場合名前を赤にする
		if(UtilityTemplates::ContainsValue(selectTypes_, static_cast<ItemBase::ITEM_TYPE>(i.num)) &&	//配列内に条件の値がないか調べる
			UtilityTemplates::ContainsValue(sleCnt_, index))
		{
			//色を赤にする
			nameColor = Utility::RED;
		}

		//名前を描画
		int offSetX = name.size() * NAME_FONT_SIZE / 4;
		constexpr int OFFSET_Y = ICON_SIZE / 2 + 20;
		DrawFormatStringToHandle(
			i.pos.x - offSetX,
			i.pos.y + OFFSET_Y,
			nameColor,
			fontHandle_,
			name.c_str());

		//インデックス更新
		index++;
	}
	SetUseMaskScreenFlag(false);
}

void PaletteIcon::DrawScrollIcon()
{
	//スクロール用アイコン
	for (EditorPaletteBase::ImgInfo& s : scrIcon_)
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

void PaletteIcon::SetExcludingItemTypeArray()
{
	//除外番号
	const std::vector<int> EXCLUDED_TYPES = {
		static_cast<int>(ItemBase::ITEM_TYPE::NONE),
		static_cast<int>(ItemBase::ITEM_TYPE::START),
		static_cast<int>(ItemBase::ITEM_TYPE::GOAL),
		static_cast<int>(ItemBase::ITEM_TYPE::MAX),
		static_cast<int>(ItemBase::ITEM_TYPE::BOMB_BIG),
		static_cast<int>(ItemBase::ITEM_TYPE::BOMB_SMALL),
	};

	for (int i = 0; i <= static_cast<int>(ItemBase::ITEM_TYPE::MAX); ++i)
	{
		if (std::find(EXCLUDED_TYPES.begin(), EXCLUDED_TYPES.end(), i) == EXCLUDED_TYPES.end())
		{
			candidates_.push_back(i);
		}
	}
}

void PaletteIcon::InitMaskScreen()
{
	//マスクスクリーンの作成
	int ret = CreateMaskScreen();
	assert(ret != -1);

	//マスク領域の描画
	DrawMask(0, 0, mskPal_, DX_MASKTRANS_BLACK);

	//マスク無効
	SetUseMaskScreenFlag(false);
}

bool PaletteIcon::CheckScrollIcon(const Vector2 _mPos)
{
	Vector2 leftTop = {};		//画像左上
	Vector2 rightBotm = {};		//画像右下

	//アイテムパレットをスクロールする処理
	for (int i = 0; i < SCROLL_ICON_NUM; i++)
	{
		EditorPaletteBase::ImgInfo& s = scrIcon_[i];
		leftTop = { s.pos.x - s.size.x / 2, s.pos.y - s.size.y / 2 };
		rightBotm = { s.pos.x + s.size.x / 2, s.pos.y + s.size.y / 2 };
		//上
		if (Utility::IsPointInRect(_mPos, leftTop, rightBotm) &&
			scrLimitLine_ != 0 &&
			i == static_cast<int>(SCROLL::UP))
		{
			ChangeState(STATE::SCR_DOWN);
			scrLimitLine_--;
			return true;
		}
		//下
		else if (Utility::IsPointInRect(_mPos, leftTop, rightBotm) &&
			scrLimitLine_ != SCROLL_LIMIT_LINE &&
			i == static_cast<int>(SCROLL::DOWWN))
		{
			ChangeState(STATE::SCR_UP);
			scrLimitLine_++;
			return true;
		}
	}

	return false;
}

bool PaletteIcon::CheckItemIcon(const Vector2 _mPos, const int _playerIndex)
{
	Vector2 leftTop = {};		//画像左上
	Vector2 rightBotm = {};		//画像右下

	//選択タイプのパックアップ
	ItemBase::ITEM_TYPE preType = selectTypes_[_playerIndex];

	//アイテムの種類の確認
	for (int i = 0; i < icons_.size(); i++)
	{
		EditorPaletteBase::ImgInfo& ic = icons_[i];	//情報の格納
		leftTop = { ic.pos.x - ic.size.x / 2, ic.pos.y - ic.size.y / 2 };//座標定義
		rightBotm = { ic.pos.x + ic.size.x / 2, ic.pos.y + ic.size.y / 2 };
		//位置の確認
		if (Utility::IsPointInRect(_mPos, leftTop, rightBotm))
		{
			selectTypes_[_playerIndex] = static_cast<ItemBase::ITEM_TYPE>(ic.num);
			sleCnt_[_playerIndex] = i;
			break;
		}
		else
		{
			sleCnt_[_playerIndex] = -1;
		}
	}

	//選択カウントが設定されなかった場合
	if (sleCnt_[_playerIndex] == -1)
	{
		return false;
	}

	//もう一度選択したアイテムをクリックしたとき
	EditorPaletteBase::ImgInfo& ic = icons_[sleCnt_[_playerIndex]];	//情報の格納
	leftTop = { ic.pos.x - ic.size.x / 2, ic.pos.y - ic.size.y / 2 };//座標定義
	rightBotm = { ic.pos.x + ic.size.x / 2, ic.pos.y + ic.size.y / 2 };
	if (Utility::IsPointInRect(_mPos, leftTop, rightBotm) &&
		selectTypes_[_playerIndex] == preType) //1クリックで生成するのを防ぐ
	{
		//生成開始する
		return true;
	}

	return false;
}
