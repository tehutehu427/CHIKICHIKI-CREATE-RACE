#pragma once
#include "GameScene.h"

class SoloChallenge : public GameScene
{
public:

	// コンストラクタ
	SoloChallenge(void);

	// デストラクタ
	~SoloChallenge(void) override;

	//読み込み処理
	void Load(void) override;

	//初期化処理
	void Init(void) override;

	//セレクト画面で選択されたステージを用意する
	//パレットは課題用のを用意

private:

	//描画関数
	void NormalDraw(void) override;

	//アクション時の更新処理
	void UpdateAction(void) override;

	//エディット時の更新処理
	void UpdateEdit(void) override;
};

