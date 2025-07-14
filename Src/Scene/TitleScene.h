#pragma once
#include <functional>
#include "SceneBase.h"
#include "../Common/Vector2.h"

class SkyDome;

class TitleScene : public SceneBase
{
public:

	// コンストラクタ
	TitleScene();

	// デストラクタ
	~TitleScene() override;

	//読み込み処理
	void Load() override;

	//初期化処理
	void Init() override;

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

	//更新処理の管理
	std::function<void()> titleUpdateFunc_;

	//スカイドーム
	std::unique_ptr<SkyDome> skyDome_;

	//更新関数
	void NormalUpdate() override;

	//描画関数
	void NormalDraw() override;

	//処理の変更
	void ChangeNormal() override;

	//メッセージの描画
	void DrawMessage();

	//状態別更新処理
	void UpdateWait();
	void UpdatePlaySe();
	void UpdateNone();
};
