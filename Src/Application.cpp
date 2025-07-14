#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "Manager/System/InputManager.h"
#include "Manager/System/KeyConfig.h"
#include "Manager/System/ResourceManager.h"
#include "Manager/System/SceneManager.h"
#include "Manager/System/SoundManager.h"
#include "FpsControl/FpsControl.h"
#include "Common/FontRegistry.h"
#include "Application.h"

Application* Application::instance_ = nullptr;

const std::string Application::PATH_IMAGE = "Data/Image/";
const std::string Application::PATH_MODEL = "Data/Model/";
const std::string Application::PATH_EFFECT = "Data/Effect/";
const std::string Application::PATH_SOUND_BGM = "Data/Sound/BGM/";
const std::string Application::PATH_SOUND_SE = "Data/Sound/SE/";
const std::string Application::PATH_FONT = "Data/Font/";
const std::string Application::PATH_TEXT = "Data/Text/";
const std::string Application::PATH_JSON = "Data/JSON/";
const std::string Application::PATH_CSV = "Data/CSV/";
const std::string Application::PATH_SHADER = "Data/Shader/";

void Application::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new Application();
	}
	instance_->Init();
}

Application& Application::GetInstance(void)
{
	return *instance_;
}

void Application::Init(void)
{

	// アプリケーションの初期設定
	SetWindowText("CHIKICHKI CREATE RACE");

	// ウィンドウサイズ
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
	ChangeWindowMode(true);

	// DxLibの初期化
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	isInitFail_ = false;
	if (DxLib_Init() == -1)
	{
		isInitFail_ = true;
		return;
	}

	// Effekseerの初期化
	InitEffekseer();

	// キー制御初期化
	SetUseDirectInputFlag(true);

	// リソース管理初期化
	ResourceManager::CreateInstance();	
	
	//入力管理の初期化
	KeyConfig::CreateInstance();

	//サウンド関係の初期化
	SoundManager::CreateInstance();

	// シーン管理初期化
	SceneManager::CreateInstance();

	// FPS初期化
	fps_ = std::make_unique<FpsControl>();
	fps_->Init();

	//フォント登録
	fontReg_ = std::make_unique<FontRegistry>();
	fontReg_->Init();
}

void Application::Run(void)
{
	LONGLONG time = GetNowHiPerformanceCount();

	KeyConfig& keyConfig = KeyConfig::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();

	// ゲームループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		//フレームレートを更新
		if (!fps_->UpdateFrameRate()) continue;

		keyConfig.Update();
		sceneManager.Update();

		sceneManager.Draw();

		//フレームレート計算
		fps_->CalcFrameRate();

		ScreenFlip();
	}

}

void Application::Destroy(void)
{
	fontReg_->Destroy();
	SceneManager::GetInstance().Destroy();
	SoundManager::GetInstance().Destroy();	
	KeyConfig::GetInstance().Destroy();
	ResourceManager::GetInstance().Destroy();

	// Effekseerを終了する。
	Effkseer_End();

	// DxLib終了
	if (DxLib_End() == -1)
	{
		isReleaseFail_ = true;
	}

	delete instance_;
	instance_ = nullptr;
}

bool Application::IsInitFail(void) const
{
	return isInitFail_;
}

bool Application::IsReleaseFail(void) const
{
	return isReleaseFail_;
}

Application::Application(void)
{
	isInitFail_ = false;
	isReleaseFail_ = false;
	fps_ = nullptr;
}

Application::~Application(void)
{

}

void Application::InitEffekseer(void)
{
	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
	}

	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
}
