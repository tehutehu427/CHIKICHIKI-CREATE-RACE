#pragma once
#include "../ItemBase.h"

#include<memory>

class CannonShot : public ItemBase
{
public:

	//ステータス
	static constexpr float ALIVE_TIME = 5.0f;	//生存時間	
	static constexpr float SPEED = 15.0f;		//弾の速度
	static constexpr float SCALE = 0.3f;		//弾の大きさ

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_pos">親の座標</param>
	/// <param name="_quaRot">親の回転</param>
	/// <param name="_scl">親の大きさ</param>
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
	inline const bool IsAlive(void)const { return isAlive_; }

	/// <summary>
	/// 当たり判定後の処理
	/// </summary>
	/// <param name="_hitColTag">相手側の当たり判定</param>
	void OnHit(const std::weak_ptr<Collider> _hitCol)override;

private:

	float cnt_;			//生存カウンタ
	bool isAlive_;		//生存判定

	//移動処理
	inline void Move(void);

	//削除
	inline void Kill(void);
};

