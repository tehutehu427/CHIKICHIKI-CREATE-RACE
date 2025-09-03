#pragma once
#include <functional>
#include "SceneBase.h"
#include "../Common/Vector2.h"

class SkyDome;
class DemoPlay;

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

	//デモプレイ
	static constexpr float DEMO_CHANGE_TIME = 10.0f;	//デモプレイへの遷移時間
	static constexpr int DEMO_IMAGE_ALPHA = 100;		//デモプレイでの画像の透明度
	static constexpr float DEMO_LOGO_MOVE_TIME = 2.0f;	//デモプレイでのロゴの移動時間
	static constexpr float DEMO_CHANGE_SIZE = 16.0f;	//デモプレイでのロゴの縮小倍率

	//ロゴ
	static constexpr int LOGO_POS_Y = 250;				//ロゴの基本Y座標
	static constexpr float LOGO_MIN_SIZE = 0.5f;		//ロゴの最小サイズ
	static constexpr int LOGO_MIN_POS_X = 140;			//ロゴの最小X座標
	static constexpr int LOGO_MIN_POS_Y = 120;			//ロゴの最小Y座標

	//メッセージ
	static constexpr float MES_POS_Y = 550.0f;			//メッセージのY座標
	static constexpr float DEMO_MES_POS_X = 600.0f;		//デモメッセージのX座標
	static constexpr float DEMO_MES_POS_Y = 750.0f;		//デモメッセージのY座標

	//風船
	static constexpr int BALLOON_NUM = 20;				//風船を描画する最大数
	static constexpr int BALLOON_TYPE = 8;				//風船の種類
	static constexpr int BALLOON_SIZE_ONE_X = 160;				//風船1つのサイズX
	static constexpr int BALLOON_SIZE_ONE_HALF_X = 80;				//風船1つのサイズX
	static constexpr int BALLOON_SIZE_ONE_Y = 240;				//風船1つのサイズY
	static constexpr int BALLOON_SIZE_ONE_HALF_Y = 120;				//風船1つのサイズY
	static constexpr int BALLOON_NUM_X = 4;			//風船のXの個数
	static constexpr int BALLOON_NUM_Y = 2;			//風船のYの個数
	static constexpr int BALLOON_POS_MAX_Y = -BALLOON_SIZE_ONE_Y;
	static constexpr float BALLOON_STEP_MAX = 1.0f;//風船の出現間隔
	static constexpr int SPEED_MIN = 2;//風船の出現間隔
	static constexpr int SPEED_MAX = 5;//風船の出現間隔

	//タイトル画像
	int imgTitleLogo_;

	//タイトルロゴサイズ
	float logoSize_;

	//メッセージ画像
	int imgMessage_;

	//デモメッセージ画像
	int imgDemoMessage_;

	//BGM
	int bgm_;

	//風船画像
	int* imgBalloons_;					//風船の複数画像
	int balloonType_[BALLOON_NUM];		//風船の種類
	bool isBalloonAlive_[BALLOON_NUM];	//風船の生存判定
	float balloonStep_;					//風船の間隔カウント
	Vector2 balloonPos_[BALLOON_NUM];	//風船座標
	int speed_[BALLOON_NUM];

	//ステップの更新
	float step_;
	float demoUIStep_;

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

	//デモ
	std::unique_ptr<DemoPlay>demo_;

	//更新関数(通常)
	void NormalUpdate(void) override;

	//更新関数(デモプレイ)
	void DemoUpdate(void);

	//描画関数(通常)
	void NormalDraw(void) override;

	//描画関数(デモプレイ)
	void DemoDraw(void);

	//処理の変更(通常)
	void ChangeNormal(void) override;

	//処理の変更(デモプレイ)
	void ChangeDemo(void);

	//メッセージの描画
	void DrawMessage(void);

	//デモプレイのメッセージ描画
	void DemoMessage(void);

	//状態別更新処理
	void UpdateWait();
	void UpdatePlaySe();
	void UpdateNone();

	//風船の更新
	void BalloonUpdate(void);

	//風船描画
	void DrawBalloon(void);
};
