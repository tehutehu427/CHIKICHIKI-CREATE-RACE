#include "MultiPaletteIcon.h"
#include <random>
#include <algorithm>
#include <unordered_set>
#include "../../../../Manager/System/ResourceManager.h"
#include "../../../../Manager/System/DateBank.h"
#include "../../../../Manager/System/SoundManager.h"
#include "../Utility/Utility.h"
#include "../Utility/UtilityTemplates.h"
#include "../PaletteCursor.h"

// ランダム生成器の初期化
static std::random_device rd;
static std::mt19937 gen(rd());

MultiPaletteIcon::MultiPaletteIcon(void)
{
	imgCursors_ = nullptr;
	isSkips_.clear();
	cursors_.clear();
}

MultiPaletteIcon::~MultiPaletteIcon(void)
{
}

void MultiPaletteIcon::Load(void)
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
		cursors_.push_back(std::move(std::make_unique<PaletteCursor>(i, imgCursors_[i])));

		//スキップも設定
		isSkips_.push_back(false);
	}
}

void MultiPaletteIcon::Init(void)
{		
	
	//状態変更
	ChangeState(STATE::NONE);

	//初期化
	icons_.clear();
	selectTypes_.clear();
	sleCnt_.clear();
	candidates_.clear();
	
	//除外番号を除いたアイテム配列を生成
	SetExcludingItemTypeArray();

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

	//初期化
	for (auto isSkip : isSkips_)
	{
		isSkip = false;
	}
}

void MultiPaletteIcon::Draw(void)
{
	//通常の描画
	PaletteIcon::Draw();

	//カーソルの描画
	for (auto& cursor : cursors_)
	{
		cursor->Draw();
	}
}

void MultiPaletteIcon::SetExcludingItemTypeArray(void)
{

	const std::unordered_map<ItemBase::ITEM_TYPE, int> ITEM_WEIGHTS =
	{
	{ ItemBase::ITEM_TYPE::BOMB_SMALL, ITEM_WEIGHT }, // 出やすい
	// 他のアイテムも必要に応じて追加
	};

	// 除外するアイテム（NONE, START, GOALなど）
	const std::unordered_set<ItemBase::ITEM_TYPE> EXCLUDED_TYPES = 
	{
		ItemBase::ITEM_TYPE::NONE,
		ItemBase::ITEM_TYPE::START,
		ItemBase::ITEM_TYPE::GOAL,
		ItemBase::ITEM_TYPE::BOMB_BIG,
		ItemBase::ITEM_TYPE::MAX
	};

	for (int i = 0; i < static_cast<int>(ItemBase::ITEM_TYPE::MAX); ++i)
	{
		ItemBase::ITEM_TYPE type = static_cast<ItemBase::ITEM_TYPE>(i);

		if (EXCLUDED_TYPES.count(type) > 0)
		{
			continue;
		}

		// 重みが指定されているか確認
		auto it = ITEM_WEIGHTS.find(type);

		//重みを設定(通常は1)
		int weight = (it != ITEM_WEIGHTS.end()) ? it->second : 1;

		for (int j = 0; j < weight; ++j)
		{
			candidates_.push_back(i);
		}
	}
}

int MultiPaletteIcon::GetRandTypeExcluding(void)
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

void MultiPaletteIcon::UpdateSelect(void)
{		
	//カーソル更新処理
	for (auto& cursor : cursors_)
	{
		cursor->Update();
	}

	//すべてのプレイヤーが選び終えたか調べる
	bool isAllFinish = true;
	
	//カーソル分回す
	for (int i = 0; i < static_cast<int>(cursors_.size()); i++)
	{
		//選択済みのプレイヤーは処理を飛ばす
		if (selectTypes_[i] != ItemBase::ITEM_TYPE::NONE || isSkips_[i])
		{

			continue;
		}

		if (DateBank::GetInstance().IsItemSetSkip() && 
			KeyConfig::GetInstance().IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_SKIP, static_cast<KeyConfig::JOYPAD_NO>(i + 1)))
		{
			selectTypes_[i] = ItemBase::ITEM_TYPE::NONE;	//NONEにする
			cursors_[i]->SetDecide(true);					//カーソルを決定済みにする
			isSkips_[i] = true;								//スキップする
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
	for (int i = 0; i < static_cast<int>(icons_.size()); i++)
	{
		EditorPaletteBase::ImgInfo& ic = icons_[i];
		leftTop = { ic.pos.x - ic.size.x / 2, ic.pos.y - ic.size.y / 2 };
		rightBotm = { ic.pos.x + ic.size.x / 2, ic.pos.y + ic.size.y / 2 };
		//位置の確認
		if (Utility::IsPointInRect(_cPos, leftTop, rightBotm) && !IsChosenByOtherPlayer(i, _playerIndex))
		{
			SoundManager::GetInstance().Play(SoundManager::SRC::DECISION, SoundManager::PLAYTYPE::BACK);	//選択音を鳴らす
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

void MultiPaletteIcon::DrawItemIcon(void)
{
	//アイコンの描画のみマスク処理
	SetUseMaskScreenFlag(true);
	int index = 0;
	for (EditorPaletteBase::ImgInfo& i : icons_)
	{
		//既に誰かが選択済みの場合描画しない
		if (IsChosenByOtherPlayer(index, -1))	//全てのプレイやーを調べるため-1を設定
		{
			index++;
			continue;
		}

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
		if (UtilityTemplates::ContainsValue(selectTypes_, static_cast<ItemBase::ITEM_TYPE>(i.num)) &&	//配列内に条件の値がないか調べる
			UtilityTemplates::ContainsValue(sleCnt_, index))
		{
			//色を赤にする
			nameColor = Utility::RED;
		}

		//名前を描画
		int offSetX = static_cast<int>(name.size()) * NAME_FONT_SIZE / 4;
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
