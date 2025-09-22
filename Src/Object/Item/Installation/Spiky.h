#pragma once
#include "../ItemBase.h"

class Spiky : public ItemBase
{

public:

	//マップ
	static constexpr IntVector3 MAP_SIZE = { 2,1,2 };				//マップサイズ
	static constexpr VECTOR MAP_LOCALPOS = { 27.5f,5.5f,27.5f };	//マップとの相対座標

	//モデルのサイズ
	static constexpr VECTOR MODEL_SIZE = { 55.0f,11.0f,55.0f };		

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name=""></param>
	Spiky(void);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~Spiky(void)override;

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
};

