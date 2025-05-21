#include "FreePaletteIcon.h"
#include "../../../../Utility/Utility.h"

FreePaletteIcon::FreePaletteIcon()
{
}

FreePaletteIcon::~FreePaletteIcon()
{
}

void FreePaletteIcon::Init()
{
	//初期化
	for (int i = 0; i < ICON_NUM; i++)
	{
		EditorPaletteBase::ImgInfo info;
		info.num = 0;
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

	//下のほうのみ角度を変える
	scrIcon_[1].angle = Utility::Deg2RadF(SCR_ICON_DEG);

	//スクロール制限初期値
	scrLimitLine_ = 0;

	//タイプの割り当て
	AssignType();

	//マスクスクリーンの初期設定
	InitMaskScreen();

	//状態変更
	ChangeState(STATE::NONE);
}

void FreePaletteIcon::DrawItemIcon()
{
	//アイコンの描画のみマスク処理
	SetUseMaskScreenFlag(true);
	for (EditorPaletteBase::ImgInfo& i : icons_)
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
}

void FreePaletteIcon::CheckItemIcon(const Vector2 _mPos)
{
	Vector2 leftTop = {};		//画像左上
	Vector2 rightBotm = {};		//画像右下

	//選択タイプのパックアップ
	ItemBase::ITEM_TYPE preType = selectType_;

	//アイテムの種類の確認
	for (int i = 0; i < ICON_NUM; i++)
	{
		EditorPaletteBase::ImgInfo& ic = icons_[i];
		leftTop = { ic.pos.x - ic.size.x / 2, ic.pos.y - ic.size.y / 2 };
		rightBotm = { ic.pos.x + ic.size.x / 2, ic.pos.y + ic.size.y / 2 };
		//位置の確認
		if (Utility::IsPointInRect(_mPos, leftTop, rightBotm))
		{
			selectType_ = static_cast<ItemBase::ITEM_TYPE>(ic.num);
			sleCnt_ = i;
		}
	}

	//選択したアイテムをクリックしたとき
	EditorPaletteBase::ImgInfo& ic = icons_[sleCnt_];
	leftTop = { ic.pos.x - ic.size.x / 2, ic.pos.y - ic.size.y / 2 };
	rightBotm = { ic.pos.x + ic.size.x / 2, ic.pos.y + ic.size.y / 2 };
	if (Utility::IsPointInRect(_mPos, leftTop, rightBotm) &&
		selectType_ == preType) //1クリックで生成するのを防ぐ
	{
		//生成開始する
		isCreate_ = true;
	}
}

void FreePaletteIcon::AssignType()
{
	//種類を割り与える
	for (int i = 0; i < icons_.size(); i++)
	{
		icons_[i].num = i + EXCLUSION;

		//用意するアイコン分設定できたら
		if (i - 1 == ICON_NUM)
		{
			break;	//for文を抜ける
		}
	}
}