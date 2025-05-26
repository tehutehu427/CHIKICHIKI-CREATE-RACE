#pragma once
#include "GameScene.h"

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
};

