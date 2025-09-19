#pragma once
#include "../ItemBase.h"

class MoveHoriFloor : public ItemBase
{

public:

	//マップ
	static constexpr IntVector3 MAP_SIZE = { 4,1,4 };				//マップサイズ
	static constexpr int MOVE_X = 8;								//横移動距離
	static constexpr float ONE_POINT_SEC = 5.0f;					//1地点移動するのにかかる秒数
	static constexpr int ROUTE = 2;									//ルート数
	static constexpr VECTOR MAP_LOCALPOS = { 220.0f,15.0f,220.0f };	//マップとの相対座標

	//モデルのサイズ
	static constexpr VECTOR MODEL_SIZE = { 440.0f,30.0f,440.0f };	

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name=""></param>
	MoveHoriFloor(void);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~MoveHoriFloor(void)override;

	/// <summary>
	/// 個人設定
	/// </summary>
	/// <param name=""></param>
	void SetParam(void)override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name=""></param>
	void Update(void)override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name=""></param>
	void Draw(void)override;

	/// <summary>
	/// 当たり判定後の処理
	/// </summary>
	/// <param name="_hitColTag">相手側の当たり判定</param>
	void OnHit(const std::weak_ptr<Collider> _hitCol)override;

	/// <summary>
	/// サイズの取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>サイズ(マップ座標基準)</returns>
	inline const IntVector3 GetHitSize(void)const override;

	/// <summary>
	/// 値リセット
	/// </summary>
	/// <param name=""></param>
	void ResetValue(void)override;

private:

	//ルート関係
	VECTOR route_[ROUTE];	//ルート
	VECTOR startRoute_;		//開始ルート
	VECTOR goalRoute_;		//終了ルート
	int routeNum_;			//現在のルートナンバー

	//移動関係
	float speed_;		//移動速度
	float distance_;	//ルートまでの距離
	VECTOR moveVec_;	//移動方向

	//移動
	void Move(void);

	//ルート初期化
	void InitRoute(void);

	//ルート設定
	void SetRoute(void);

	//ルートを超えたかの判定
	bool IsBeyondRoute(void);
};

