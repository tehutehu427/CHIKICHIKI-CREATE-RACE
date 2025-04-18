#pragma once
#include "ItemBase.h"

class MoveHoriFloor : public ItemBase
{
public:

	//マップサイズ
	static constexpr IntVector3 MAP_SIZE = { 4,1,4 };

	//横移動距離
	static constexpr int MOVE_X = 8;

	//1地点移動するのにかかる秒数
	static constexpr float ONE_POINT_SEC = 10.0f;

	//ルート数
	static constexpr int ROUTE = 2;

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

	//ルート
	VECTOR route_[ROUTE];
	int routeNum_;

	//移動速度
	float speed_;
	//移動秒数用カウンタ
	float cnt_;
	//ルートまでの距離
	double distance_;

	//移動
	void Move(void);

	//ルート設定
	void SetRoute(void);
};

