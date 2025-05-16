#pragma once
#include "ItemBase.h"

class CannonShot : public ItemBase
{
public:

	//生存時間
	static constexpr float ALIVE_TIME = 5.0f;
	
	//弾の速度
	static constexpr float SPEED = 5.0f;

	//弾の大きさ
	static constexpr float SCALE = 0.3f;

	//コンストラクタ
	CannonShot(VECTOR _pos, Quaternion _quaRot);
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

	//弾が何かに当たった
	void Hit(void);

private:

	//生存カウンタ
	float cnt_;

	//生存判定
	bool isAlive_;

	//移動量
	VECTOR movePow_;

	//移動処理
	void Move(void);
};

