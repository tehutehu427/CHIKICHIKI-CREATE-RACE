#pragma once

#include<vector>
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

		PUNCH,			//パンチ

		SHADOW,			//影

		START,			//開始地点
		GOAL,			//終了地点

		NORMAL_ITEM,	//通常アイテム		
		MOVE_VER_FLOOR,		//縦移動床
		MOVE_HORI_FLOOR,	//横移動床
		CANNON_AIM,		//大砲の狙い範囲
		SLIME_FLOOR,	//スライム床
		SPRING,			//ばね
		WIND,
		KILLER_ITEM,	//接触すると死ぬアイテム

		DESTROYER,		//アイテムオブジェクトを破壊する
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="type">衝突用タグ</param>
	/// <param name="geometry">当たり判定の形状</param>
	Collider(ObjectBase& _parent, const std::vector<TAG> _tags, Geometry& _geometry);

	// デストラクタ
	~Collider(void);

	//衝突用タグの取得
	inline const std::vector<TAG> GetTags(void)const { return tags_; }

	//当たり判定の形状を取得
	inline Geometry& GetGeometry(void)const { return geometry_; }

	//親を取得
	inline const ObjectBase& GetParent(void)const { return parent_; }

	//当たったかの判定の取得
	inline const bool IsHit(void)const { return isHit_; }

	//当たっていない
	inline void NotHit(void) { isHit_ = false; }

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
	std::vector<TAG> tags_;

	//当たり判定の形状
	Geometry& geometry_;

	//当たったかの判定
	bool isHit_;

	//終了判定
	bool isDead_;
};
