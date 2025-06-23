#pragma once

#include<vector>
#include<memory>
#include<map>
#include<functional>
#include"../../Object/Common/Collider.h"

class Geometry;

class CollisionManager
{
public:

	//当たり判定をする範囲
	static constexpr float HIT_RANGE_NORMAL = 1000.0f;	//通常の当たり判定距離
	static constexpr float HIT_RANGE_START = 600.0f;	//通常の当たり判定距離
	static constexpr float HIT_RANGE_GOAL = 600.0f;	//通常の当たり判定距離
	static constexpr float HIT_RANGE_TARGET = 2500.0f;	//ターゲット用の当たり判定距離
	
	//インスタンス生成
	static void CreateInstance(void);

	//インスタンスの取得
	static CollisionManager& GetInstance(void) { return *instance_; }

	//コライダの追加
	void AddCollider(const std::shared_ptr<Collider> _collider) { colliders_.push_back(_collider); }

	//必要なくなったコライダの削除(更新の最後に置く)
	void Sweep(void);

	//更新
	void Update(void);

	//削除
	void Destroy(void);

private:

	//静的インスタンス
	static CollisionManager* instance_;

	//当たり判定格納
	std::vector<std::shared_ptr<Collider>>colliders_;

	//当たり判定距離の二乗
	std::map<Collider::TAG, float> hitRange_;

	//コンストラクタ
	CollisionManager(void);

	//コピーの禁止
	CollisionManager(const CollisionManager& _copy) = delete;
	void operator= (const CollisionManager& _copy) = delete;

	//デストラクタ
	~CollisionManager(void);

	/// <summary>
	/// 当たり判定距離内にいるか
	/// </summary>
	/// <param name="_col1">1つ目のコライダ</param>
	/// <param name="_col2">2つ目のコライダ</param>
	/// <returns>true:範囲内</returns>
	const bool IsWithInHitRange(const std::weak_ptr<Collider> _col1, const std::weak_ptr<Collider> _col2)const;

	/// <summary>
	/// 当たり判定
	/// </summary>
	/// <param name="_col1">1つ目のコライダ</param>
	/// <param name="_col2">2つ目のコライダ</param>
	/// <returns>true:当たった</returns>
	bool IsCollision(const std::weak_ptr<Collider> _col1, const std::weak_ptr<Collider> _col2);

};

