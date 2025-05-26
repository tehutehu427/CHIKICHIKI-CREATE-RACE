#include <DxLib.h>
#include <string>
#include "../Manager/System/ResourceManager.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/System/InputManager.h"
#include "../Utility/Utility.h"
#include "SceneBase.h"

SceneBase::SceneBase(void) :
	resMng_(ResourceManager::GetInstance())
	,scnMng_(SceneManager::GetInstance())
	,inputMng_(InputManager::GetInstance())
{
	buttnFontHandle_ = -1;
	loadingTime_ = -1;
}

SceneBase::~SceneBase(void)
{
	DeleteFontToHandle(buttnFontHandle_);
}

void SceneBase::Update(void)
{
	updataFunc_();
	return;
}

void SceneBase::Draw(void)
{
	drawFunc_();
	return;
}

void SceneBase::LoadingUpdate(void)
{
	bool loadTimeOver = Utility::IsTimeOver(loadingTime_, LOADING_TIME);

	//ロードが完了したか判断
	if (GetASyncLoadNum() == 0 && loadTimeOver)
	{
		//非同期処理を無効にする
		SetUseASyncLoadFlag(false);

		//初期化処理
		Init();

		//フェードイン開始
		scnMng_.StartFadeIn();

		//通常の処理へ移る
		ChangeNormal();
	}
}

void SceneBase::LoadingDraw(void)
{
	//NowLoadingの描画
	DrawNowLoading();
}

void SceneBase::DrawNowLoading(void)
{
	//ロード中
	auto time = scnMng_.GetTotalTime();
	int count = static_cast<int>(time / COMMA_TIME);
	count %= COMMA_MAX_NUM;

	std::string loadStr = "Now loading";
	std::string dotStr = ".";

	for (int i = 0; i < count; i++)
	{
		loadStr += dotStr;
	}
	DrawStringToHandle(LOADING_STRING_POS_X, LOADING_STRING_POS_Y, loadStr.c_str(), Utility::WHITE, buttnFontHandle_);

}
