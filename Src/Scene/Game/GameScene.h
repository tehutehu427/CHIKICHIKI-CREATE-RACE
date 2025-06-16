#pragma once
#include <memory>
#include <functional>
#include"../Object/Player/Player.h"
#include"../Object/Editor/EditController.h"
#include "../SceneBase.h"

class EditorPaletteBase;
class Grid;
class SkyDome;
class GameClear;
class MapDataIO;
class EditorUi;

class GameScene : public SceneBase
{

public:

	static constexpr VECTOR ACTION_CAMERA_POS = { 2000.0f, 1700.0f, -2000.0f };	//アクション時のカメラ位置

	static constexpr int PLAYER_NUM = 2;

	/// <summary>
	/// フェーズ
	/// </summary>
	enum class PHASE
	{		
		SELECT_PHASE,	//選択(マルチのみ)
		EDIT_PHASE,		//エディット
		ACTION_PHASE,	//アクション
		RESULT_PHASE,	//リザルト(マルチのみ)	
		CLEAR_PHASE,	//クリア
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
	
	//フェーズ遷移
	void ChangePhase(const PHASE phase);

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

	//エディター用UI
	std::unique_ptr<EditorUi> editorUi_;

	//パレット
	std::unique_ptr<EditorPaletteBase> palette_;

	//マップ情報の入出力
	std::unique_ptr<MapDataIO> mapIO_;
	
	//フェーズ管理(遷移時の初期処理)
	std::map<PHASE, std::function<void(void)>> phaseChanges_;

	//フェーズ管理(更新ステップ)
	std::function<void(void)> phaseUpdate_;
	std::function<void(void)> phaseDraw_;	
	
	//更新関数
	void NormalUpdate(void) override;

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

	//プレイヤーがクリアオブジェクトに当たった後の遷移
	void ChangePlayerClearPhase(void);

	//デバッグ時の更新処理
	virtual void DebagUpdate(void);	

	//デバッグ時の描画処理
	virtual void DebagDraw(void);	//描画


};