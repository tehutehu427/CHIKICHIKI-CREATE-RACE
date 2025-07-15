 #include <chrono>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../../Scene/TitleScene.h"
#include "../../Scene/SelectScene.h"
#include "../../Scene/Game/GameScene.h"
#include "../../Scene/Game/FreePlay.h"
#include "../../Scene/Game/MultiParty.h"
#include "../../Scene/Game/SoloChallenge.h"
#include "../Game/PlayerManager.h"
#include "../Game/CollisionManager.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "SoundManager.h"
#include "DateBank.h"
#include "SceneManager.h"

SceneManager* SceneManager::instance_ = nullptr;

void SceneManager::CreateInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new SceneManager();
	}
	instance_->Init();
}

SceneManager& SceneManager::GetInstance(void)
{
	return *instance_;
}

void SceneManager::Init(void)
{

	sceneId_ = SCENE_ID::TITLE;
	waitSceneId_ = SCENE_ID::NONE;

	fader_ = std::make_unique<Fader>();
	fader_->Init();

	// カメラ（初期化時は1つのみ生成）
	CreateCameras(1);

	isSceneChanging_ = true;

	// デルタタイム
	preTime_ = std::chrono::system_clock::now();

	// メインスクリーン
	mainScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);

	//データバンクを生成
	DateBank::CreateInstance();

	//当たり判定管理の初期化(各シーンで追加の可能性があるため)
	CollisionManager::CreateInstance();

	//ウィンドウがアクティブ状態でなくとも処理を行う
	SetAlwaysRunFlag(true);

	// 3D用の設定
	Init3D();

	// 初期シーンの設定
	DoChangeScene(SCENE_ID::TITLE);

	screenIndex_ = 0;
}

void SceneManager::Init3D(void)
{
	// 背景色設定
	SetBackgroundColor(0, 0, 0);

	// Zバッファを有効にする
	SetUseZBuffer3D(true);

	// Zバッファへの書き込みを有効にする
	SetWriteZBuffer3D(true);

	// バックカリングを有効にする
	SetUseBackCulling(true);

	// ライトの設定
	SetUseLighting(true);
	
	// ライトの設定
	ChangeLightTypeDir({ 0.3f, -0.7f, 0.8f });

	// フォグ設定
	SetFogEnable(true);
	SetFogColor(5, 5, 5);
	SetFogStartEnd(10000.0f, 20000.0f);

}

void SceneManager::Update(void)
{
	if (scene_ == nullptr) { return; }

	// デルタタイム
	auto nowTime = std::chrono::system_clock::now();
	deltaTime_ = static_cast<float>(
		std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);
	preTime_ = nowTime;
	totalTime_ += deltaTime_;

	fader_->Update();
	if (isSceneChanging_)
	{
		Fade();
	}

	//シーンごとの更新
	scene_->Update();

	for (auto& c : cameras_)
	{
		c->Update();
	}
}

void SceneManager::Draw(void)
{
	
	// 描画先グラフィック領域の指定
	// (３Ｄ描画で使用するカメラの設定などがリセットされる)
	SetDrawScreen(mainScreen_);

	// 画面を初期化
	ClearDrawScreen();

	if (isSplitMode_)
	{
		//複数画面の描画
		DrawMultiScreen();
	}
	else
	{
		// カメラ設定
		cameras_[0]->SetBeforeDraw();

		// Effekseerにより再生中のエフェクトを更新する。
		UpdateEffekseer3D();

		// 描画
		scene_->Draw();

		// 主にポストエフェクト用
		cameras_[0]->Draw();

		// Effekseerにより再生中のエフェクトを描画する。
		DrawEffekseer3D();

		// 暗転・明転
		fader_->Draw();
	}

	//背面スクリーンにメインスクリーンを描画
	SetDrawScreen(DX_SCREEN_BACK);

	// 画面を初期化
	ClearDrawScreen();

	// カメラ設定
	cameras_[0]->CameraSetting();

	//メインスクリーンを描画
	DrawGraph(0, 0, mainScreen_, false);

}

void SceneManager::CreateScene(std::shared_ptr<SceneBase> scene)
{
	//if (scenes_.empty())
	//{
	//	scenes_.push_back(scene);
	//}
	//else
	//{
	//	scenes_.front() = scene;
	//}

	////データのロード
	//scenes_.front()->Load();
}

