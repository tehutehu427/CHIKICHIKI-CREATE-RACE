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

	/// <summary>
	/// フェーズ
	/// </summary>
	enum class PHASE
	{
		EDIT_PHASE,		//エディット
		ACTION_PHASE,	//アクション
		CLEAR_PHASE,	//クリア

		//マルチ限定
		SELECT_PHASE,	//選択
		RESULT_PHASE,	//リザルト
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

protected:

	/// <summary>
	/// 通常時の描画処理
	/// </summary>
	/// <param name=""></param>
	virtual void NormalDraw(void) override;
	
	/// <summary>
	/// エディット時の更新処理
	/// </summary>
	/// <param name=""></param>
	virtual void UpdateEdit(void);

	/// <summary>
	/// アクション時の更新処理
	/// </summary>
	/// <param name=""></param>
	virtual void UpdateAction(void);	

	/// <summary>
	/// クリア時の更新処理
	/// </summary>
	/// <param name=""></param>
	virtual void UpdateClear(void);
	
	/// <summary>
	/// デバッグ時の更新処理
	/// </summary>
	/// <param name=""></param>
	virtual void DebagUpdate(void);	

	/// <summary>
	/// デバッグ時の描画処理
	/// </summary>
	/// <param name=""></param>
	virtual void DebagDraw(void);	//描画

	//エディットコントローラー
	std::shared_ptr<EditController> editController_;

	//パレット
	std::unique_ptr<EditorPaletteBase> palette_;

	//マップ情報の入出力
	std::unique_ptr<MapDataIO> mapIO_;
	
	//フェーズ管理(遷移時の初期処理)
	std::map<PHASE, std::function<void(void)>> phaseChanges_;

	//フェーズ管理(更新ステップ)
	std::function<void(void)> phaseUpdate_;
	std::function<void(void)> phaseDraw_;

private:

	//フェーズ管理
	PHASE phase_;
	
	//更新関数
	void NormalUpdate(void) override;

	//処理の変更
	void ChangeNormal(void) override;

	//フェーズ遷移
	void ChangePhase(PHASE phase);
	void ChangePhaseEdit(void);
	void ChangePhaseAction(void);
	void ChangePhaseClear(void);

	//クリア描画
	virtual void DrawEdit();
	virtual void DrawAction();
	void DrawClear();

	//プレイヤーがクリアオブジェクトに立った後の遷移
	void ChangePlayerClearPhase(void);

	//グリッド
	std::unique_ptr<Grid>grid_;

	//スカイドーム
	std::unique_ptr<SkyDome> sky_;

	//ゲームクリア
	std::unique_ptr<GameClear> gameClear_;

	//エディター用UI
	std::unique_ptr<EditorUi> editorUi_;
};