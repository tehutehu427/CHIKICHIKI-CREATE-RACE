#pragma once

#include"Geometry/Geometry.h"

class ObjectBase;

class Collider
{

public :

	// 衝突用タグ
	enum class TAG
	{
		PLAYER1,		//プレイヤー1
		PLAYER2,		//プレイヤー2
		PLAYER3,		//プレイヤー3
		PLAYER4,		//プレイヤー4

		START,			//開始地点
		GOAL,			//終了地点

		NORMAL_ITEM,	//通常アイテム		
		MOVE_FLOOR,		//移動床
		CANNON_AIM,		//大砲の狙い範囲
		SLIME_FLOOR,	//スライム床
		SPRING,			//ばね
		KILLER_ITEM,	//接触すると死ぬアイテム

		DESTROYER,		//アイテムオブジェクトを破壊する
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="type">衝突用タグ</param>
	/// <param name="geometry">当たり判定の形状</param>
	Collider(ObjectBase& _parent, const TAG _tag, Geometry& _geometry);

	// デストラクタ
	~Collider(void);

	//衝突用タグの取得
	inline const TAG GetTag(void)const { return tag_; }

	//当たり判定の形状を取得
	inline const Geometry& GetGeometry(void)const { return geometry_; }

	//親を取得
	inline const ObjectBase& GetParent(void)const { return parent_; }

	//終了判定の取得
	inline const bool IsDead(void)const { return isDead_; }

	//終了処理(所持者の解放時に置く)
	inline void Kill(void) { isDead_ = true; }

	/// <summary>
	/// 当たった時の処理
	/// </summary>
	/// <param name="_collider">相手のコライダ</param>
	void OnHit(const std::weak_ptr<Collider> _collider);

private:

	//親
	ObjectBase& parent_;

	// 衝突用タグ
	TAG tag_;

	//当たり判定の形状
	Geometry& geometry_;

	//終了判定
	bool isDead_;
};
