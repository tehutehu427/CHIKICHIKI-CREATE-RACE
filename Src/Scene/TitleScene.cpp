#include <string>
#include <DxLib.h>
#include "../Common/FontRegistry.h"
#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/System/ResourceManager.h"
#include "../Manager/System/KeyConfig.h"
#include "../Manager/System/Camera.h"
#include "../Manager/System/InputManager.h"
#include "../Manager/System/SoundManager.h"
#include "../Object/SkyDome/SkyDome.h"
#include "Game/DemoPlay.h"
#include "TitleScene.h"

TitleScene::TitleScene(void)
{
	//更新関数のセット
	func_.updataFunc_ = std::bind(&TitleScene::LoadingUpdate, this);
	//描画関数のセット
	func_.drawFunc_ = std::bind(&TitleScene::LoadingDraw, this);

	imgTitleLogo_ = -1;
	imgMessage_ = -1;
	imgDemoMessage_ = -1;

	int i = -1;
	imgBalloons_ = &i;

	for (int i = 0; i < BALLOON_NUM; i++)
	{
		balloonPos_[i] = { 0,0 };
		balloonType_[i] = -1;
		isBalloonAlive_[i] = false;
	}
	skyDome_ = nullptr;
	alphaDir_ = 1; 
	mesPosY_ = 0.0f;
	mesAlpha_ = 0;
	demoUIStep_ = 0.0f;
	logoSize_ = 1.0f;
	balloonStep_ = 0.0f;
	titleUpdateFunc_ = std::bind(&TitleScene::UpdateWait, this);	//初期更新処理
}

TitleScene::~TitleScene(void)
{
	//フォント解放
	DeleteFontToHandle(buttnFontHandle_);
}

void TitleScene::Load(void)
{
	//リソースマネージャー
	ResourceManager& res = ResourceManager::GetInstance();	
	
	//フォントの登録
	buttnFontHandle_ = CreateFontToHandle(FontRegistry::DOT.c_str(), FONT_SIZE, 0);

	//ローディング中のリソースを取得
	LoadRandomLoadingMessage();

	//リソースの読み込み
	imgTitleLogo_ = res.Load(ResourceManager::SRC::TITLE_LOGO).handleId_;
	imgMessage_ = res.Load(ResourceManager::SRC::PUSHSPACE).handleId_;
	imgDemoMessage_ = res.Load(ResourceManager::SRC::PUSH_DEMO).handleId_;
	imgBalloons_ = res.Load(ResourceManager::SRC::BALLOON).handleIds_;
	sndMng_.LoadResource(SoundManager::SRC::TITLE_BGM);
	sndMng_.LoadResource(SoundManager::SRC::TITLE_SCENE_CHANGE);
	sndMng_.LoadResource(SoundManager::SRC::CHICKEN_SE);

	//音量設定
	sndMng_.SetLoadedSoundsVolume();

	//スカイドーム
	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Load();
}

void TitleScene::Init(void)
{
	//アルファ値初期化
	mesAlpha_ = Utility::ALPHA_MAX;
	
	//BGMを再生
	sndMng_.Play(SoundManager::SRC::TITLE_BGM, SoundManager::PLAYTYPE::LOOP);

	//スカイドーム初期化
	skyDome_->Init();

	//カメラ設定
	auto camera = scnMng_.GetCamera(0).lock();
	camera->ChangeMode(Camera::MODE::FIXED_POINT);
}

void TitleScene::NormalUpdate(void)
{	
	//デルタタイム
	float delta = SceneManager::GetInstance().GetDeltaTime();

	//状態別更新処理
	titleUpdateFunc_();

	//スカイドーム更新
	skyDome_->Update();

	//風船更新
	BalloonUpdate();

	//カウンタ更新
	balloonStep_ += delta;
}

