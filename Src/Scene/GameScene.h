#pragma once
#include <memory>
#include <functional>
#include"../Object/Player/Player.h"
#include"../Object/Editor/EditController.h"
#include "SceneBase.h"

class EditorPaletteBase;
class Grid;
class SkyDome;

class GameScene : public SceneBase
{

public:
	enum class PHASE
	{
		EDIT_PHASE,
		ACTION_PHASE
	};

	//プレイヤー人数(のちにデータバンクで持ってくる)
	static constexpr int PLAYER_NUM = 2;

	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void);

	//読み込み処理
	void Load(void) override;

	//初期化処理
	void Init(void) override;

private:

	//更新関数
	void NormalUpdate(void) override;

	//描画関数
	void NormalDraw(void) override;

	//処理の変更
	void ChangeNormal(void) override;
	
	//デバッグ処理
	void DebagUpdate(void);
	void DebagDraw(void);

	//フェーズ管理
	PHASE phase_;
	//フェーズ管理(遷移時の初期処理)
	std::map<PHASE, std::function<void(void)>> phaseChanges_;
	//フェーズ管理(更新ステップ)
	std::function<void(void)> phaseUpdate_;

	// フェーズ遷移
	void ChangePhase(PHASE phase);
	void ChangePhaseEdit(void);
	void ChangePhaseAction(void);
	// 更新ステップ
	void UpdateEdit(void);
	void UpdateAction(void);
	//プレイヤー
	std::unique_ptr<EditorPaletteBase> palette_;

	std::unique_ptr<Grid>grid_;

	std::shared_ptr<EditController> editController_;

	std::unique_ptr<SkyDome> sky_;
};