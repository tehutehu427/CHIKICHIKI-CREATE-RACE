#pragma once
#include<functional>
#include<DxLib.h>
#include "../../../Application.h"
#include "../../../Common/Vector2.h"

class SceneManager;
class DateBank;
class SelectScene;
class SelectUIGlow;
class SelectUIDarkly;
class ManualTab;

class ModeSelect
{
public:

	/// <summary>
	/// 更新状態
	/// </summary>
	enum class UPD_STATE
	{
		NONE,
		SELECT,	//選択
		ROTATE,	//回転
	};

	//円弧の描画数
	static constexpr int DRAW_ARC_NUM = 4;	//描画数

	//円弧の回転
	static constexpr int ROTATE_CENTER_X = 0;
	static constexpr int ROTATE_CENTER_Y = Application::SCREEN_HALF_Y;
	static constexpr float ROTATE_STEP = DX_PI_F / 2.0f;	// 90度
	static constexpr float ROTATE_SPEED = 0.1f;				// 回転アニメ速度
	static constexpr float ORBIT_RADIUS = 300.0f;			// 中心からの半径

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ModeSelect();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ModeSelect();

	/// <summary>
	/// 読み込み
	/// </summary>
	void Load();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(SelectScene& _parent);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private:

	//シーン管理
	SceneManager& scnMng_;

	//データ管理
	DateBank& dateBank_;

	//円弧
	struct Arc
	{
		int img = 0;				//画像
		Vector2 pos = {};			//座標
		float angle = 0.0f;			//角度
	};

	//画像のハンドル
	int imgBackArc_;
	int* imgArcs_;
	int imgShadowArc_;

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

	//状態変更処理の管理
	std::unordered_map<UPD_STATE, std::function<void(SelectScene&)>> stateMap_;

	//更新関数
	std::function<void(SelectScene&)> selectUpdateFunc_;

	//エフェクトグロー
	std::unique_ptr<SelectUIGlow> uiGlow_;

	//エフェクトダークリー
	std::unique_ptr<SelectUIDarkly> uiDarkly_;

	//マニュアル
	std::unique_ptr<ManualTab> manual_;

	//状態の処理を登録
	void RegisterArcState(const UPD_STATE _state, std::function<void(SelectScene&)> _update);

	//状態変更
	void ChangeUpdateState(const UPD_STATE _state);

	//円弧の状態別更新
	void SelectUpdate(SelectScene& _parent);	//待機
	void RotateUpdate(SelectScene&);	//回転

	//次に表示される円弧にメニュー項目を与える
	void SetMenuItem(const int _imgIndex, const int _arcIndex);

	//画像を暗く描画する
	void DrawDarkly(const int _index);

	//画像周りを発光させる
	void DrawGlow(const int _index);

	//デバッグ
	void DebugUpdate();	//更新
	void DebugDraw();	//描画
};

