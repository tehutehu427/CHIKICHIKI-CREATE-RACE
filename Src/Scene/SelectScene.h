#pragma once
#include "../Common/Vector2.h"
#include "SceneBase.h"

class SelectScene : public SceneBase
{
public:

	//セレクトメニュー
	enum class SELECT_MENU
	{
		SOLO,		//ソロ課題モード
		FREE,		//フリープレイモード	
		MULTI,		//マルチプレイモード
		SETTINGS,	//設定
		EXIT,		//終了
		MAX,		//最大値
	};

	//セレクトメニューの項目数
	static constexpr int MENU_LIST_NUM = static_cast<int>(SELECT_MENU::MAX);	//項目数

	//円弧の描画数
	static constexpr int DRAW_ARC_NUM = 4;	//描画数

	// コンストラクタ
	SelectScene();

	// デストラクタ
	~SelectScene() override;

	//読み込み処理
	void Load() override;

	//初期化処理
	void Init() override;

private:

	//円弧
	struct Arc
	{
		int img;		//画像
		Vector2 pos;	//座標
		float angle;	//角度
	};
	
	//円弧の配列
	Arc arc_[DRAW_ARC_NUM];

	//UIの回転
	void ArcRotation();

	//更新関数
	void NormalUpdate() override;

	//描画関数
	void NormalDraw() override;

	//処理の変更
	void ChangeNormal() override;	
	
	//デバッグ
	void DebugUpdate();	//更新
	void DebugDraw();	//描画

};

