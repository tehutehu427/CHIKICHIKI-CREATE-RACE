#pragma once
#include "GameScene.h"
#include "../../Manager/System/SoundManager.h"

class MultiResult;
class RoundDisplay;

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
	void Reset() override;

	/// <summary>
	/// ラウンドリセット
	/// </summary>
	void RoundReset();

private:

	//ラウンド遷移タイム
	static constexpr float ROUND_CHANGE_TIME = 3.5f;
	//エディットからの遷移時間
	static constexpr float EDIT_CHANGE_TIME = 2.0f;
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
	void ChangePhaseEdit() override;
	void ChangePhaseAction() override;
	void ChangePhaseClear() override;	
	void ChangePhaseRound();
	void ChangePhaseSelect();
	void ChangePhaseResult();

	//状態別更新処理
	void UpdateClear() override;
	void UpdateRound();
	void UpdateSelect();
	void UpdateResult();

	//状態別描画処理
	void DrawRound();
	void DrawSelect();
	void DrawResult();

	//プレイヤーらの処理が終えたか調べる
	void CheckPlayerFinish() override;

	//サウンド読み込み
	void LoadSound() override;

	//ランダムBGMを取得
	void RandomBgm();

	//デバッグ処理
	void DebagUpdate() override;

	//エディットBGM種類
	SoundManager::SRC editBgmSrc_;

	//プレイBGM種類
	SoundManager::SRC playBgmSrc_;

	//フェーズ遷移タイマー更新
	float phaseChangeTimer_;

	//エディットからのフェーズ遷移時間管理
	float editChengeTime_;
	//リザルト処理
	std::unique_ptr<MultiResult> result_;
	std::unique_ptr<RoundDisplay> round_;
	std::map<int, std::function<std::vector<std::shared_ptr<Camera>>(void)>>createCamera_;
};

