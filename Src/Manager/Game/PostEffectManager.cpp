#include <DxLib.h>
#include "PostEffectManager.h"
#include "../Application.h"
#include "../Manager//System/SceneManager.h"
#include "../Object/PostEffect/PostEffectBase.h"
#include "../Object/PostEffect/WiggleEffect.h"
#include "../Object/PostEffect/FishEyeEffect.h"
#include "../Object/PostEffect/SandStormEffect.h"

PostEffectManager* PostEffectManager::instance_ = nullptr;

void PostEffectManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new PostEffectManager();
	}
	instance_->Load();
}

PostEffectManager& PostEffectManager::GetInstance(void)
{
	return *instance_;
}

void PostEffectManager::Destroy(void)
{
	//自身のインスタンス削除
	delete instance_;
	instance_ = nullptr;
	DeleteGraph(postEffectScreen_);
}

void PostEffectManager::Load(void)
{
	//各種エフェクト実装
	postMap_[TYPE::WIGGLE] = std::make_unique<WiggleEffect>();
	postMap_[TYPE::FISH_EYE] = std::make_unique<FishEyeEffect>();
	postMap_[TYPE::SANDSTORM] = std::make_unique<SandStormEffect>();

	//各アイテムの描画
	for (auto& [type, post] : postMap_)
	{
		post->Load();
	}

	//スクリーンの作成
	postEffectScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);
}

void PostEffectManager::Init(void)
{
	//各アイテムの描画
	for (auto& [type, post] : postMap_)
	{
		post->Init();
	}

	type_ = TYPE::SANDSTORM;
}

void PostEffectManager::Draw(void)
{
	if (type_ == TYPE::NONE) { return; }

	//スクリーン
	int mainScreen = SceneManager::GetInstance().GetMainScreen();

	//スクリーンの設定
	SetDrawScreen(postEffectScreen_);

	// 画面を初期化
	ClearDrawScreen();

	//エフェクトを描画
	postMap_[type_]->Draw();

	// メインに戻す
	SetDrawScreen(mainScreen);
	
	//描画
	DrawGraph(0, 0, postEffectScreen_, false);
}
