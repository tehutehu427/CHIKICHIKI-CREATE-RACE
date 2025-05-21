#pragma once
#include "ItemBase.h"

#include<memory>

class Cannon;

class CannonShot : public ItemBase
{
public:

	//ステータス
	static constexpr float ALIVE_TIME = 5.0f;	//生存時間	
	static constexpr float SPEED = 5.0f;		//弾の速度
	static constexpr float SCALE = 0.3f;		//弾の大きさ

	//コンストラクタ
	CannonShot(VECTOR _pos, Quaternion _quaRot, Cannon* _parent);
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

	float cnt_;			//生存カウンタ
	bool isAlive_;		//生存判定
	VECTOR movePow_;	//移動量
	Cannon* parent_;	//親大砲

	//移動処理
	void Move(void);
};

