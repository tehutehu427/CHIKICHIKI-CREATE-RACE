#include "SoloChallenge.h"

SoloChallenge::SoloChallenge(void)
{
}

SoloChallenge::~SoloChallenge(void)
{
}

void SoloChallenge::Load(void)
{
	//親クラスの読み込み処理を呼ぶ
	GameScene::Load();
}

void SoloChallenge::Init(void)
{
	//親クラスの初期化処理を呼ぶ	
	GameScene::Init();

	//アクションフェーズへ移る
	ChangePhase(PHASE::ACTION_PHASE);
}

void SoloChallenge::NormalDraw(void)
{
	//親クラスの描画処理を呼ぶ
	GameScene::NormalDraw();
}

void SoloChallenge::UpdateAction(void)
{
	//親クラスのアクション時の更新処理を呼ぶ
	GameScene::UpdateAction();
}