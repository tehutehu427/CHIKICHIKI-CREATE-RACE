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
	seTest_ = res.Load(ResourceManager::SRC::TEST_SE).handleId_;
	seTest2_ = res.Load(ResourceManager::SRC::TEST2_SE).handleId_;

	//フォントの登録
	buttnFontHandle_ = CreateFontToHandle(FontRegistry::DOT.c_str(), FONT_SIZE, 0);
}

void TitleScene::Init(void)
{

}

void TitleScene::NormalUpdate(void)
{	
	// シーン遷移
	KeyConfig& ins = KeyConfig::GetInstance();
	SoundManager& snd = SoundManager::GetInstance();
	if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::SELECT);
	}

	//音を鳴らす
	if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_RIGHT, KeyConfig::JOYPAD_NO::PAD1))
	{
		snd.Play(seTest_, SoundManager::PLAYTYPE::BACK);
	}
	else if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_LEFT, KeyConfig::JOYPAD_NO::PAD1))
	{
		snd.Play(seTest2_, SoundManager::PLAYTYPE::NORMAL);
	}
	else if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_DOWN, KeyConfig::JOYPAD_NO::PAD1))
	{
		snd.Play(seTest2_, SoundManager::PLAYTYPE::LOOP);
	}
	else if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_UP, KeyConfig::JOYPAD_NO::PAD1))
	{
		snd.Stop(seTest_);
		snd.Stop(seTest2_);
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

