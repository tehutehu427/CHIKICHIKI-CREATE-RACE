#pragma once
#include "SceneBase.h"


class TitleScene : public SceneBase
{

public:

	// コンストラクタ
	TitleScene(void);

	// デストラクタ
	~TitleScene(void) override;

	//読み込み処理
	void Load(void) override;

	//初期化処理
	void Init(void) override;

private:

	//タイトル画像
	int imgTitleLogo_;

	//更新関数
	void NormalUpdate(void) override;

	//描画関数
	void NormalDraw(void) override;

	//処理の変更
	void ChangeNormal(void) override;
};
