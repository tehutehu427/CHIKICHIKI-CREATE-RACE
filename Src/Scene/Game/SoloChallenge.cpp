#include "SoloChallenge.h"
#include "../Manager/System/SoundManager.h"
#include "../Manager/System/DateBank.h"
#include "../Manager/Game/EventManager.h"
#include "../Manager/Game/PostEffectManager.h"
#include "../Object/System/Select/SelectStage.h"

SoloChallenge::SoloChallenge(void)
{
}

SoloChallenge::~SoloChallenge(void)
{
	PostEffectManager::GetInstance().Destroy();
	EventManager::GetInstance().Destroy();
}

void SoloChallenge::Load(void)
{
	//親クラスの読み込み処理を呼ぶ
	GameScene::Load();	
	
	//ポストエフェクトクラス生成
	PostEffectManager::CreateInstance();

	//イベントクラス生成
	EventManager::CreateInstance();

	//BGMボリュームを設定
	sndMng_.SetLoadedSoundsVolume();
}

void SoloChallenge::Init(void)
{
	//親クラスの初期化処理を呼ぶ	
	GameScene::Init();

	//イベント初期化
	EventManager::GetInstance().Init();

	//ポストエフェクト初期化
	PostEffectManager::GetInstance().Init();

	//アクションフェーズへ移る
	ChangePhase(PHASE::ACTION_PHASE);

	//サウンド再生
	sndMng_.Play(SoundManager::SRC::PLAY_BGM_1, SoundManager::PLAYTYPE::LOOP);

	//ステージ別イベント処理
	SetEventByStage();
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

	//イベント処理の更新処理
	if (actionStartTime_ <= 0.0f)
	{
		EventManager::GetInstance().Update();
	}
}

void SoloChallenge::DrawAction(void)
{
	//親クラスのアクション時の描画処理を呼ぶ
	GameScene::DrawAction();

	//イベントクラス
	PostEffectManager::GetInstance().Draw();
}

void SoloChallenge::LoadSound(void)
{
	GameScene::LoadSound();
	sndMng_.LoadResource(SoundManager::SRC::PLAY_BGM_1);
}

void SoloChallenge::SetEventByStage()
{
	int index = DateBank::GetInstance().GetStageNo();
	auto& eventMng = EventManager::GetInstance();

	switch (static_cast<SelectStage::STAGE_TYPE>(index))
	{
	case SelectStage::STAGE_TYPE::ADVANCE:
		eventMng.SetEventType(EventManager::EVENT_TYPE::SAND_STORM);
		break;

	case SelectStage::STAGE_TYPE::OBLIVION:
		eventMng.SetEventType(EventManager::EVENT_TYPE::FISH_EYE);
		break;

	default:
		break;
	}
}
