#include <string>
#include <DxLib.h>
#include "../Common/FontRegistry.h"
#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/System/ResourceManager.h"
#include "../Manager/System/InputManager.h"
#include "TitleScene.h"

TitleScene::TitleScene(void)
{
	//更新関数のセット
	updataFunc_ = std::bind(&TitleScene::LoadingUpdate, this);
	//描画関数のセット
	drawFunc_ = std::bind(&TitleScene::LoadingDraw, this);
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Load(void)
{
	//フォントの登録
	buttnFontHandle_ = CreateFontToHandle(FontRegistry::DOT.c_str(), FONT_SIZE, 0);
}

void TitleScene::Init(void)
{
}

void TitleScene::NormalUpdate(void)
{	
	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::SELECT);
	}
}

void TitleScene::NormalDraw(void)
{
	DrawBox(
		0,
		0,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		0x0000ff,
		true
	);

	DrawFormatString(
		0, 0,
		0x000000,
		"TitleScene"
	);
}

void TitleScene::ChangeNormal(void)
{
	//処理変更
	updataFunc_ = std::bind(&TitleScene::NormalUpdate, this);
	drawFunc_ = std::bind(&TitleScene::NormalDraw, this);
}

