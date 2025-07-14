#pragma once
#include "SceneBase.h"
#include "../Common/Vector2.h"

class SkyDome;

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

	//ロゴのY座標
	static constexpr int LOGO_POS_Y = 250;

	//メッセージのY座標
	static constexpr float MES_POS_Y = 550.0f;

	//タイトル画像
	int imgTitleLogo_;

	//メッセージ画像
	int imgMessage_;

	//BGM
	int bgm_;

	//ステップの更新
	float step_;

	//アルファ値
	int mesAlpha_;

	//アルファ方向
	int alphaDir_;

	//メッセージ座標
	float mesPosY_;

	//スカイドーム
	std::unique_ptr<SkyDome> skyDome_;

	//更新関数
	void NormalUpdate(void) override;

	//描画関数
	void NormalDraw(void) override;

	//処理の変更
	void ChangeNormal(void) override;

	//メッセージの描画
	void DrawMessage(void);
};