void SceneManager::ChangeAllScene(std::shared_ptr<SceneBase> scene)
{
	////フェード開始
	//StartFadeIn();

	//scenes_.clear();
	//scenes_.push_back(scene);
	////データのロード
	//scenes_.front()->LoadData();
}

void SceneManager::PushScene(std::shared_ptr<SceneBase> scene)
{
	//scene->Init();
	//scenes_.push_front(scene);
}

void SceneManager::PopScene()
{
	/*if (scenes_.size() >= 1)
	{
		scenes_.pop_front();
	}*/
}

void SceneManager::Destroy(void)
{
	//当たり判定管理の解放
	CollisionManager::GetInstance().Destroy();
	
	//データバンクの解放
	//スクリーンの解放
	DeleteGraph(mainScreen_);
	for(auto & screen : splitScreens_){ DeleteGraph(screen); }

	DateBank::GetInstance().Destroy();
	
	//自身のインスタンス解放
	delete instance_;
	instance_ = nullptr;
}

void SceneManager::ChangeScene(SCENE_ID nextId)
{

	// フェード処理が終わってからシーンを変える場合もあるため、
	// 遷移先シーンをメンバ変数に保持
	waitSceneId_ = nextId;

	// フェードアウト(暗転)を開始する
	fader_->SetFade(Fader::STATE::FADE_OUT);
	isSceneChanging_ = true;

}

void SceneManager::StartFadeIn(void)
{
	//フェードを明ける
	fader_->SetFade(Fader::STATE::FADE_IN);

	//シーンチェンジ
	isSceneChanging_ = false;
}

std::weak_ptr<Camera> SceneManager::GetCamera(const int _playerIndex) const
{
	return cameras_[_playerIndex];
}

void SceneManager::CreateCameras(const int _playerNum)
{
	//現在のカメラの数が引数と同じ場合
	if (cameras_.size() == _playerNum)
	{
		//処理は実行しない
		return;
	}
	//カメラが空じゃない場合
	else if (!cameras_.empty())
	{
		//カメラの中身を削除
		cameras_.clear();
	}
	//カメラ生成
	for (int i = 0; i < _playerNum; i++)
	{
		std::shared_ptr<Camera> camera;
		camera = std::make_shared<Camera>(i);
		camera->Init();
		cameras_.push_back(std::move(camera));
	}

}

void SceneManager::CreateSplitScreen(const int _playerNum)
{
	//引数が１以下
	// 最大人数を超える場合,
	// または引数と現在のスクリーン数が同じとき
	if (_playerNum <= 1 || 
		_playerNum > PlayerManager::PLAYER_NUM_MAX ||
		splitScreens_.size() == _playerNum)
	{
		isSplitMode_ = false;	//分割しない
		return;					//生成しない
	}
	//空じゃない場合
	else if (!splitScreens_.empty())
	{
		//中身削除
		splitScreens_.clear();
	}

	//分割を行う
	isSplitMode_ = true;

	int createNum = _playerNum;	//生成数
	int divY = 1;				//Yの分割数
	
	//人数が条件以上の場合
	if (_playerNum >= CASE_VALUE)
	{
		createNum = PlayerManager::PLAYER_NUM_MAX;	//最大人数分生成
		divY++;									//画面分割数増加
	}

	//スクリーン生成
	for (int i = 0; i < createNum; i++)
	{
		int screen = MakeScreen(
			Application::SCREEN_HALF_X,
			Application::SCREEN_SIZE_Y / divY,
			true);
		splitScreens_.push_back(screen);
	}
}

SceneManager::SceneManager(void)
{
	mainScreen_ = -1;	//メインスクリーンの初期化
	screenIndex_ = 0;	//分割スクリーンのインデックス初期化

	sceneId_ = SCENE_ID::NONE;
	waitSceneId_ = SCENE_ID::NONE;

	scene_ = nullptr;
	fader_ = nullptr;

	isSceneChanging_ = false;
	isSplitMode_ = false;

	cameras_.clear();
	splitScreens_.clear();

	// デルタタイム
	deltaTime_ = 1.0f / 60.0f;
	totalTime_ = -1.0f;

}

SceneManager::~SceneManager(void)
{
}

