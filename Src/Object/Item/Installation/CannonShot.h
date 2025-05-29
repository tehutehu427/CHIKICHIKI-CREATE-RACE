#pragma once
#include "../ItemBase.h"

#include<memory>

class CannonShot : public ItemBase
{
public:

	//ステータス
	static constexpr float ALIVE_TIME = 5.0f;	//生存時間	
	static constexpr float SPEED = 5.0f;		//弾の速度
	static constexpr float SCALE = 0.3f;		//弾の大きさ

	//コンストラクタ
	CannonShot(const VECTOR _pos, const Quaternion _quaRot, const VECTOR _scl);
	//デストラクタ
	~CannonShot()override;

	//個人設定
	void SetParam(void)override;
	//更新
	void Update(void)override;
	//描画
	void Draw(void)override;

	//弾の生存判定
	const bool IsAlive(void)const { return isAlive_; }

	/// <summary>
	/// 当たった処理
	/// </summary>
	/// <param name="_hitTrans">対象のモデル情報</param>
	void Hit(Transform& _hitTrans)override;

private:

	float cnt_;			//生存カウンタ
	bool isAlive_;		//生存判定
	VECTOR movePow_;	//移動量

	//移動処理
	void Move(void);

	//削除
	void Kill(void);
};

