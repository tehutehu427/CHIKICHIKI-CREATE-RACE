#include "MultiParty.h"
#include "../../Manager/System/Camera.h"
#include "../../Manager/System/DateBank.h"
#include "../../Manager/System/SceneManager.h"
#include "../../Manager/Game/ScoreManager.h"
#include "../../Object/Editor/Palette/EditorPaletteBase.h"
#include "../../Object/Editor/Palette/MultiPalette.h"
#include "../../Object/System/Result/MultiResult.h"

MultiParty::MultiParty(void)
{
	result_ = nullptr;
	phaseChanges_.emplace(PHASE::SELECT_PHASE, std::bind(&MultiParty::ChangePhaseSelect, this));
	phaseChanges_.emplace(PHASE::RESULT_PHASE, std::bind(&MultiParty::ChangePhaseResult, this));
}

	//カメラ作成
	{
		createCamera_[1] = [this]()
			{
				std::vector<std::shared_ptr<Camera>> cameras;
				std::shared_ptr<Camera> camera = std::make_shared<Camera>();
				camera->Init();
				cameras.push_back(camera);
				return cameras;
			};
		createCamera_[2] = [this]()
			{
				std::vector<std::shared_ptr<Camera>> cameras;
				for (int i = 0; i < 2;i++)
				{
					std::shared_ptr<Camera> camera = std::make_shared<Camera>();
					camera->Init();
					cameras.push_back(camera);
				}
				return cameras;
			};
		createCamera_[3] = [this]()
			{
				std::vector<std::shared_ptr<Camera>> cameras;
				for (int i = 0; i < 4;i++)
				{
					std::shared_ptr<Camera> camera = std::make_shared<Camera>();
					camera->Init();
					cameras.push_back(camera);
				}
				return cameras;
			};
		createCamera_[4] = [this]()
			{
				std::vector<std::shared_ptr<Camera>> cameras;
				for (int i = 0; i < 4;i++)
				{
					std::shared_ptr<Camera> camera = std::make_shared<Camera>();
					camera->Init();
					cameras.push_back(camera);
				}
				return cameras;
			};
	}
}

MultiParty::~MultiParty(void)
{
	for (auto& screen : screens_)
	{
		DeleteGraph(screen);
	}

	//スコアマネージャーのインスタンスを削除
	ScoreManager::GetInstance().Destroy();
}

void MultiParty::Load(void)
{
	//親クラスの読み込み処理を呼ぶ
	GameScene::Load();

	//パレット生成
	palette_ = std::make_unique<MultiPalette>(editControllers_);
	palette_->Load();

	//リザルト
	result_ = std::make_unique<MultiResult>();
	result_->Load();

	//スコアマネージャーを生成
	ScoreManager::GetInstance().CreateInstance();

	// プレイヤー人数を取得
	int playerNum = DateBank::GetInstance().GetPlayerNum();

	// 分割数を計算（2人なら左右分割、それ以上は全画面）
	const Vector2 div = (playerNum == 2) ? Vector2{ 2, 1 } : Vector2{ 2, 2 };

	// プレイヤーごとに描画用スクリーンを作成
	for (int i = 0; i < playerNum; i++)
	{
		int screen = MakeScreen(Application::SCREEN_SIZE_X / div.x, Application::SCREEN_SIZE_Y / div.y, true);
		screens_.push_back(screen);
	}
}

void MultiParty::Init(void)
{
	//親クラスの初期化処理を呼ぶ
	GameScene::Init();

	//初期化
	palette_->Init();

	//リザルト初期化
	result_->Init();

	//フェーズ遷移
	ChangePhase(PHASE::RESULT_PHASE);
}

std::weak_ptr<Camera> MultiParty::GetCamera(int playerNum_)
{
	return cameras_[playerNum_];
}

void MultiParty::NormalDraw(void)
{
	//親クラスの描画処理を呼ぶ
	GameScene::NormalDraw();
}

void MultiParty::UpdateAction(void)
{
	//親クラスのアクション時の更新処理を呼ぶ
	GameScene::UpdateAction();
}

void MultiParty::UpdateEdit(void)
{
	//親クラスのエディット時の更新処理を呼ぶ
	GameScene::UpdateEdit();
}

void MultiParty::ChangePhaseSelect()
{
	phaseUpdate_ = std::bind(&MultiParty::UpdateSelect, this);
	phaseDraw_ = std::bind(&MultiParty::DrawSelect, this);
}

void MultiParty::ChangePhaseResult()
{
	phaseUpdate_ = std::bind(&MultiParty::UpdateResult, this);
	phaseDraw_ = std::bind(&MultiParty::DrawResult, this);
}

void MultiParty::UpdateSelect()
{
	palette_->Update();

	//パレット処理が終了したとき
	if (palette_->GetState() == EditorPaletteBase::STATE::NONE)
	{
		ChangePhase(PHASE::RESULT_PHASE);
	}
}

void MultiParty::UpdateResult()
{
	result_->Update(*this);
}

void MultiParty::DrawAction()
{
	//プレイヤーごとにスクリーンを分割して描画
	int playerNum = DateBank::GetInstance().GetPlayerNum();
	for (int i = 0; i < playerNum; i++)
	{
		//スクリーンを設定
		SetDrawScreen(screens_[i]);

		//画面をクリア
		ClearDrawScreen();

		//アイテムの描画
		//ItemManager::GetInstance().Draw();

		//プレイヤーの描画
		//PlayerManager::GetInstance().Draw(i);
	}

	//スクリーンを戻す
	SetDrawScreen(scnMng_.GetMainScreen());

	//全てのスクリーンを描画
	for (const auto& screen : screens_)
	{
		DrawGraph(0, 0, screen, true);

	}
	
}

void MultiParty::DrawEdit()
{
	//プレイヤーごとにスクリーンを分割して描画
	int playerNum = DateBank::GetInstance().GetPlayerNum();
	for (int i = 0; i < playerNum; i++)
	{
		//スクリーンを設定
		SetDrawScreen(screens_[i]);

		//画面をクリア
		ClearDrawScreen();

		//アイテムの描画
		//ItemManager::GetInstance().Draw();

		//エディットコントローラーの描画
		//ItemManager::GetInstance().Draw();
	}

	//スクリーンを戻す
	SetDrawScreen(scnMng_.GetMainScreen());

	//全てのスクリーンを描画
	for (const auto& screen : screens_)
	{
		DrawGraph(0, 0, screen, true);

	}
}

void MultiParty::DrawSelect()
{
	palette_->Draw();
}

void MultiParty::DrawResult()
{
	result_->Draw();
}