void SceneManager::ResetDeltaTime(void)
{
	deltaTime_ = 0.016f;
	preTime_ = std::chrono::system_clock::now();
}

void SceneManager::DoChangeScene(SCENE_ID sceneId)
{
	// リソースの解放
	ResourceManager::GetInstance().Release();	
	SoundManager::GetInstance().Release();	

	// シーンを変更する
 	sceneId_ = sceneId;
	
	//初期化
	screenIndex_ = 0;		

	// 現在のシーンを解放
	if (scene_ != nullptr)
	{
		scene_.reset();
	}
	
	//現在使用している音源の解放


	//シーンに合わせて生成数を設定
	const int createNum = (sceneId == SCENE_ID::MULTI) ? DateBank::GetInstance().GetPlayerNum() : 1;

	//カメラ生成
	CreateCameras(createNum);

	//分割スクリーン生成
	CreateSplitScreen(createNum);

	//シーンを生成
	switch (sceneId_)
	{
	case SCENE_ID::TITLE:
		scene_ = std::make_unique<TitleScene>();
		break;

	case SCENE_ID::SELECT:
		scene_ = std::make_unique<SelectScene>();
		break;

	case SCENE_ID::GAME:
		scene_ = std::make_unique<GameScene>();
		break;

	case SCENE_ID::FREE:
		scene_ = std::make_unique<FreePlay>();
		break;

	case SCENE_ID::MULTI:
		scene_ = std::make_unique<MultiParty>();
		break;

	case SCENE_ID::SOLO:
		scene_ = std::make_unique<SoloChallenge>();
		break;
	}

	//読み込み
	scene_->Load();

	//デルタタイムリセット
	ResetDeltaTime();

	//シーンID初期化
	waitSceneId_ = SCENE_ID::NONE;
}

void SceneManager::Fade(void)
{

	Fader::STATE fState = fader_->GetState();
	switch (fState)
	{
	case Fader::STATE::FADE_IN:
		// 明転中
		if (fader_->IsEnd())
		{
			// 明転が終了したら、フェード処理終了
			fader_->SetFade(Fader::STATE::NONE);
			//isSceneChanging_ = false;
		}
		break;
	case Fader::STATE::FADE_OUT:
		// 暗転中
		if (fader_->IsEnd())
		{
			// 完全に暗転してからシーン遷移
			DoChangeScene(waitSceneId_);
			// 暗転から明転へ
			//fader_->SetFade(Fader::STATE::FADE_IN);
			fader_->SetFade(Fader::STATE::NONE);
		}
		break;
	}
}

void SceneManager::DrawMultiScreen()
{
	//描画位置（分割スクリーンの左上位置）
	const Vector2 screenPos[PlayerManager::PLAYER_NUM_MAX] =
	{
		{ 0, 0 },													// 1P: 左上
		{ Application::SCREEN_HALF_X, 0 },							// 2P: 右上
		{ 0, Application::SCREEN_HALF_Y },							// 3P: 左下
		{ Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y }	// 4P: 右下
	};

	//スクリーン数分描画
	for (int i = 0; i < splitScreens_.size(); i++)
	{
		//プレイ人数が3人の時の4つ目の画面を1Pの画面を表示する
		screenIndex_ = i;	//分割スクリーンのインデックス
		int index = i;
		if (CASE_VALUE == DateBank::GetInstance().GetPlayerNum() &&
			index == PlayerManager::PLAYER_NUM_MAX - 1)
		{
			index = 1;
			screenIndex_ = 1;
		}

		//分割スクリーンの設定
		SetDrawScreen(splitScreens_[index]);

		//画面クリア
		ClearDrawScreen();

		// カメラ設定
		cameras_[index]->SetBeforeDraw();

		// Effekseerにより再生中のエフェクトを更新する。
		UpdateEffekseer3D();

		// 描画
		scene_->Draw();

		// 主にポストエフェクト用
		cameras_[index]->Draw();

		// Effekseerにより再生中のエフェクトを描画する。
		DrawEffekseer3D();

		// 暗転・明転
		fader_->Draw();
	}

	//メインスクリーンへ描画
	SetDrawScreen(mainScreen_);


	for (int i = 0; i < splitScreens_.size(); i++)
	{
		//分割したスクリーンを描画
		DrawGraph(screenPos[i].x, screenPos[i].y, splitScreens_[i], true);
	}
}
