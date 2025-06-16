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
	static constexpr float HIT_DIS = 500.0f;

	//当たり判定範囲の2乗
	static constexpr float HIT_DIS_SQUARE = HIT_DIS * HIT_DIS;
	
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

	//コンストラクタ
	CollisionManager(void);

	//コピーの禁止
	CollisionManager(const CollisionManager& _copy) = delete;
	void operator= (const CollisionManager& _copy) = delete;

	//デストラクタ
	~CollisionManager(void);

	//当たり判定
	bool IsCollision(const std::weak_ptr<Collider> _col1, const std::weak_ptr<Collider> _col2);
};

