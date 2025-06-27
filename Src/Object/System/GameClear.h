#pragma once
#include <unordered_map>
#include <functional>
#include <string>
#include <vector>
#include "../../Common/Vector2.h"

class SceneManager;
class KeyConfig;
class MultiParty;
class GameScene;

class GameClear
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameClear();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameClear();

	/// <summary>
	/// 読み込み
	/// </summary>
	void Load();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update(GameScene& _parent);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:

	//状態
	enum class STATE
	{
		NONE,
		DISPLAY_MESSAGE,
		WAITING,
		MENU,
	};

	//メニュー項目
	enum class MENU
	{
		RETRY,
		BACK_SELECT,
		BACK_TITLE,
		MAX
	};

	//メッセージ用フォント
	static constexpr int MES_FONT_SIZE = 72;
	static constexpr int MES_FONT_THICK = 0;

	//メニュー用フォント
	static constexpr int MENU_FONT_SIZE = 32;
	static constexpr int MENU_FONT_THICK = 0;

	//状態変更時間
	static constexpr float CHANGE_TIME_WAITING = 3.0f;

	//メニュー項目数
	static constexpr int MENU_LIST_NUM = static_cast<int>(MENU::MAX);

	//メッセージ位置
	static constexpr int MESSAGE_POS_X = 0;
	static constexpr int MESSAGE_POS_Y = -200;

	//メニューの表示位置
	static constexpr int MENU_POS_X = 0;
	static constexpr int MENU_POS_Y = -200;

	//アニメーション時間
	static constexpr float ANIM_TIME = 2.5f;

	//黒背景アルファ値
	static constexpr int BLACK_BOX_ALPHA = 128;
	static constexpr int ALPHA_SPEED = 3;

	// 状態ごとの構造体（更新と描画を分けて保持）
	struct StateFuncs
	{
		std::function<void(GameScene&)> updateFunc;
		std::function<void()> drawFunc;
	};

	//状態
	STATE state_;

	//状態更新管理
	std::unordered_map<STATE, StateFuncs> stateMap_;

	//メニュー項目別処理
	std::unordered_map<MENU, std::function<void(GameScene&)>> menuFuncTabe_;

	//フォント
	int messageFont_;	//メッセージ
	int menuFont_;		//メニュー

	//画像
	int imgClear_;
	int imgWin_;
	int* imgPlayerPlates_;
	int* imgSelectMenu_;

	//メニュー表示用ステップ
	float waitStep_;

	//メニュー選択用インデックス
	int menuIndex_;

	//画面フェード用アルファ値
	int alpha_;

	//メニュー項目
	std::string menuStrings_[MENU_LIST_NUM];

	//クリアUIの座標
	Vector2 clearPos_;

	//シーン管理
	SceneManager& scnMng_;

	//入力管理
	KeyConfig& keyConfig_;

	//状態別更新処理を登録
	void RegisterStateFunction(const STATE _state, std::function<void(GameScene&)> _update, std::function<void()> _draw);

	//状態変更
	const void ChangeState(const STATE _state) { state_ = _state; }

	//状態別更新処理
	void UpdateWaiting(GameScene& _parent);
	void UpdateDisplay(GameScene& _parent);
	void UpdateMenu(GameScene& _parent);

	//状態別描画処理
	void DrawWaiting();
	void DrawDisplay();
	void DrawMenu();

	//モード別に状態を初期化
	void InitStateByMode();

	//デバッグ用描画
	void DebugDraw();
};

