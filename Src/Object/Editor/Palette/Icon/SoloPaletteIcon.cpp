#include "SoloPaletteIcon.h"
#include "../../../../Manager/System/InputManager.h"
#include "../../../../Manager/Game/ItemManager.h"
#include "../../../../Utility/Utility.h"

SoloPaletteIcon::SoloPaletteIcon()
{
}

SoloPaletteIcon::~SoloPaletteIcon()
{
}

void SoloPaletteIcon::Init()
{
	//管理させるアイテムアイコンを設定
	itemIconMap_ = 
	{
		{ItemBase::ITEM_TYPE::FLOOR ,3},
		{ItemBase::ITEM_TYPE::MOVE_HORI_FLOOR ,3},
		{ItemBase::ITEM_TYPE::MOVE_VER_FLOOR ,3}
	};

	//初期化
	for (int i = 0; i < itemIconMap_.size(); i++)
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

void SoloPaletteIcon::CheckItemIcon(const Vector2 _mPos)
{
	Vector2 leftTop = {};		//画像左上
	Vector2 rightBotm = {};		//画像右下

	//選択タイプのパックアップ
	ItemBase::ITEM_TYPE preType = selectType_;

	//アイテムの種類の確認
	for (int i = 0; i < itemIconMap_.size(); i++)
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
		selectType_ == preType  &&
		itemIconMap_[selectType_] > 0) //1クリックで生成するのを防ぐ
	{
		//生成開始する
		isCreate_ = true;
		//残量を減らす
		itemIconMap_[selectType_]--;
	}
}

void SoloPaletteIcon::AssignType()
{
	//種類を割り与える
	int index = 0;
	for (auto it = itemIconMap_.begin(); it != itemIconMap_.end(); ++it)
	{
		//割り当て
		icons_[index].num = static_cast<int>(it->first);

		//インデックスを増加
		index++;
	}
}
