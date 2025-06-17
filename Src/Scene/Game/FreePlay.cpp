#include "FreePlay.h"
#include "../../Object/Editor/MapDataIO.h"
#include "../../Object/Editor/Palette/EditorPaletteBase.h"

FreePlay::FreePlay(void)
{
}

FreePlay::~FreePlay(void)
{
}

void FreePlay::Load(void)
{
	//親クラスの読み込み
	GameScene::Load();

	//パレットの生成
	palette_ = std::make_unique<EditorPaletteBase>(editControllers_);
	palette_->Load();
}

void FreePlay::Init(void)
{
	//親クラスの初期化
	GameScene::Init();

	//初期化
	palette_->Init();

	//マップデータの初期化
	mapIO_->Init();

	ChangePhase(PHASE::EDIT_PHASE);
}

void FreePlay::UpdateAction(void)
{
	//親クラスの更新
	GameScene::UpdateAction();

	//フェーズ遷移
	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_Z))
	{
		ChangePhase(PHASE::EDIT_PHASE);
	}
}

void FreePlay::UpdateEdit(void)
{
	//親クラスの更新
	GameScene::UpdateEdit();

	//フェーズ遷移
	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_Z))
	{
		ChangePhase(PHASE::ACTION_PHASE);
	}

	//マップデータの更新
	mapIO_->Update();
}

void FreePlay::NormalDraw(void)
{
	//親クラスの描画
	GameScene::NormalDraw();

	//マップデータの描画
	mapIO_->Draw();
}