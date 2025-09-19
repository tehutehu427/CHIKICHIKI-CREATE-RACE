#include "GameClear.h"
#include <DxLib.h>
#include "../../Application.h"
#include "../../Common/IntVector3.h"
#include "../../Common/FontRegistry.h"
#include "../../Manager/System/SceneManager.h"
#include "../../Manager/System/InputManager.h"
#include "../../Manager/System/SoundManager.h"
#include "../../Manager/System/ResourceManager.h"
#include "../../Manager/Game/ScoreManager.h"
#include "../../Scene/Game/GameScene.h"
#include "../../Utility/Utility.h"

GameClear::GameClear():
	scnMng_(SceneManager::GetInstance()),
	keyConfig_(KeyConfig::GetInstance()),
	sndMng_(SoundManager::GetInstance())
{
	//状態別ファンクション処理の初期化と登録
	stateMap_.clear();
	RegisterStateFunction(STATE::WAITING, [&](GameScene& _parent) { UpdateWaiting(_parent); }, [&]() { DrawWaiting(); });
	RegisterStateFunction(STATE::DISPLAY_MESSAGE, [&](GameScene& _parent) { UpdateDisplay(_parent); }, [&]() { DrawDisplay(); });
	RegisterStateFunction(STATE::MENU, [&](GameScene& _parent) { UpdateMenu(_parent); }, [&]() { DrawMenu(); });

	//初期化
	int i = -1;
	state_ = STATE::MAX;
	messageFont_ = -1;
	menuFont_ = -1;
	waitStep_ = 0.0f;
	imgClear_ = -1;
	imgOver_ = -1;
	imgWin_ = -1;
	imgWaitDrawUi_ = -1;
	imgPlayerPlates_ = &i;
	imgSelectMenu_ = &i;

	//メニュー項目別受付処理
	menuFuncTabe_ =
	{
		{MENU::RETRY,[this](GameScene& _parent)
		{
			//モード毎に状態を初期化
			InitStateByMode();

			//リセット
			_parent.Reset();
		}},
		{MENU::BACK_SELECT,[this](GameScene& _parent)
		{
			//セレクトシーンへ遷移
			scnMng_.ChangeScene(SceneManager::SCENE_ID::SELECT);
		}},
		{MENU::BACK_TITLE,[this](GameScene& _parent)
		{
			//タイトルシーンへ遷移
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

	ResourceManager& res = ResourceManager::GetInstance();
	imgPlayerPlates_ = res.Load(ResourceManager::SRC::PLAYER_PLATES).handleIds_;
	imgSelectMenu_ = res.Load(ResourceManager::SRC::CLEAR_MENUS).handleIds_;
	imgDraw_ = res.Load(ResourceManager::SRC::DRAW).handleId_;
	imgWin_ = res.Load(ResourceManager::SRC::WIN).handleId_;
	imgClear_ = res.Load(ResourceManager::SRC::CLEAR).handleId_;
	imgOver_ = res.Load(ResourceManager::SRC::GAMEOVER).handleId_;

	sndMng_.LoadResource(SoundManager::SRC::CANCEL);
	sndMng_.LoadResource(SoundManager::SRC::DECISION);
	sndMng_.LoadResource(SoundManager::SRC::CLICK_OBJECT_SE);
	sndMng_.LoadResource(SoundManager::SRC::GAME_OVER);
	sndMng_.LoadResource(SoundManager::SRC::CLEAR);
}

void GameClear::Init()
{
	//待ち時間設定
	waitStep_ = 0.0f;

	//クリアuiの初期位置設定
	clearPos_ = UI_FIRST_POS;

	//モード毎に状態を初期化
	InitStateByMode();
}

void GameClear::Update(GameScene& _parent)
{
	//状態別更新処理
	stateMap_[state_].updateFunc(_parent);
}

void GameClear::Draw()
{
#ifdef _DEBUG
	DebugDraw();
#endif

	//画面のフェード
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_);
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, Utility::BLACK, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	
	//状態別描画処理
	stateMap_[state_].drawFunc();
}

void GameClear::SetGameResultPhase(const bool _typs)
{
	if (_typs)
	{
		imgWaitDrawUi_ = imgClear_;
		sndMng_.Play(SoundManager::SRC::CLEAR, SoundManager::PLAYTYPE::BACK);
		return;
	}
	else
	{
		imgWaitDrawUi_ = imgOver_;
		sndMng_.Play(SoundManager::SRC::GAME_OVER, SoundManager::PLAYTYPE::BACK);
		return;
	}
}

void GameClear::RegisterStateFunction(const STATE _state, std::function<void(GameScene&)> _update, std::function<void()> _draw)
{
	stateMap_[_state] = StateFuncs{ _update, _draw };
}

void GameClear::UpdateWaiting(GameScene& _parent)
{	
	KeyConfig& ins = KeyConfig::GetInstance();

	//画面を暗くする
	alpha_ += ALPHA_SPEED;
	if (alpha_ >= BLACK_BOX_ALPHA) { alpha_ = BLACK_BOX_ALPHA; }
	
	//UIアニメーション(下から出す)
	constexpr float END = (float)Application::SCREEN_HALF_Y;
	waitStep_ += SceneManager::GetInstance().GetDeltaTime();	//アニメーション用ステップ
	clearPos_.y = static_cast<int>(Utility::EaseInOutBack(waitStep_, ANIM_TIME, static_cast<float>(UI_FIRST_POS.y), END));

	//スキップ
	if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
	{
		waitStep_ = CHANGE_TIME_WAITING;
	}

	if (waitStep_ >= CHANGE_TIME_WAITING)
	{
		//タイム設定
		waitStep_ = 0.0f;

		//状態変更
		ChangeState(STATE::MENU);
	}
}

void GameClear::UpdateDisplay(GameScene& _parent)
{
	KeyConfig& ins = KeyConfig::GetInstance();

	//ステップ更新
	waitStep_ += SceneManager::GetInstance().GetDeltaTime();

	//スキップ
	if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
	{
		waitStep_ = CHANGE_TIME_WAITING;
	}

	if (waitStep_ >= CHANGE_TIME_WAITING)
	{
		//タイム設定
		waitStep_ = 0.0f;

		//状態変更
		ChangeState(STATE::MENU);
	}
}

void GameClear::UpdateMenu(GameScene& _parent)
{
	KeyConfig& ins = KeyConfig::GetInstance();

	//メニューを選択させる
	if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_UP,KeyConfig::JOYPAD_NO::PAD1))
	{
		//選択メニューの更新
		menuIndex_ = (menuIndex_ - 1 + MENU_LIST_NUM) % MENU_LIST_NUM;
		sndMng_.Play(SoundManager::SRC::CLICK_OBJECT_SE, SoundManager::PLAYTYPE::BACK);
		return;
	}
	else if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_DOWN, KeyConfig::JOYPAD_NO::PAD1))
	{
		//選択メニューの更新		
		menuIndex_ = (menuIndex_ + 1) % MENU_LIST_NUM;
		sndMng_.Play(SoundManager::SRC::CLICK_OBJECT_SE, SoundManager::PLAYTYPE::BACK);
		return;
	}

	//決定処理
	else if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
	{
		//選択したメニュー項目の処理に移る
		menuFuncTabe_[static_cast<MENU>(menuIndex_)](_parent);

		//アルファ値初期化
		alpha_ = 0;

		sndMng_.Play(SoundManager::SRC::DECISION, SoundManager::PLAYTYPE::BACK);
		return;
	}
}

