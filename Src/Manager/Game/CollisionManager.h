#pragma once
#include<memory>

class ObjectBase;
class CollisionManager
{
public:
	static void CreateInstance(void);
	CollisionManager* GetInstance(void);

	enum class COL_TAG
	{
		NONE
		,PLAYER
	};

	void Update(void);
	void Draw(void);


private:
	//コピーコンストラクタの防止
	CollisionManager(void);
	CollisionManager(const CollisionManager& collisionMng) = delete;
	void operator=(const CollisionManager& collisionMng) = delete;
	~CollisionManager(void);


	//メンバ変数
	static CollisionManager* collisionMng_;


};

