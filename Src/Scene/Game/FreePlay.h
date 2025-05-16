#pragma once
#include "GameScene.h"

class FreePlay : public GameScene
{
public:

	// コンストラクタ
	FreePlay(void);

	// デストラクタ
	~FreePlay(void) override;

	//読み込み処理
	void Load(void) override;

	//初期化処理
	void Init(void) override;
};

