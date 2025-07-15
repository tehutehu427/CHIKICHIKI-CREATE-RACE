#include "SoloChallenge.h"
#include "../Manager/System/SoundManager.h"

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

	//BGMボリュームを設定
	sndMng_.SetLoadedSoundsVolume();
}

void SoloChallenge::Init(void)
{
	//親クラスの初期化処理を呼ぶ	
	GameScene::Init();

	//アクションフェーズへ移る
	ChangePhase(PHASE::ACTION_PHASE);

	sndMng_.Play(SoundManager::SRC::PLAY_BGM_1, SoundManager::PLAYTYPE::LOOP);
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

void SoloChallenge::LoadSound(void)
{
	GameScene::LoadSound();
	sndMng_.LoadResource(SoundManager::SRC::PLAY_BGM_1);
}
