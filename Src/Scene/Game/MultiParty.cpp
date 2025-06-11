#include "MultiParty.h"
#include "../../Manager/System/Camera.h"
#include "../../Manager/System/DateBank.h"

MultiParty::MultiParty(void)
{
	phaseChanges_.emplace(PHASE::SELECT_PHASE, std::bind(&MultiParty::ChangePhaseSelect, this));
	phaseChanges_.emplace(PHASE::RESULT_PHASE, std::bind(&MultiParty::ChangePhaseResult, this));
	//スクリーン作成
	{
		makeScreens_[1] = [this]()
			{
				std::vector<int> screens;
				int screen = MakeScreen(Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y, true);
				screens.push_back(screen);
				return screens;
			};
		makeScreens_[2] = [this]()
			{
				std::vector<int> screens;
				for (int i = 0; i < 2; ++i)
				{
					int screen = MakeScreen(Application::SCREEN_HALF_X / 2, Application::SCREEN_HALF_Y, true);
					screens.push_back(screen);
				}
				return screens;
			};
		makeScreens_[3] = [this]()
			{
				std::vector<int> screens;
				for (int i = 0; i < 4; ++i)
				{
					int screen = MakeScreen(Application::SCREEN_HALF_X / 2, Application::SCREEN_HALF_Y / 2, true);
					screens.push_back(screen);
				}
				return screens;
			};
		makeScreens_[4] = [this]()
			{
				std::vector<int> screens;
				for (int i = 0; i < 4; ++i)
				{
					int screen = MakeScreen(Application::SCREEN_HALF_X / 2, Application::SCREEN_HALF_Y / 2, true);
					screens.push_back(screen);
				}
				return screens;
			};
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
}

void MultiParty::Load(void)
{
	//親クラスの読み込み処理を呼ぶ
	GameScene::Load();
	//screens_ = makeScreens_[DateBank::GetInstance().GetPlayerNum()]();
	//cameras_ = createCamera_[DateBank::GetInstance().GetPlayerNum()]();
}

void MultiParty::Init(void)
{
	//親クラスの初期化処理を呼ぶ
	GameScene::Init();

	ChangePhase(PHASE::SELECT_PHASE);
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
}

void MultiParty::UpdateResult()
{
}

void MultiParty::DrawAction()
{
}

void MultiParty::DrawEdit()
{
}

void MultiParty::DrawSelect()
{
}

void MultiParty::DrawResult()
{
}
