#pragma once
#include"ItemBase.h"

class MoveVerFloor : public ItemBase
{
public:

	//マップサイズ
	static constexpr IntVector3 MAP_SIZE = { 4,1,4 };

	//横移動距離
	static constexpr int MOVE_Y = 4;

	//1地点移動するのにかかる秒数
	static constexpr float ONE_POINT_SEC = 5.0f;

	//ルート数
	static constexpr int ROUTE = 2;

	//コンストラクタ
	MoveVerFloor();
	//デストラクタ
	~MoveVerFloor()override;

	//個人設定
	void SetParam(void)override;
	//更新
	void Update(void)override;
	//描画
	void Draw(void)override;

private:

	//ルート
	VECTOR route_[ROUTE];
	VECTOR startRoute_;
	VECTOR goalRoute_;
	int routeNum_;

	//移動速度
	float speed_;
	//ルートまでの距離
	double distance_;
	//移動方向
	VECTOR moveVec_;
	//移動量
	VECTOR movePow_;

	//移動
	void Move(void);

	//ルート初期化
	void InitRoute(void);

	//ルート設定
	void SetRoute(void);

	//ルートを超えたかの判定
	bool IsBeyondRoute(void);

	/// <summary>
	/// 当たった物にも同じ移動量を与える
	/// </summary>
	/// <param name="_hitTrans">当たった物のTransform</param>
	void HitObject(Transform& _hitTrans);

};

