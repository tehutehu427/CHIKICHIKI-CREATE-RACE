#pragma once
#include <unordered_map>
#include "../Common/Vector2.h"
#include "SceneBase.h"

class MultiReady;
class ModeSelect;
class SelectStage;
class SkyDome;
class SystemSetting;

class SelectScene : public SceneBase
{

public:

	//セレクトメニュー
	enum class SELECT_MENU
	{
		SOLO,		//ソロ課題モード
		FREE,		//フリープレイモード	
		MULTI,		//マルチプレイモード
		SETTING,	//設定
		EXIT,		//終了
		MAX,		//最大値
	};

	//更新処理
	enum class STATE
	{
		SELECT_MENU,	//メニュー選択
		SELECT_STAGE,	//ステージ選択
		CHECK_PLAYER,	//プレイヤー確認
		SETTING,		//設定
	};	

	//メッセージ種類
	enum class SELECT_MES
	{
		MODE_SELECT,	//モードセレクト
		STAGE_SELECT,	//ステージセレクト
		PLAYER_NUM,		//プレイヤー人数
		PUSH_BUTTON,	//ボタン
		GAME_START,		//ゲーム開始
		MAX
	};

	//セレクトメニューの項目数
	static constexpr int MENU_LIST_NUM = static_cast<int>(SELECT_MENU::MAX);	//項目数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SelectScene(void);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SelectScene(void) override;

	/// <summary>
	/// 読み込み中
	/// </summary>
	void Load(void) override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init(void) override;

	/// <summary>
	/// メニュー項目を実行
	/// </summary>
	/// <param name="_menu">メニュー内容</param>
	void ProcessMenuFunction(const SELECT_MENU _menu);

	/// <summary>
	/// 状態遷移
	/// </summary>
	/// <param name="state_">変更先の状態</param>
	const void ChangeState(const STATE _state) { state_ = _state; }

private:

	//状態
	STATE state_;

	//メニュー項目を管理するテーブル
	std::unordered_map <SELECT_MENU, std::function<void(void)>> menuFuncTable_;

	//状態別処理を管理するテーブル
	std::unordered_map <STATE, ProcessFunction> funcTable_;

	//モードセレクト
	std::unique_ptr<ModeSelect> modeSelect_;

	//マルチ準備
	std::unique_ptr<MultiReady> multiReady_;

	//ステージセレクト
	std::unique_ptr<SelectStage> selectStage_;

	//システム設定
	std::unique_ptr<SystemSetting> systemSetting_;

	//スカイドーム
	std::unique_ptr<SkyDome> skyDome_;

	//処理の登録
	void RegisterProcessFunc(const STATE _state, ProcessFunction _funcs);

	//更新関数
	void NormalUpdate(void) override;

	//状態別更新関数
	void UpdateSelectMenu(void);
	void UpdateSelectStage(void);
	void UpdateCheckPlayer(void);
	void UpdateSetting(void);

	//状態別描画関数
	void DrawSelectMenu(void);
	void DrawSelectStage(void);
	void DrawCheckPlayer(void);
	void DrawSetting(void);

	//描画関数
	void NormalDraw(void) override;

	//処理の変更
	void ChangeNormal(void) override;
};

