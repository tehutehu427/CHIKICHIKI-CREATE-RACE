#pragma once
#include "GameScene.h"

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

private:

	//ラウンド遷移タイム
	static constexpr float ROUND_CHANGE_TIME = 3.5f;

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
	void ChangePhaseRound();
	void ChangePhaseSelect();
	void ChangePhaseResult();

	//状態別更新処理
	void UpdateRound();
	void UpdateSelect();
	void UpdateResult();

	//状態別描画処理
	void DrawRound();
	void DrawSelect();
	void DrawResult();

	//デバッグ処理
	void DebagUpdate() override;

	//フェーズ遷移タイマー更新
	float phaseChangeTimer_;

	//リザルト処理
	std::unique_ptr<MultiResult> result_;
	std::unique_ptr<RoundDisplay> round_;
	std::map<int, std::function<std::vector<std::shared_ptr<Camera>>(void)>>createCamera_;
};

