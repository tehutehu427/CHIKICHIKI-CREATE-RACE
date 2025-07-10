#pragma once
#include <memory>
#include <functional>
#include"../Object/Player/Player.h"
#include "../SceneBase.h"

class EditController;
class EditorPaletteBase;
class Grid;
class SkyDome;
class GameClear;
class MapDataIO;

class GameScene : public SceneBase
{

public:

	static constexpr VECTOR ACTION_CAMERA_POS = { 2000.0f, 1700.0f, -2000.0f };	//アクション時のカメラ位置
	static constexpr float ACTION_START_TIME = 3.0f;	//アクション開始時のカウント

	/// <summary>
	/// フェーズ
	/// </summary>
	enum class PHASE
	{		
		ROUND_PHASE,	//ラウンドを表示
		SELECT_PHASE,	//選択(マルチのみ)
		EDIT_PHASE,		//エディット
		ACTION_PHASE,	//アクション
		RESULT_PHASE,	//リザルト(マルチのみ)	
		CLEAR_PHASE,	//クリア
		GAME_OVER,		//ゲームオーバー
	};


	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name=""></param>
	GameScene(void);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	virtual ~GameScene(void);

	/// <summary>
	/// 読み込み処理
	/// </summary>
	/// <param name=""></param>
	virtual void Load(void) override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name=""></param>
	virtual void Init(void) override;	
	
	/// <summary>
	/// フェーズ遷移
	/// </summary>
	/// <param name="phase">フェーズ</param>
	void ChangePhase(const PHASE phase);

	/// <summary>
	/// リセット
	/// </summary>
	virtual void Reset();

	/// <summary>
	/// フェーズを取得する
	/// </summary>
	/// <returns>現在のフェーズ</returns>
	const PHASE GetPhase() const { return phase_; }

protected:

	//フェーズ管理
	PHASE phase_;

	//エディットコントローラー
	std::vector<std::unique_ptr<EditController>> editControllers_;

	//グリッド
	std::unique_ptr<Grid>grid_;

	//スカイドーム
	std::unique_ptr<SkyDome> sky_;

	//ゲームクリア
	std::unique_ptr<GameClear> gameClear_;

	//パレット
	std::unique_ptr<EditorPaletteBase> palette_;

	//マップ情報の入出力
	std::unique_ptr<MapDataIO> mapIO_;
	
	float actionStartTime_;	//アクション開始のカウントダウン

	//フェーズ管理(遷移時の初期処理)
	std::map<PHASE, std::function<void(void)>> phaseChanges_;

	//フェーズ管理(更新ステップ)
	std::function<void(void)> phaseUpdate_;
	std::function<void(void)> phaseDraw_;	
	
	//グリッドを表示するかプレイヤー別
	std::vector<bool> isGrid_;

	//更新関数
	virtual void NormalUpdate(void) override;

	//通常時の描画処理
	virtual void NormalDraw(void) override;	
	
	//処理の変更
	void ChangeNormal(void) override;	
	
	//フェーズ遷移
	virtual void ChangePhaseEdit(void);
	virtual void ChangePhaseAction(void);
	void ChangePhaseClear(void);
	
	//フェーズ別更新処理
	virtual void UpdateEdit(void);		//エディット時の更新処理
	virtual void UpdateAction(void);	//アクション時の更新処理
	virtual void UpdateClear(void);		//クリア時の更新処理

	//フェーズ別描画処理
	virtual void DrawEdit();
	virtual void DrawAction();
	void DrawClear();

	//プレイヤーがゲームを終了か調べる
	virtual void CheckPlayerFinish(void);

	//デバッグ時の更新処理
	virtual void DebagUpdate(void);	

	//デバッグ時の描画処理
	virtual void DebagDraw(void);	//描画


};