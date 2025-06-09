#pragma once
#include<DxLib.h>
#include<memory>
#include<vector>
#include<map>
#include"../Common/IntVector3.h"
#include"../../Object/Common/Collider.h"
class ObjectBase;
class Collider;
class Player;
class ItemBase;
class CollisionManager
{
public:
	static void CreateInstance(void);
	static CollisionManager& GetInstance(void);

	enum class COL_TAG
	{
		NONE
		,PLAYER
		,MOVE_VER_FLOOR
		,MOVE_HORI_FLOOR
		,
	};
	enum class COLLISION_SHAPE
	{
		SPHERE
		,CUBE
		,LINE
		,MODEL
	};

	struct Coll_Info
	{
		Collider::COL_TAG tag;	//何と当たっているか
		bool isHit;				//当たったか
		VECTOR hitPos;			//当たっている座標
		VECTOR colTargetPos;	//当たっているオブジェクトの座標
		VECTOR normal;			//法線ベクトル
	};

	//当たり判定の更新
	void Update(Player& _player);

	//コライダの追加
	void AddCollider(std::weak_ptr<Collider>_collider);
	//コライダの削除
	void ClearCollider(void);
	//インスタンス解放
	void Destroy(void);
	//線の当たり判定
	Coll_Info LineCol(VECTOR pos1, VECTOR pos2);
	//球の当たり判定
	Collider::COL_TAG SphereCol(float RADIUS,VECTOR pos);

	//プレイヤーの当たり判定を行う範囲内にアイテムがあるかを調べる
	void CheckItemsInPlayerColRange(Player& _player, IntVector3 _colPos);
	//アイテムの支点
	std::vector<IntVector3>itemLPos_;
private:
	//コピーコンストラクタの防止
	CollisionManager(void);
	CollisionManager(const CollisionManager& collisionMng) = delete;
	void operator=(const CollisionManager& collisionMng) = delete;
	~CollisionManager(void);


	//メンバ変数
	static CollisionManager* collisionMng_;

	std::shared_ptr<ObjectBase>object_;

	//当たり判定配列
	std::vector<std::weak_ptr<Collider>>colliders_;
	//std::map<COL_TAG, std::vector<std::unique_ptr<Collider>>>colliders_;


};

