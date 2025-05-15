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

private:


};

