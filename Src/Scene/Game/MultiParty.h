#pragma once
#include "GameScene.h"

class MultiResult;
class RoundDisplay;

class MultiParty : public GameScene
{
public:

	// コンストラクタ
	MultiParty(void);

	// デストラクタ
	~MultiParty(void) override;

	//読み込み処理
	void Load(void) override;

	//初期化処理
	void Init(void) override;

	//プレイヤーやカメラは人数分用意する
	//描画処理を画面分割して行う
	//パレットをマルチ仕様のを呼び出す


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

