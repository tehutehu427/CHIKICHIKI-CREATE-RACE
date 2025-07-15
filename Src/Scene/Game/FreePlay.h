#pragma once
#include "GameScene.h"

class CheckChangePhase;
class ManualTab;
class EditEscape;

class FreePlay : public GameScene
{
public:

	// コンストラクタ
	FreePlay(void);

	// デストラクタ
	~FreePlay(void) override;

	//読み込み処理
	void Load(void) override;

	//初期化処理
	void Init(void) override;

	//プレイヤーは一人のみ生成
	//マップデータの入出力はここでのみ使う

private:

	//フェーズをプレイに変える
	std::unique_ptr<CheckChangePhase> checkChangePhase_;
	
	//マニュアル
	std::unique_ptr<ManualTab> manual_;

	//編集をやめる
	std::unique_ptr<EditEscape> editEscape_;

	//アクション時の更新処理
	void UpdateAction(void) override;

	//エディット時の更新処理
	void UpdateEdit(void) override;		
	
	//アクションへの状態遷移
	void ChangePhaseAction(void);	
	
	//エディットへ状態遷移
	void ChangePhaseEdit(void);

	//更新関数
	void NormalUpdate() override;

	//描画関数
	void NormalDraw(void) override;

	//エディット中の描画
	void DrawEdit() override;

	//音源の読み込み
	void LoadSound() override;

};

