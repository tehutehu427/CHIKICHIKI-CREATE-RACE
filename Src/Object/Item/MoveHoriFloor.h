#pragma once
#include "ItemBase.h"

class MoveHoriFloor : public ItemBase
{
public:

	//マップサイズ
	static constexpr IntVector3 MAP_SIZE = { 4,1,4 };

	//横移動距離
	static constexpr int MOVE_X = 8;

	//速度
	static constexpr float SPEED = 5.0f;

	//コンストラクタ
	MoveHoriFloor();
	//デストラクタ
	~MoveHoriFloor()override;

	//読み込み
	void Load(void)override;
	//初期化
	void Init(void)override;
	//個人設定
	void SetParam(void)override;
	//更新
	void Update(void)override;
	//描画
	void Draw(void)override;
	//解放
	void Release(void)override;

private:

	//移動
	void Move(void);

};

