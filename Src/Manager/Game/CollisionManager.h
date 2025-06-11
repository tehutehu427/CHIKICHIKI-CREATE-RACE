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
	//void AddCollider(Collider& _colllider);
	//コライダの削除
	void ClearCollider(void);
	//インスタンス解放
	void Destroy(void);
	/// <summary>
	/// モデルと線の当たり判定
	/// </summary>
	/// <param name="_col">相手のモデルコライダ</param>
	/// <param name="_startPos">線の始まりの座標</param>
	/// <param name="_endPos">線の終わりの座標</param>
	/// <returns>当たった座標などの基本情報</returns>
	Coll_Info LineCol(Collider& _col,VECTOR _startPos, VECTOR _endPos);
	//球の当たり判定
	Collider::COL_TAG SphereCol(float RADIUS,VECTOR pos);

	//当たり判定のチェック
	void CheckCollider(void);

	//プレイヤーの当たり判定を行う範囲内にアイテムがあるかを調べる
	void CheckItemsInPlayerColRange(Player& _player, IntVector3 _colPos);

	/// <summary>
	/// コライダの生成
	/// </summary>
	/// <param name="_owner">持ち主自身</param>
	/// <param name="_tag">タグ</param>
	/// <param name="_type">当たり判定の形状</param>
	/// <param name="_modelId">自分自身のモデルID</param>
	void MakeColllider(std::weak_ptr<ObjectBase>_owner, Collider::COL_TAG _tag,Collider::COLLISION_TYPE _type,int _modelId);

	//線の当たり判定情報取得
	inline const Coll_Info GetLineCol(void)const { return lineCol_; }


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

	//当たり判定配列
	std::vector<std::shared_ptr<Collider>>colliders_;
	//std::map<COL_TAG, std::vector<std::unique_ptr<Collider>>>colliders_;

	//線の当たり判定情報
	Coll_Info lineCol_;

};

