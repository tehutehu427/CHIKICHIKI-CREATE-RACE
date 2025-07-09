#pragma once
#include "../ItemBase.h"

class Wind;

class Fan : public ItemBase
{
public:

	//マップ
	static constexpr IntVector3 MAP_SIZE = { 4,3,4 };					//マップサイズ
	static constexpr VECTOR MAP_LOCALPOS = { 150.0f,160.0f,110.0f };	//マップとの相対座標

	//サイズ
	static constexpr VECTOR MODEL_SIZE = { 300.0f,320.0f,220.0f };		//モデルのサイズ

	//コンストラクタ
	Fan();
	//デストラクタ
	~Fan()override;

	//個人設定
	void SetParam(void)override;
	//更新
	void Update(void)override;
	//描画
	void Draw(void)override;

	/// <summary>
	/// 当たり判定後の処理
	/// </summary>
	/// <param name="_hitColTag">相手側の当たり判定</param>
	void OnHit(const std::weak_ptr<Collider> _hitCol)override;

	//アイテムの値リセット
	void ResetValue(void)override;

private:

	std::unique_ptr<Wind> wind_;	//風オブジェクト

	//風の生成
	void CreateWind(void);
};

