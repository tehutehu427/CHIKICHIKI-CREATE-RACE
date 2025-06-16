#pragma once
#include "GameScene.h"

class MultiResult;

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

	//描画関数
	void NormalDraw(void) override;

	//アクション時の更新処理
	void UpdateAction(void) override;

	//エディット時の更新処理
	void UpdateEdit(void) override;

	//状態遷移
	void ChangePhaseEdit() override;
	void ChangePhaseAction() override;
	void ChangePhaseSelect();
	void ChangePhaseResult();

	//状態別更新処理
	void UpdateSelect();
	void UpdateResult();

	//状態別描画処理
	void DrawSelect();
	void DrawResult();

	//デバッグ処理
	void DebagUpdate() override;

	//リザルト処理
	std::unique_ptr<MultiResult> result_;
	std::map<int, std::function<std::vector<std::shared_ptr<Camera>>(void)>>createCamera_;
};

