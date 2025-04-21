#pragma once
#include <memory>
#include"../Object/Player/Player.h"
#include "SceneBase.h"

class EditerPaletteBase;


class GameScene : public SceneBase
{

public:
	
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

	//プレイヤー
	std::unique_ptr<Player>player_;
	std::unique_ptr<EditerPaletteBase> palette_;

};