#include "SelectScene.h"
#include <string>
#include <DxLib.h>
#include <cmath>
#include "../Application.h"
#include "../Common/FontRegistry.h"
#include "../Utility/Utility.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/System/ResourceManager.h"
#include "../Manager/System/InputManager.h"
#include "../Object/System/Select/ModeSelect.h"
#include "../Object/System/Select/MultiReady.h"
#include "../Object/System/Select/SelectStage.h"

SelectScene::SelectScene()
{
	//更新関数のセット
	func_.updataFunc_ = std::bind(&SelectScene::LoadingUpdate, this);

	//描画関数のセット
	func_.drawFunc_ = std::bind(&SelectScene::LoadingDraw, this);

	//状態別処理の登録
	RegisterProcessFunc(STATE::CHECK_PLAYER, ProcessFunction{ [&]() { UpdateCheckPlayer(); },  [&]() { DrawCheckPlayer(); } });
	RegisterProcessFunc(STATE::SELECT_MENU, ProcessFunction{ [&]() { UpdateSelectMenu(); },  [&]() { DrawSelectMenu(); } });
	RegisterProcessFunc(STATE::SELECT_STAGE, ProcessFunction{ [&]() { UpdateSelectStage(); },  [&]() { DrawSelectStage(); } });
	RegisterProcessFunc(STATE::SETTING, ProcessFunction{ [&]() { UpdateSetting(); },  [&]() { DrawSetting(); } });

	//選択したメニューごとの処理をセット
	menuFuncTable_ = {
		{SELECT_MENU::SOLO,[this]()
		{
			ChangeState(STATE::SELECT_STAGE);
		}},
		{SELECT_MENU::MULTI,[this]()
		{
			ChangeState(STATE::CHECK_PLAYER);
		}},
		{SELECT_MENU::FREE,[this]()
		{
			scnMng_.ChangeScene(SceneManager::SCENE_ID::FREE);
		}},
		{SELECT_MENU::SETTING,[this]()
		{
			ChangeState(STATE::SETTING);
		}},
		{SELECT_MENU::EXIT,[this]()
		{
			scnMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
		}}
	};

	state_ = STATE::SELECT_MENU;
	modeSelect_ = nullptr;
	multiReady_ = nullptr;
	selectStage_ = nullptr;
}

SelectScene::~SelectScene()
{
	DeleteFontToHandle(buttnFontHandle_);
}

void SelectScene::Load()
{
	//フォントの登録
	buttnFontHandle_ = CreateFontToHandle(FontRegistry::DOT.c_str(), FONT_SIZE, 0);

	modeSelect_ = std::make_unique<ModeSelect>();
	modeSelect_->Load();

	multiReady_ = std::make_unique<MultiReady>();
	multiReady_->Load();

	selectStage_ = std::make_unique<SelectStage>();
	selectStage_->Load();
}

void SelectScene::Init(void)
{
	modeSelect_->Init();
	multiReady_->Init();
	selectStage_->Init();
}

void SelectScene::ProcessMenuFunction(const SELECT_MENU _menu)
{
	//選択したメニューの処理を実行
	auto it = menuFuncTable_.find(_menu);
	if (it != menuFuncTable_.end())
	{
		it->second();
	}
}

void SelectScene::RegisterProcessFunc(const STATE _state, ProcessFunction _funcs)
{
	funcTable_[_state] = _funcs;
}

void SelectScene::NormalUpdate()
{
	funcTable_[state_].updataFunc_();
}

void SelectScene::UpdateSelectMenu()
{	
	modeSelect_->Update(*this);
}

void SelectScene::UpdateSelectStage()
{
	selectStage_->Update(*this);
}

void SelectScene::UpdateCheckPlayer()
{
	multiReady_->Update(*this);
}

void SelectScene::UpdateSetting()
{

}

void SelectScene::DrawSelectMenu()
{
	modeSelect_->Draw();
}

void SelectScene::DrawSelectStage()
{
	selectStage_->Draw();
}

void SelectScene::DrawCheckPlayer()
{
	multiReady_->Draw();
}

void SelectScene::DrawSetting()
{
}

void SelectScene::NormalDraw()
{
#ifdef _DEBUG
	DebugDraw();
#endif

	funcTable_[state_].drawFunc_();
}

void SelectScene::ChangeNormal()
{
	//処理変更
	func_.updataFunc_ = std::bind(&SelectScene::NormalUpdate, this);
	func_.drawFunc_ = std::bind(&SelectScene::NormalDraw, this);
}

void SelectScene::DebugUpdate()
{
}

void SelectScene::DebugDraw()
{
}