void TitleScene::DemoUpdate(void)
{
	//インスタンス取得
	KeyConfig& ins = KeyConfig::GetInstance();
	SoundManager& snd = SoundManager::GetInstance();

	//デルタタイム
	float delta = SceneManager::GetInstance().GetDeltaTime();

	//ステップの更新
	step_ += delta;
	demoUIStep_ += delta;

	float moveSize =1.0f - LOGO_MIN_SIZE;
	float moveTime = moveSize / DEMO_LOGO_MOVE_TIME;

	//ロゴのサイズ変更
	logoSize_ = std::clamp(1.0f - demoUIStep_ / (DEMO_CHANGE_SIZE * moveTime), LOGO_MIN_SIZE, 1.0f);

	//スカイドーム更新
	skyDome_->Update();

	//デモプレイ更新
	demo_->Update();

	//風船更新
	BalloonUpdate();

	// シーン遷移
	if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::DEMO_TO_TITLE_BACK, KeyConfig::JOYPAD_NO::PAD1))
	{
		//カウンタの初期化
		step_ = 0.0f;
		demoUIStep_ = 0.0f;

		//最初に戻る
		ChangeNormal();
		return;
	}
}

void TitleScene::NormalDraw(void)
{
	//スカイドーム描画
	skyDome_->Draw();

	//風船描画
	DrawBalloon();

	//タイトルロゴ
	DrawRotaGraph(
		Application::SCREEN_HALF_X,
		LOGO_POS_Y,
		1.0,
		0.0,
		imgTitleLogo_,
		true,
		false
	);

	//メッセージの描画
	DrawMessage();
}

void TitleScene::DemoDraw(void)
{
	//スカイドーム描画
	skyDome_->Draw();

	//透明度追加
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, DEMO_IMAGE_ALPHA);

	float movePosX = Application::SCREEN_HALF_X - LOGO_MIN_POS_X;
	float moveTimeX = movePosX / DEMO_LOGO_MOVE_TIME;
	float movePosY = LOGO_POS_Y - LOGO_MIN_POS_Y;
	float moveTimeY = movePosY / DEMO_LOGO_MOVE_TIME;

	//風船描画
	DrawBalloon();

	//タイトルロゴ
	DrawRotaGraph(
		std::clamp(Application::SCREEN_HALF_X - demoUIStep_ * moveTimeX, static_cast<float>(LOGO_MIN_POS_X), static_cast<float>(Application::SCREEN_HALF_X)),
		std::clamp(LOGO_POS_Y - demoUIStep_ * moveTimeY,static_cast<float>(LOGO_MIN_POS_Y), static_cast<float>(LOGO_POS_Y)),
		static_cast<double>(logoSize_),
		0.0,
		imgTitleLogo_,
		true,
		false
	);

	//元に戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//デモプレイ描画
	demo_->Draw();

	//メッセージの描画
	DemoMessage();
}

void TitleScene::ChangeNormal(void)
{
	//処理変更
	func_.updataFunc_ = std::bind(&TitleScene::NormalUpdate, this);
	func_.drawFunc_ = std::bind(&TitleScene::NormalDraw, this);
	titleUpdateFunc_ = std::bind(&TitleScene::UpdateWait, this);

	//デモの解放
	demo_.reset();
}

void TitleScene::ChangeDemo(void)
{
	//処理変更
	titleUpdateFunc_ = std::bind(&TitleScene::DemoUpdate, this);
	func_.drawFunc_ = std::bind(&TitleScene::DemoDraw, this);

	//生成しなおす
	demo_ = std::make_unique<DemoPlay>();
	demo_->Load();

	//初期化も
	demo_->Init();
}