void GameClear::DrawWaiting()
{
	//「クリア」の描画
	DrawRotaGraph(
		clearPos_.x,
		clearPos_.y,
		DEFAULT_UI_RATE,
		0.0f,
		imgWaitDrawUi_,
		true,
		false
	);
}

void GameClear::DrawDisplay()
{
	auto & score =  ScoreManager::GetInstance();	//UI用インデックス
	constexpr int POS_X = Application::SCREEN_HALF_X;	//X座標共通
	constexpr int WINNER_POS_Y = 250;	//勝者UIのY座標
	constexpr int WIN_POS_Y = 400;		//WINのY座標


	if (score.IsDraw())
	{
		//引き分けの描画
		DrawRotaGraph(
			POS_X,
			Application::SCREEN_HALF_Y,
			1.0f,
			0.0f,
			imgDraw_,
			true,
			false
		);
	}
	else
	{
		const int index = score.GetNowWinnerPlayerIndex();

		//勝者の描画
		DrawRotaGraph(
			POS_X,
			WINNER_POS_Y,
			1.0f,
			0.0f,
			imgPlayerPlates_[index],
			true,
			false
		);

		//winの描画
		DrawRotaGraph(
			POS_X,
			WIN_POS_Y,
			DEFAULT_UI_RATE,
			0.0f,
			imgWin_,
			true,
			false
		);
	}
}

void GameClear::DrawMenu()
{
	constexpr int MARGIN_Y = 200;	//余白
	constexpr int INTERVAL = 90;	//間隔
	constexpr IntVector3 SELECT_ADD_COLOR = { 128,0,0 };	//選択色

	for (int i = 0; i < STATE_MAX; i++)
	{
		//デフォルト座標
		Vector2 pos{
		Application::SCREEN_HALF_X,
		Application::SCREEN_HALF_Y  - MARGIN_Y + i * MARGIN_Y };
		
		//画像の色を変える
		if (i == menuIndex_)
		{
			SetDrawAddColor(SELECT_ADD_COLOR.x, SELECT_ADD_COLOR.y, SELECT_ADD_COLOR.z);
		}	
		
		//画像
		DrawRotaGraph(
			pos.x,
			pos.y,
			DEFAULT_UI_RATE,
			0.0f,
			imgSelectMenu_[i],
			true,
			false
		);

		//色を戻す
		SetDrawAddColor(0, 0, 0);
	}
}

void GameClear::InitStateByMode()
{
	if (SceneManager::GetInstance().GetSceneID() == SceneManager::SCENE_ID::MULTI)
	{
		ChangeState(STATE::DISPLAY_MESSAGE);
	}
	else
	{
		ChangeState(STATE::WAITING);
	}
}

void GameClear::DebugDraw()
{

}