#pragma once
#include "../Common/Vector2.h"
#include "SceneBase.h"

class ModeSelect;

class SelectScene : public SceneBase
{
public:

	// コンストラクタ
	SelectScene();

	// デストラクタ
	~SelectScene() override;

	//読み込み処理
	void Load() override;

	//初期化処理
	void Init() override;

private:

	//モードセレクト
	std::unique_ptr<ModeSelect> modeSelect_;

	//更新関数
	void NormalUpdate() override;

	//描画関数
	void NormalDraw() override;

	//処理の変更
	void ChangeNormal() override;	

	//デバッグ
	void DebugUpdate();	//更新
	void DebugDraw();	//描画

};

