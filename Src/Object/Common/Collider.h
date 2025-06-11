#pragma once
#include<memory>
class ObjectBase;
class Collider
{

public :

	// 衝突種別
	enum class COL_TAG
	{
		NONE,
		STAGE,
		PLAYER,
		DEATH,
		FLOOR,
		SLIME,
	};

	enum class COLLISION_TYPE
	{
		NONE,
		MODEL,
		SPHERE,
		LINE,
		CUBE,
		CAPSULE
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_owner">持ち主</param>
	/// <param name="_tag">タグ</param>
	/// <param name="_type">どんな当たり判定のタイプ</param>
	/// <param name="_modelId">モデルID</param>
	Collider(std::weak_ptr<ObjectBase> _owner,COL_TAG _tag,COLLISION_TYPE _type ,int _modelId);

	// デストラクタ
	~Collider(void);

	//void OnHit(const Collider& target)override
	//ゲッタ
	//持ち主
	std::weak_ptr<ObjectBase> GetOwner(void)const{ return owner_; }
	//タグ
	const COL_TAG GetTag(void)const { return tag_; }
	//当たり判定タイプ
	const COLLISION_TYPE GetColType(void) { return type_; }
	//モデルID
	const int GetModelId(void)const { return modelId_; }

protected:
private:
	//当たり判定の持ち主
	std::weak_ptr<ObjectBase> owner_;

	// 衝突種別
	COL_TAG tag_;

	//当たり判定のタイプ
	COLLISION_TYPE type_;

	// モデルのハンドルID
	int modelId_;
};
