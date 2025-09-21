#pragma once
#include "../ItemBase.h"

class CoinFollower;

class Coin : public ItemBase
{

public:

	//マップ
	static constexpr IntVector3 MAP_SIZE = { 1,1,1 };				//マップサイズ
	static constexpr VECTOR MAP_LOCALPOS = { 50.0f,50.0f,5.0f };	//マップとの相対座標
	static constexpr float RADIUS = 25.0f;							//当たり判定用基本半径

	//サイズ
	static constexpr VECTOR MODEL_SIZE = { 100.0f,100.0f,10.0f };	//モデルのサイズ
	static constexpr float SIZE_MULTI = 0.3f;						//モデルのサイズ倍率
	static constexpr float EFFECT_SCALE = 10.0f;					//エフェクトの大きさ

	//回転
	static constexpr float ROTATE_SPEED = 2.0f;						//回転速度

	//追従
	static constexpr VECTOR FOLLOW_LOCAL_POS = { 0.0f,60.0f,-60.0f };	//追従座標
	static constexpr float COIN_DIS = 80.0f;							//コイン同士の距離

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name=""></param>
	Coin(void);
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~Coin(void);

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
	/// 当たり判定後の処理
	/// </summary>
	/// <param name="_hitColTag">相手側の当たり判定</param>
	void OnHit(const std::weak_ptr<Collider> _hitCol)override;

	/// <summary>
	/// 終了
	/// </summary>
	/// <param name=""></param>
	void End(void);

private:

	//追従関係
	std::weak_ptr<Collider> followCol_;			//ついていく対象
	std::unique_ptr<CoinFollower> follower_;	//ついていくプレイヤー
	bool isEnd_;								//終了状態
	VECTOR followPos_;							//追従座標

	/// <summary>
	/// マップ上からの削除処理
	/// </summary>
	void Delete(void);
};

