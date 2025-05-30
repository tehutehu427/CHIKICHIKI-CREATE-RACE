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
#include "../Object/System/ModeSelect.h"

SelectScene::SelectScene(void)
{
	//更新関数のセット
	updataFunc_ = std::bind(&SelectScene::LoadingUpdate, this);
	//描画関数のセット
	drawFunc_ = std::bind(&SelectScene::LoadingDraw, this);

	modeSelect_ = nullptr;
}

SelectScene::~SelectScene(void)
{
}

void SelectScene::Load(void)
{
	//フォントの登録
	buttnFontHandle_ = CreateFontToHandle(FontRegistry::DOT.c_str(), FONT_SIZE, 0);

	modeSelect_ = std::make_unique<ModeSelect>();
	modeSelect_->Load();
}

void SelectScene::Init(void)
{
	modeSelect_->Init();
}

void SelectScene::NormalUpdate(void)
{
	modeSelect_->Update();
}

void SelectScene::NormalDraw(void)
{
#ifdef _DEBUG
	DebugDraw();
#endif

	modeSelect_->Draw();
}

void SelectScene::ChangeNormal(void)
{
	//処理変更
	updataFunc_ = std::bind(&SelectScene::NormalUpdate, this);
	drawFunc_ = std::bind(&SelectScene::NormalDraw, this);
}

void SelectScene::DebugUpdate()
{
}

void SelectScene::DebugDraw()
{
}