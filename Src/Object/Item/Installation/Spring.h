#pragma once
#include "../ItemBase.h"

class Spring : public ItemBase
{
public:

	//マップ
	static constexpr IntVector3 MAP_SIZE = { 1,1,1 };				//マップサイズ
	static constexpr VECTOR MAP_LOCALPOS = { 16.0f,9.0f,16.0f };	//マップとの相対座標

	//サイズ
	static constexpr VECTOR MODEL_SIZE = { 32.0f,18.0f,32.0f };		//モデルのサイズ

	//移動力
	static constexpr float MOVE_POW = 5.0f;		//移動力(プレイヤーを跳ねさせる専用)

	//コンストラクタ
	Spring();
	//デストラクタ
	~Spring()override;

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

	//移動力を初期化
	void InitMovePow(void);
};

