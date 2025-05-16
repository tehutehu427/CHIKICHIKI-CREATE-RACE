#include <string>
#include <DxLib.h>
#include <cmath>
#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/System/ResourceManager.h"
#include "../Manager/System/InputManager.h"
#include "SelectScene.h"

SelectScene::SelectScene(void)
{
	//更新関数のセット
	updataFunc_ = std::bind(&SelectScene::LoadingUpdate, this);
	//描画関数のセット
	drawFunc_ = std::bind(&SelectScene::LoadingDraw, this);
}

SelectScene::~SelectScene(void)
{
}

void SelectScene::Load(void)
{
	//フォントの登録
	resMng_.Load(ResourceManager::SRC::DOT_FONT);
	buttnFontHandle_ = CreateFontToHandle("ベストテンDOT", FONT_SIZE, 0);

	//リソースの読み込み
	for (int i = 0; i < DRAW_ARC_NUM; i++)
	{
		arc_[i].img = resMng_.Load(ResourceManager::SRC::ARC).handleId_;
	}
}

void SelectScene::Init(void)
{
	//初期化
	arc_[0].pos = {Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2};
	arc_[0].angle = 0.0f;

	arc_[1].pos = { Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2 };
	arc_[1].angle = 90.0f;

	arc_[2].pos = { Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2 };
	arc_[2].angle = 180.0f;

	arc_[3].pos = { Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2 };
	arc_[3].angle = 270.0f;
}

void SelectScene::ArcRotation()
{
	constexpr float ROTATION_SPEED = 0.05f; // 回転速度
	constexpr int CENTER_RADIUS = 300;		// 中心からの距離

	Vector2 center = { 0, Application::SCREEN_SIZE_Y / 2 };

	arc_[0].pos.x = center.x + std::cos(arc_[0].angle) * CENTER_RADIUS;
	arc_[0].pos.y = center.y + std::sin(arc_[0].angle) * CENTER_RADIUS;

	// 角度を進める
	arc_[0].angle += ROTATION_SPEED;
}

void SelectScene::NormalUpdate(void)
{
#ifdef _DEBUG
	DebugUpdate();
#endif

	//円弧の回転
	ArcRotation();
}

void SelectScene::NormalDraw(void)
{
#ifdef _DEBUG
	DebugDraw();
#endif
}

void SelectScene::ChangeNormal(void)
{
	//処理変更
	updataFunc_ = std::bind(&SelectScene::NormalUpdate, this);
	drawFunc_ = std::bind(&SelectScene::NormalDraw, this);
}

void SelectScene::DebugUpdate()
{
	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}
}

void SelectScene::DebugDraw()
{
	DrawBox(
		0,
		0,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		Utility::CYAN,
		true
	);

	DrawFormatString(
		0,
		0,
		Utility::BLACK,
		"SelectScene"
	);

	/*for (Arc& a : arc_)
	{
		DrawRotaGraph(
			a.pos.x,
			a.pos.y,
			1.0f,
			a.angle,
			a.img,
			true,
			false
		);
	}*/

	DrawRotaGraph(
		arc_[0].pos.x,
		arc_[0].pos.y,
		1.0f,
		arc_[0].angle,
		arc_[0].img,
		true,
		false
	);
}
