#pragma once
#include "GameScene.h"

class MapDataIO;

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

	//マップ情報の入出力
	std::unique_ptr<MapDataIO> mapIO_;

	//アクション時の更新処理
	void UpdateAction(void) override;

	//エディット時の更新処理
	void UpdateEdit(void) override;	
	
	//描画関数
	void NormalDraw(void) override;

};

