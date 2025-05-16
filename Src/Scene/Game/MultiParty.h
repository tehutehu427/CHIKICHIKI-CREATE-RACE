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
};

