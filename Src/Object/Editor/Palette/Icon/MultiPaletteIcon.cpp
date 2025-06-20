#include "MultiPaletteIcon.h"
#include <random>
#include <algorithm>
#include "../../../../Manager/System/ResourceManager.h"
#include "../../../../Manager/System/DateBank.h"
#include "../Utility/Utility.h"
#include "../PaletteCursor.h"

// ランダム生成器の初期化
static std::random_device rd;
static std::mt19937 gen(rd());

MultiPaletteIcon::MultiPaletteIcon()
{
	int i = -1;
	imgCursors_ = &i;
}

MultiPaletteIcon::~MultiPaletteIcon()
{
}

void MultiPaletteIcon::Load()
{
	//通常の読み込み
	PaletteIcon::Load();

	//リソースの追加読み込み
	ResourceManager& res = ResourceManager::GetInstance();
	imgCursors_ = res.Load(ResourceManager::SRC::CURSORS).handleIds_;

	//カーソル
	int playerNum = DateBank::GetInstance().GetPlayerNum();
	for (int i = 0; i < playerNum; i++)
	{
		cursors_.push_back(std::make_unique<PaletteCursor>(i, imgCursors_[i]));
	}
}

void MultiPaletteIcon::Init()
{
	//除外番号を除いたアイテム配列を生成
	SetExcludingItemTypeArray();

	//初期化
	for (int i = 0; i < CREATE_NUM; i++)
	{
		EditorPaletteBase::ImgInfo info;
		info.num = GetRandTypeExcluding();
		info.angle = 0.0f;
		info.rate = ICON_RATE;
		info.pos =
		{
			MULTI_ICON_POS_X + (i % DRAW_COL) * INTERVAL_X,
			ICON_POS_Y + (i / DRAW_COL) * INTERVAL_Y
		};
		info.size =
		{
			static_cast<int>(ICON_SIZE * info.rate),
			static_cast<int>(ICON_SIZE * info.rate)
		};
		icons_.push_back(info);
	}

	//人数分回す
	for (auto& cursor : cursors_)
	{
		//カーソルを初期化
		cursor->Init();	
		
		//初期選択アイテム
		selectTypes_.push_back(ItemBase::ITEM_TYPE::NONE);

		//初期選択カウント
		sleCnt_.push_back(-1);
	}

	//マスクスクリーンの初期設定
	InitMaskScreen();

	//状態変更
	ChangeState(STATE::NONE);
}

void MultiPaletteIcon::Draw()
{
	//通常の描画
	PaletteIcon::Draw();

	//カーソルの描画
	for (auto& cursor : cursors_)
	{
		cursor->Draw();
	}
}

void MultiPaletteIcon::SetExcludingItemTypeArray()
{
	//除外番号
	const std::vector<int> EXCLUDED_TYPES = {
		static_cast<int>(ItemBase::ITEM_TYPE::NONE),
		static_cast<int>(ItemBase::ITEM_TYPE::START),
		static_cast<int>(ItemBase::ITEM_TYPE::GOAL),
		static_cast<int>(ItemBase::ITEM_TYPE::MAX)
	};

	for (int i = 0; i <= static_cast<int>(ItemBase::ITEM_TYPE::MAX); ++i)
	{
		if (std::find(EXCLUDED_TYPES.begin(), EXCLUDED_TYPES.end(), i) == EXCLUDED_TYPES.end())
		{
			candidates_.push_back(i);
		}
	}
}

int MultiPaletteIcon::GetRandTypeExcluding()
{
	// 候補がない場合はエラー（-1 を返す）
	if (candidates_.empty())
	{
		return -1;
	}

	// ランダムにインデックスを選ぶ
	std::uniform_int_distribution<int> dist(0, static_cast<int>(candidates_.size()) - 1);

	return candidates_[dist(gen)];
}

void MultiPaletteIcon::UpdateSelect()
{		
	//カーソル更新処理
	for (auto& cursor : cursors_)
	{
		cursor->Update();
	}

	//すべてのプレイヤーが選び終えたか調べる
	bool isAllFinish = true;
	
	//カーソル分回す
	for (int i = 0; i < cursors_.size(); i++)
	{
		//選択済みのプレイヤーは処理を飛ばす
		if (selectTypes_[i] != ItemBase::ITEM_TYPE::NONE) 
		{
			continue;
		}

		//カーソルが決定済みの時
		if (cursors_[i]->IsDecide())
		{
			//アイコン位置か確認
			if (!CheckItemIcon(cursors_[i]->GetPos(), i))
			{
				//カーソルを非決定状態に戻す
				cursors_[i]->SetDecide(false);
				isAllFinish = false;
			}
			else
			{
				continue;
			}
			
		}
		//一人でもまだ決まっていないとき判定はfalseを返す
		isAllFinish = false;
	}

	//全員選び終えたら
	if (isAllFinish)
	{
		//生成判定を返す
		isCreate_ = true;
	}

}

bool MultiPaletteIcon::CheckItemIcon(const Vector2 _cPos, const int _playerIndex)
{
	Vector2 leftTop = {};		//画像左上
	Vector2 rightBotm = {};		//画像右下

	//選択タイプのパックアップ

	//アイテムの種類の確認
	for (int i = 0; i < icons_.size(); i++)
	{
		EditorPaletteBase::ImgInfo& ic = icons_[i];
		leftTop = { ic.pos.x - ic.size.x / 2, ic.pos.y - ic.size.y / 2 };
		rightBotm = { ic.pos.x + ic.size.x / 2, ic.pos.y + ic.size.y / 2 };
		//位置の確認
		if (Utility::IsPointInRect(_cPos, leftTop, rightBotm) && !IsChosenByOtherPlayer(i, _playerIndex))
		{
			selectTypes_[_playerIndex] = static_cast<ItemBase::ITEM_TYPE>(ic.num);
			sleCnt_[_playerIndex] = i;
			return true;
		}
	}

	return false;
}

bool MultiPaletteIcon::IsChosenByOtherPlayer(const int _iconIndex, const int _playerIndex)
{
	//他のプレイヤーが既に選択していないか調べる
	for (int i = 0; i < sleCnt_.size(); i++)
	{
		if (sleCnt_[i] == _iconIndex&& i != _playerIndex)
		{
			return true;	//既に選択済み
		}
	}
	return false;
}
