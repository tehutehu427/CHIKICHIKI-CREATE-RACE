#pragma once
#include <memory>
#include <functional>
#include"../Object/Player/Player.h"
#include"../Object/Editor/EditController.h"
#include "../SceneBase.h"

class EditorPaletteBase;
class Grid;
class SkyDome;

class GameScene : public SceneBase
{

public:

	/// <summary>
	/// フェーズ
	/// </summary>
	enum class PHASE
	{
		EDIT_PHASE,		//エディット
		ACTION_PHASE,	//アクション
	};

	//プレイヤー人数(のちにデータバンクで持ってくる
	static constexpr int PLAYER_NUM = 2;

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
	/// デバッグ時の更新処理
	/// </summary>
	/// <param name=""></param>
	virtual void DebagUpdate(void);	

	/// <summary>
	/// デバッグ時の描画処理
	/// </summary>
	/// <param name=""></param>
	virtual void DebagDraw(void);	//描画

private:

	//フェーズ管理
	PHASE phase_;

	//フェーズ管理(遷移時の初期処理)
	std::map<PHASE, std::function<void(void)>> phaseChanges_;

	//フェーズ管理(更新ステップ)
	std::function<void(void)> phaseUpdate_;
	
	//更新関数
	void NormalUpdate(void) override;

	//処理の変更
	void ChangeNormal(void) override;

	std::function<void(void)> phaseDraw_;

	// フェーズ遷移
	void ChangePhase(PHASE phase);
	void ChangePhaseEdit(void);
	void ChangePhaseAction(void);

	void DrawEdit();
	void DrawAction();

	//プレイヤー
	std::unique_ptr<EditorPaletteBase> palette_;

	//グリッド
	std::unique_ptr<Grid>grid_;

	//エディットコントローラー
	std::shared_ptr<EditController> editController_;

	//スカイドーム
	std::unique_ptr<SkyDome> sky_;
};