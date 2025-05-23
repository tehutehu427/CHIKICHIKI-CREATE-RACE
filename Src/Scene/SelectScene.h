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

	enum class UPD_STATE
	{
		NONE,
		SELECT,	//選択
		ROTATE,	//回転
	};

	//セレクトメニューの項目数
	static constexpr int MENU_LIST_NUM = static_cast<int>(SELECT_MENU::MAX);	//項目数

	//円弧の描画数
	static constexpr int DRAW_ARC_NUM = 4;	//描画数

	//円弧の回転
	static constexpr int ROTATE_CENTER_X = 0;
	static constexpr int ROTATE_CENTER_Y = Application::SCREEN_HALF_Y;
	static constexpr float ROTATE_STEP = DX_PI_F / 2.0f;	// 90度
	static constexpr float ROTATE_SPEED = 0.1f;				// 回転アニメ速度
	static constexpr float ORBIT_RADIUS = 300.0f;			// 中心からの半径

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
		int img = 0;				//画像
		Vector2 pos = {};			//座標
		float angle = 0.0f;			//角度
	};

	//画像のハンドル
	int *imgArcs_;	

	//移動角度
	float currentAngle_;

	//ターゲット角度
	float targetAngle_;

	//円弧インデックス
	int arcIndex_;

	//選択メニューインデックス
	int menuIndex_;			
	
	//状態
	UPD_STATE updState_;	

	//円弧の配列
	Arc arc_[DRAW_ARC_NUM];
	
	//選択したモード
	std::string selectType_[MENU_LIST_NUM];	

	//更新関数
	std::function<void()> selectUpdateFunc_;

	//状態変更処理の管理
	std::unordered_map<UPD_STATE, std::function<void()>> stateMap_;	
	std::unordered_map <SELECT_MENU, std::function<void()>> menuFuncTable_;

	//状態の処理を登録
	void RegisterArcState(const UPD_STATE _state, std::function<void()> _update);

	//状態変更
	void ChangeUpdateState(const UPD_STATE _state);

	//UIの回転
	void ArcRotation(const int _index);

	//更新関数
	void NormalUpdate() override;

	//描画関数
	void NormalDraw() override;

	//処理の変更
	void ChangeNormal() override;	

	//円弧の状態別更新
	void SelectUpdate();	//待機
	void RotateUpdate();	//回転

	//次に表示される円弧にメニュー項目を与える
	void SetMenuItem(const int _imgIndex,const int _arcIndex);
	
	//デバッグ
	void DebugUpdate();	//更新
	void DebugDraw();	//描画

};

