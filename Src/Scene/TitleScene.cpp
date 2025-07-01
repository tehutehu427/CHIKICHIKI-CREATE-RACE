#include <string>
#include <DxLib.h>
#include "../Common/FontRegistry.h"
#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/System/ResourceManager.h"
#include "../Manager/System/KeyConfig.h"
#include "../Manager/System/InputManager.h"
#include "../Manager/System/SoundManager.h"
#include "TitleScene.h"

TitleScene::TitleScene(void)
{
	//更新関数のセット
	func_.updataFunc_ = std::bind(&TitleScene::LoadingUpdate, this);
	//描画関数のセット
	func_.drawFunc_ = std::bind(&TitleScene::LoadingDraw, this);

	imgTitleLogo_ = -1;
}

TitleScene::~TitleScene(void)
{
	DeleteFontToHandle(buttnFontHandle_);
}

void TitleScene::Load(void)
{
	ResourceManager& res = ResourceManager::GetInstance();
	imgTitleLogo_ = res.Load(ResourceManager::SRC::TITLE_LOGO).handleId_;
	bgm_ = res.Load(ResourceManager::SRC::TITLE_BGM).handleId_;

	//フォントの登録
	buttnFontHandle_ = CreateFontToHandle(FontRegistry::DOT.c_str(), FONT_SIZE, 0);
}

void TitleScene::Init(void)
{
	//BGMを再生
	sndMng_.Play(bgm_, SoundManager::PLAYTYPE::LOOP);
}

void TitleScene::NormalUpdate(void)
{	
	// シーン遷移
	KeyConfig& ins = KeyConfig::GetInstance();
	SoundManager& snd = SoundManager::GetInstance();
	if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
	{
		sndMng_.Stop(bgm_);
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

	DrawRotaGraph(
		Application::SCREEN_HALF_X,
		Application::SCREEN_HALF_Y,
		1.0f,
		0.0f,
		imgTitleLogo_,
		true,
		false
	);
}

void TitleScene::ChangeNormal(void)
{
	//処理変更
	func_.updataFunc_ = std::bind(&TitleScene::NormalUpdate, this);
	func_.drawFunc_ = std::bind(&TitleScene::NormalDraw, this);
}

