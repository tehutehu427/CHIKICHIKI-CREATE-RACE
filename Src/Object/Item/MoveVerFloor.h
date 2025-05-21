#pragma once
#include"ItemBase.h"

class MoveVerFloor : public ItemBase
{
public:

	//マップ
	static constexpr IntVector3 MAP_SIZE = { 4,1,4 };	//マップサイズ
	static constexpr int MOVE_Y = 4;					//横移動距離
	static constexpr float ONE_POINT_SEC = 5.0f;		//1地点移動するのにかかる秒数
	static constexpr int ROUTE = 2;						//ルート数

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

	/// <summary>
	/// 当たった処理
	/// </summary>
	/// <param name="_hitTrans">対象のモデル情報</param>
	void Hit(Transform& _hitTrans)override;

private:

	//ルート関係
	VECTOR route_[ROUTE];	//ルート
	VECTOR startRoute_;		//開始ルート
	VECTOR goalRoute_;		//終了ルート
	int routeNum_;			//現在のルートナンバー

	//移動関係
	float speed_;		//移動速度
	double distance_;	//ルートまでの距離
	VECTOR moveVec_;	//移動方向
	VECTOR movePow_;	//移動量

	//移動
	void Move(void);

	//ルート初期化
	void InitRoute(void);

	//ルート設定
	void SetRoute(void);

	//ルートを超えたかの判定
	bool IsBeyondRoute(void);

};