void TitleScene::DrawMessage(void)
{
	//メッセージの動作関係
	constexpr float RATE = 0.6f;
	constexpr float ALPHA_STEP = 3.0f;
	constexpr float SHAKE_SPEED = 5.0f;
	constexpr float SHAKE_AMPLITUDE = 5.0f;

	//座標を揺らす
	mesPosY_ = MES_POS_Y;
	mesPosY_ = Utility::GetShake(mesPosY_, step_, SHAKE_SPEED, SHAKE_AMPLITUDE);

	//アルファ値を変え
	mesAlpha_ = Utility::PingPongUpdate(mesAlpha_, ALPHA_STEP, Utility::ALPHA_MAX, Utility::ALPHA_MAX / 2, alphaDir_);

	//アルファ値ブレンド
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, mesAlpha_);

	//メッセージ
	DrawRotaGraph(
		Application::SCREEN_HALF_X,
		mesPosY_,
		static_cast<double>(RATE),
		0.0f,
		imgMessage_,
		true,
		false
	);

	//元に戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TitleScene::DemoMessage(void)
{
	//メッセージの動作関係
	constexpr float RATE = 0.6f;
	constexpr float ALPHA_STEP = 3.0f;
	constexpr float SHAKE_SPEED = 5.0f;
	constexpr float SHAKE_AMPLITUDE = 5.0f;
	constexpr float ROTATE_ANGLE = 270.0f;

	//座標を揺らす
	mesPosY_ = DEMO_MES_POS_Y;
	mesPosY_ = Utility::GetShake(mesPosY_, step_, SHAKE_SPEED, SHAKE_AMPLITUDE);

	//アルファ値ブレンド
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, DEMO_IMAGE_ALPHA);

	//デモメッセージ
	DrawRotaGraph(
		DEMO_MES_POS_X,
		mesPosY_,
		static_cast<double>(RATE),
		Utility::Deg2RadF(ROTATE_ANGLE),
		imgDemoMessage_,
		true,
		false
	);

	//元に戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TitleScene::UpdateWait()
{
	//デルタタイム
	float delta = SceneManager::GetInstance().GetDeltaTime();

	//ステップの更新
	step_ += delta;

	// シーン遷移
	KeyConfig& ins = KeyConfig::GetInstance();
	SoundManager& snd = SoundManager::GetInstance();
	if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
	{
		//BGMの停止
		sndMng_.Stop(SoundManager::SRC::TITLE_BGM);

		//BGMの再生
		sndMng_.Play(SoundManager::SRC::TITLE_SCENE_CHANGE, SoundManager::PLAYTYPE::BACK);
		sndMng_.Play(SoundManager::SRC::CHICKEN_SE, SoundManager::PLAYTYPE::BACK);

		//更新処理の変更
		titleUpdateFunc_ = std::bind(&TitleScene::UpdatePlaySe, this);
		return;
	}

	//デモへの遷移
	if (step_ >= DEMO_CHANGE_TIME)
	{
		//デモへ
		ChangeDemo();
	}
}

void TitleScene::UpdatePlaySe()
{
	//SEが終わったら
	if (!sndMng_.IsPlay(SoundManager::SRC::CHICKEN_SE))
	{
		//セレクトに遷移
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::SELECT);

		//更新処理の変更
		titleUpdateFunc_ = std::bind(&TitleScene::UpdateNone, this);
		return;
	}
}

void TitleScene::UpdateNone()
{
	//何もしない
}

void TitleScene::BalloonUpdate(void)
{
	//全バルーン
	for (int i = 0; i < BALLOON_NUM; i++)
	{
		//風船の座標を画面下よりどれだけ下げるか
		const int OFFSET_Y = 120;

		//上に上がるスピード
		int speed_Y = 0;
		//バルーンが死んでおり、間隔が1.0秒以上言ったら
		if (!isBalloonAlive_[i] && balloonStep_ > BALLOON_STEP_MAX)
		{
			//新しいバルーンを生成
			balloonType_[i] = GetRand(BALLOON_TYPE);
			isBalloonAlive_[i] = true;
			balloonPos_[i].x = std::rand() % Application::SCREEN_SIZE_X + BALLOON_SIZE_ONE_HALF_X;
			balloonPos_[i].y = Application::SCREEN_SIZE_Y + OFFSET_Y;
			balloonStep_ = 0.0f;
			speed_Y = std::rand() % SPEED_MAX + SPEED_MIN;
			speed_[i] = speed_Y;
		}
		//バルーンが生きている間の処理
		if (isBalloonAlive_[i])
		{
			//バルーンを上昇
			balloonPos_[i].y -= speed_[i];
		}

		//バルーンが上まで上がったら
		if (balloonPos_[i].y < BALLOON_POS_MAX_Y)
		{
			//バルーンの消去
			balloonType_[i] = -1;
			balloonPos_[i] = { 0,-BALLOON_SIZE_ONE_Y };
			isBalloonAlive_[i] = false;
		}
	}
}

void TitleScene::DrawBalloon(void)
{
	//全バルーンの描画
	for (int i = 0; i < BALLOON_NUM; i++)
	{
		//バルーンが生きているなら
		if (isBalloonAlive_[i])
		{
			//描画
			DrawRotaGraph(
				balloonPos_[i].x,
				balloonPos_[i].y,
				1.0f,
				0.0f,
				imgBalloons_[balloonType_[i]],
				true,
				false
			);
		}
	}
}

