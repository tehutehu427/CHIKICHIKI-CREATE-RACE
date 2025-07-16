#pragma once
#include "../ItemBase.h"

#include<memory>
#include<functional>
#include<map>

class EffectController;

class CannonShot : public ItemBase
{
public:

	//ステータス
	static constexpr float ALIVE_TIME = 3.0f;	//生存時間	
	static constexpr float BLAST_TIME = 0.5f;	//爆発残存時間	
	static constexpr float SPEED = 15.0f;		//弾の速度
	static constexpr float SCALE = 0.3f;		//弾の大きさ
	static constexpr float BLAST_SCALE = 30.0f;						//爆発の大きさ
	static constexpr float BLAST_COL_SCALE = BLAST_SCALE * 10.0f;	//爆発の当たり判定の大きさ

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

	//状態
	enum class STATE
	{
		ALIVE,	//生存
		BLAST,	//爆発
		DEAD,	//死亡
	};

	STATE state_;		//状態
	float cnt_;			//生存カウンタ
	bool isAlive_;		//生存判定

	//関数ポインタ
	std::map<STATE,std::function<void(void)>> update_;	//更新
	std::map<STATE, std::function<void(void)>>draw_;	//描画

	//状態ごとの更新
	void UpdateAlive(void);
	void UpdateBlast(void);
	void UpdateDead(void);

	//状態ごとの描画
	void DrawAlive(void);
	void DrawBlast(void);
	void DrawDead(void);

	//移動処理
	inline void Move(void);

	//爆発
	void Blast(void);

	//削除
	inline void Kill(void);

	void InitShader() override;
};

