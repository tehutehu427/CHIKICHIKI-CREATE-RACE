#include "FreePlay.h"
#include "../../Object/Editor/MapDataIO.h"
#include "../../Object/Editor/Palette/EditorPaletteBase.h"
#include "../../Object/System/CheckChangePhase.h"

FreePlay::FreePlay(void)
{
	changePhasePlay_ = nullptr;
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

	//フェーズ遷移
	changePhasePlay_ = std::make_unique<CheckChangePhase>();
	changePhasePlay_->Load();
}

void FreePlay::Init(void)
{
	//親クラスの初期化
	GameScene::Init();

	//初期化
	palette_->Init();

	//初期化
	changePhasePlay_->Init();

	//マップデータの初期化
	mapIO_->Init();

	ChangePhase(PHASE::EDIT_PHASE);
}

void FreePlay::UpdateAction(void)
{
	//親クラスの更新
	GameScene::UpdateAction();
}

void FreePlay::UpdateEdit(void)
{
	//親クラスの更新
	GameScene::UpdateEdit();

	//マップデータの更新
	mapIO_->Update();
}

void FreePlay::ChangePhaseAction(void)
{
	//親クラスの処理を実行
	GameScene::ChangePhaseAction();

	//次のフェーズ遷移の設定
	changePhasePlay_->SetNextPhase(PHASE::EDIT_PHASE);
}

void FreePlay::ChangePhaseEdit(void)
{
	//親クラスの処理を実行
	GameScene::ChangePhaseEdit();

	//次のフェーズ遷移の設定
	changePhasePlay_->SetNextPhase(PHASE::ACTION_PHASE);
}

void FreePlay::NormalUpdate()
{	
	//親クラスの処理を呼び出し
	GameScene::NormalUpdate();

	//フェーズ遷移
	changePhasePlay_->Update(*this);
}

void FreePlay::NormalDraw(void)
{
	//親クラスの描画
	GameScene::NormalDraw();

	//マップデータの描画
	mapIO_->Draw();

	//フェーズ遷移アイコン
	changePhasePlay_->Draw();
}