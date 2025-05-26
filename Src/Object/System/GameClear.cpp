#include "GameClear.h"
#include <DxLib.h>
#include "../../Application.h"
#include "../../Common/FontRegistry.h"
#include "../../Manager/System/SceneManager.h"
#include "../../Manager/System/InputManager.h"
#include "../../Utility/Utility.h"

namespace
{
	Vector2 defaultPos = { Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y };
}

GameClear::GameClear():
	scnMng_(SceneManager::GetInstance()),
	input_(InputManager::GetInstance())
{
	//状態別ファンクション処理の初期化と登録
	stateMap_.clear();
	RegisterStateFunction(STATE::WAITING, [&]() { UpdateWaiting(); }, [&]() { DrawWaiting(); });
	RegisterStateFunction(STATE::MENU, [&]() { UpdateMenu(); }, [&]() { DrawMenu(); });

	//初期化
	state_ = STATE::NONE;
	messageFont_ = -1;
	menuFont_ = -1;
	waitStep_ = 0.0f;

	//メニュー項目別受付処理
	menuFuncTabe_ =
	{
		{MENU::RETRY,[this]()
		{
			scnMng_.ChangeScene(scnMng_.GetSceneID());
		}},
		{MENU::BACK_SELECT,[this]()
		{
			scnMng_.ChangeScene(SceneManager::SCENE_ID::SELECT);
		}},
		{MENU::BACK_TITLE,[this]()
		{
			scnMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
		}}
	};

	//メニュー項目
	menuStrings_[static_cast<int>(MENU::RETRY)] = "リトライ";
	menuStrings_[static_cast<int>(MENU::BACK_SELECT)] = "セレクトへ戻る";
	menuStrings_[static_cast<int>(MENU::BACK_TITLE)] = "タイトルに戻る";
}

GameClear::~GameClear()
{
	DeleteFontToHandle(messageFont_);
	DeleteFontToHandle(menuFont_);
}

void GameClear::Load()
{
	//フォント生成
	messageFont_ = CreateFontToHandle(FontRegistry::HANAZOME.c_str(), MES_FONT_SIZE, MES_FONT_THICK);
	menuFont_ = CreateFontToHandle(FontRegistry::HANAZOME.c_str(), MENU_FONT_SIZE, MENU_FONT_THICK);
}

void GameClear::Init()
{
	//待ち時間設定
	waitStep_ = CHANGE_TIME_WAITING;

	//初期状態
	ChangeState(STATE::WAITING);
}

void GameClear::Update()
{
	//状態別更新処理
	stateMap_[state_].updateFunc();
}

void GameClear::Draw()
{
#ifdef _DEBUG
	DebugDraw();
#endif
	
	//状態別描画処理
	stateMap_[state_].drawFunc();
}

void GameClear::RegisterStateFunction(const STATE _state, std::function<void()> _update, std::function<void()> _draw)
{
	stateMap_[_state] = StateFuncs{ _update, _draw };
}

void GameClear::UpdateWaiting()
{
	InputManager& ins = InputManager::GetInstance();

	//ステップ更新
	waitStep_ -= SceneManager::GetInstance().GetDeltaTime();
	
	//スキップ
	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_RETURN))
	{
		waitStep_ = 0.0f;	//ステップを条件値に設定
		//他は効果音を停止など
	}

	if (waitStep_ <= 0.0f)
	{
		//タイム設定
		waitStep_ = CHANGE_TIME_WAITING;

		//状態変更
		ChangeState(STATE::MENU);
	}
}

void GameClear::UpdateMenu()
{
	InputManager& ins = InputManager::GetInstance();

	//メニューを選択させる
	if (ins.IsTrgDown(KEY_INPUT_UP))
	{
		//選択メニューの更新
		menuIndex_ = (menuIndex_ - 1 + MENU_LIST_NUM) % MENU_LIST_NUM;
	}
	else if (ins.IsTrgDown(KEY_INPUT_DOWN))
	{
		//選択メニューの更新		
		menuIndex_ = (menuIndex_ + 1) % MENU_LIST_NUM;
	}

	//決定処理
	if (ins.IsTrgDown(KEY_INPUT_RETURN))
	{
		//選択したメニュー項目の処理に移る
		menuFuncTabe_[static_cast<MENU>(menuIndex_)]();
	}
}

void GameClear::DrawWaiting()
{
	std::string mes = "おめでとう";
	Vector2 pos{ 
		MESSAGE_POS_X + Application::SCREEN_HALF_X - static_cast<int>(mes.size() * MES_FONT_SIZE / 2),
		MESSAGE_POS_Y + Application::SCREEN_HALF_Y};	//座標

	//クリアメッセージの描画
	DrawFormatStringToHandle(
		pos.x,
		pos.y,
		Utility::WHITE,
		messageFont_,
		mes.c_str());
}

void GameClear::DrawMenu()
{
	int color = Utility::WHITE;		//デフォルトカラー
	int index = 0;
	constexpr int MARGIN_Y = 50;	//余白
	constexpr int ARROW_OFFSET_X = -40;
	constexpr int SHADOW_OFFSET = 2;

	for (auto menuString : menuStrings_)
	{
		//デフォルト座標
		Vector2 pos{
		MENU_POS_X + Application::SCREEN_HALF_X - static_cast<int>(menuString.size() * MES_FONT_SIZE / 4),
		MENU_POS_Y + Application::SCREEN_HALF_Y };
		
		//文字の色を変える
		int color = Utility::WHITE;		//デフォルトカラー
		if (index == menuIndex_)
		{
			color = Utility::RED;
			
			//影
			DrawFormatStringToHandle(
				pos.x + ARROW_OFFSET_X + SHADOW_OFFSET,
				pos.y + MARGIN_Y * index + SHADOW_OFFSET,
				Utility::BLACK,
				menuFont_,
				"⇒");
			//矢印を描画させる
			DrawFormatStringToHandle(
				pos.x + ARROW_OFFSET_X,
				pos.y + MARGIN_Y * index,
				Utility::PINK,
				menuFont_,
				"⇒");
		}	
		
		//影
		DrawFormatStringToHandle(
			pos.x + SHADOW_OFFSET,
			pos.y + MARGIN_Y * index + SHADOW_OFFSET,
			Utility::BLACK,
			menuFont_,
			menuString.c_str());

		//メニュー
		DrawFormatStringToHandle(
			pos.x,
			pos.y + MARGIN_Y * index,
			color, 
			menuFont_,
			menuString.c_str());

		//インデックス更新
		index++;
	}
}

void GameClear::DebugDraw()
{
	constexpr int MARGIN = 100;
	constexpr float THICK = 20.0f;

	DrawBoxAA(
		MARGIN,
		MARGIN,
		Application::SCREEN_SIZE_X - MARGIN,
		Application::SCREEN_SIZE_Y - MARGIN,
		Utility::CYAN,
		true);

	DrawBoxAA(
		MARGIN,
		MARGIN,
		Application::SCREEN_SIZE_X - MARGIN,
		Application::SCREEN_SIZE_Y - MARGIN,
		Utility::BLUE,
		false,
		THICK);
}
