#pragma once
#include "GameScene.h"
#include "../../Manager/System/SoundManager.h"

class MultiResult;
class RoundDisplay;
class WiggleEffect;

class MultiParty : public GameScene
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name=""></param>
	MultiParty(void);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~MultiParty(void) override;

	/// <summary>
	/// 読み込み
	/// </summary>
	/// <param name=""></param>
	void Load(void) override;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name=""></param>
	void Init(void) override;

	/// <summary>
	/// リセット
	/// </summary>
	void Reset(void) override;

	/// <summary>
	/// 共通部分の描画
	/// </summary>
	/// <param name=""></param>
	void CommonDraw(void) override;

	/// <summary>
	/// ラウンドリセット
	/// </summary>
	void RoundReset(void);

private:

	//ラウンド遷移タイム
	static constexpr float ROUND_CHANGE_TIME = 3.5f;

	//エディットからの遷移時間
	static constexpr float EDIT_CHANGE_TIME = 2.0f;

	//アクションからの遷移時間
	static constexpr float ACTION_CHANGE_TIME = 2.0f;

	//描画関数
	void NormalDraw(void) override;

	//アクション時の更新処理
	void UpdateAction(void) override;

	//エディット時の更新処理
	void UpdateEdit(void) override;

	//あくしょん時の描画処理
	void DrawAction(void) override;

	//エディット時の描画処理
	void DrawEdit(void) override;

	//状態遷移
	void ChangePhaseEdit(void) override;
	void ChangePhaseAction(void) override;
	void ChangePhaseClear(void) override;
	void ChangePhaseRound(void);
	void ChangePhaseSelect(void);
	void ChangePhaseResult(void);

	//状態別更新処理
	void UpdateClear(void) override;
	void UpdateRound(void);
	void UpdateSelect(void);
	void UpdateResult(void);

	//状態別描画処理
	void DrawRound(void);
	void DrawSelect(void);
	void DrawResult(void);

	//プレイヤーらの処理が終えたか調べる
	void CheckPlayerFinish(void) override;

	//サウンド読み込み
	void LoadSound(void) override;

	//ランダムBGMを取得
	void RandomBgm(void);

	//エディットBGM種類
	SoundManager::SRC editBgmSrc_;

	//プレイBGM種類
	SoundManager::SRC playBgmSrc_;

	//フェーズ遷移タイマー更新
	float phaseChangeTimer_;

	//エディットからのフェーズ遷移時間管理
	float editChangeTime_;

	//アクションからのフェーズ遷移時間管理
	float actionChangeTime_;

	//リザルト処理
	std::unique_ptr<MultiResult> result_;
	std::unique_ptr<RoundDisplay> round_;
	std::map<int, std::function<std::vector<std::shared_ptr<Camera>>(void)>>createCamera_;
};