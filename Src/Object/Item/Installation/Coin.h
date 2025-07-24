#pragma once
#include "../ItemBase.h"

class Coin : public ItemBase
{
public:

	//マップ
	static constexpr IntVector3 MAP_SIZE = { 1,1,1 };					//マップサイズ
	static constexpr VECTOR MAP_LOCALPOS = { 50.0f,50.0f,5.0f };		//マップとの相対座標
	static constexpr float RADIUS = 25.0f;								//当たり判定用基本半径

	//サイズ
	static constexpr VECTOR MODEL_SIZE = { 100.0f,100.0f,10.0f };		//モデルのサイズ
	static constexpr float SIZE_MULTI = 0.3f;							//モデルのサイズ倍率

	//回転
	static constexpr float ROTATE_SPEED = 2.0f;							//回転速度

	//追従
	static constexpr VECTOR FOLLOW_LOCAL_POS = { 0.0f,60.0f,-60.0f };	//追従座標

	//コンストラクタ
	Coin(void);
	
	//デストラクタ
	~Coin(void);

	//個人設定
	void SetParam(void)override;
	//更新
	void Update(void)override;

	/// <summary>
	/// 当たり判定後の処理
	/// </summary>
	/// <param name="_hitColTag">相手側の当たり判定</param>
	void OnHit(const std::weak_ptr<Collider> _hitCol)override;

private:

	std::weak_ptr<Collider> followCol_;		//ついていく対象
};

