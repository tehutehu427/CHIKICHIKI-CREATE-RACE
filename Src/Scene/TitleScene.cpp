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
#include "../Object/SkyDome/SkyDome.h"
#include "TitleScene.h"

TitleScene::TitleScene(void)
{
	//更新関数のセット
	func_.updataFunc_ = std::bind(&TitleScene::LoadingUpdate, this);
	//描画関数のセット
	func_.drawFunc_ = std::bind(&TitleScene::LoadingDraw, this);

	imgTitleLogo_ = -1;
	imgMessage_ = -1;
	skyDome_ = nullptr;
	alphaDir_ = 1; 
	mesPosY_ = 0.0f;
	mesAlpha_ = 0;
}

TitleScene::~TitleScene(void)
{
	DeleteFontToHandle(buttnFontHandle_);
}

void TitleScene::Load(void)
{
	ResourceManager& res = ResourceManager::GetInstance();

	//リソースの読み込み
	imgTitleLogo_ = res.Load(ResourceManager::SRC::TITLE_LOGO).handleId_;
	imgMessage_ = res.Load(ResourceManager::SRC::PUSHSPACE).handleId_;
	bgm_ = res.Load(ResourceManager::SRC::TITLE_BGM).handleId_;

	//フォントの登録
	buttnFontHandle_ = CreateFontToHandle(FontRegistry::DOT.c_str(), FONT_SIZE, 0);

	//スカイドーム
	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Load();
}

void TitleScene::Init(void)
{
	mesAlpha_ = Utility::ALPHA_MAX;
	

	//BGMを再生
	sndMng_.Play(bgm_, SoundManager::PLAYTYPE::LOOP);

	//スカイドーム初期化
	skyDome_->Init();
}

void TitleScene::NormalUpdate(void)
{	
	//ステップの更新
	step_ += SceneManager::GetInstance().GetDeltaTime();

	// シーン遷移
	KeyConfig& ins = KeyConfig::GetInstance();
	SoundManager& snd = SoundManager::GetInstance();
	if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
	{
		sndMng_.Stop(bgm_);
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::SELECT);
		return;
	}

	//スカイドーム更新
	skyDome_->Update();
}

void TitleScene::NormalDraw(void)
{
	//スカイドーム描画
	skyDome_->Draw();

	//タイトルロゴ
	DrawRotaGraph(
		Application::SCREEN_HALF_X,
		LOGO_POS_Y,
		1.0f,
		0.0f,
		imgTitleLogo_,
		true,
		false
	);

	//メッセージの描画
	DrawMessage();
}

void TitleScene::ChangeNormal(void)
{
	//処理変更
	func_.updataFunc_ = std::bind(&TitleScene::NormalUpdate, this);
	func_.drawFunc_ = std::bind(&TitleScene::NormalDraw, this);
}

void TitleScene::DrawMessage(void)
{
	constexpr float RATE = 0.6f;
	constexpr float ALPHA_STEP = 3.0f;
	constexpr float SHAKE_SPEED = 5.0f;
	constexpr float SHAKE_AMPLITUDE = 5.0f;

	//座標を揺らす
	mesPosY_ = MES_POS_Y;
	mesPosY_ = Utility::GetShake(mesPosY_, step_, SHAKE_SPEED, SHAKE_AMPLITUDE);

	//アルファ値を変え
	mesAlpha_ = Utility::PingPongUpdate(mesAlpha_, ALPHA_STEP, Utility::ALPHA_MAX, Utility::ALPHA_MAX / 2, alphaDir_);

	//メッセージ
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, mesAlpha_);
	DrawRotaGraph(
		Application::SCREEN_HALF_X,
		mesPosY_,
		RATE,
		0.0f,
		imgMessage_,
		true,
		false
	);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

