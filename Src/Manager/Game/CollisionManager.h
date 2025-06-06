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
	CollisionManager* GetInstance(void);

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

	void AddCollider(void);
	void LineCol(ObjectBase& _object,VECTOR pos1, VECTOR pos2);


private:
	//コピーコンストラクタの防止
	CollisionManager(void);
	CollisionManager(const CollisionManager& collisionMng) = delete;
	void operator=(const CollisionManager& collisionMng) = delete;
	~CollisionManager(void);


	//メンバ変数
	static CollisionManager* collisionMng_;
	//std::map<COL_TAG, std::vector<std::unique_ptr<Collider>>>colliders_;


};

