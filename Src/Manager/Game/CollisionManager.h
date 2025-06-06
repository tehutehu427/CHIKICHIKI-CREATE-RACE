#pragma once
#include<DxLib.h>
#include<memory>
#include<vector>
#include<map>

class ObjectBase;
class Collider;
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
		VECTOR hitPos;			//当たっている座標
		VECTOR normal;			//法線ベクトル
	};

	void AddCollider(std::weak_ptr<Collider>_collider);
	void ClearCollider(void);
	Collider::COL_TAG LineCol(VECTOR pos1, VECTOR pos2);


private:
	//コピーコンストラクタの防止
	CollisionManager(void);
	CollisionManager(const CollisionManager& collisionMng) = delete;
	void operator=(const CollisionManager& collisionMng) = delete;
	~CollisionManager(void);


	//メンバ変数
	static CollisionManager* collisionMng_;

	//当たり判定配列
	std::vector<std::weak_ptr<Collider>>colliders_;
	//std::map<COL_TAG, std::vector<std::unique_ptr<Collider>>>colliders_;


};

