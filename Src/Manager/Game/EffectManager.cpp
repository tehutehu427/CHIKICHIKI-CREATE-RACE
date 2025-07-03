#include<DxLib.h>
#include<EffekseerForDXLib.h>
#include "EffectManager.h"

//インスタンス初期化
EffectManager* EffectManager::instance_ = nullptr;

void EffectManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		//初期生成
		instance_ = new EffectManager();
	}

	//初期化処理
	if (!Init())
	{
		//初期化失敗
		DxLib_End();
	}
}

void EffectManager::Destroy(void)
{
	// Effekseerを終了する。
	Effkseer_End();

	//自身のインスタンス解放
	delete instance_;
	instance_ = nullptr;
}

EffectManager::EffectManager(void)
{
}

const bool EffectManager::Init(void)
{
	//エフェクシアの初期化
	if (Effekseer_Init(PARTICLE_MAX) == -1)
	{
		//初期化失敗
		return false;
	}

	//画面モード切替時のグラフィックハンドルのリセットをしない
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	//DxLibのデバイスロスト時にコールバックを設定
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	//初期化成功
	return true;
}

EffectManager::~EffectManager(void)
{
}
