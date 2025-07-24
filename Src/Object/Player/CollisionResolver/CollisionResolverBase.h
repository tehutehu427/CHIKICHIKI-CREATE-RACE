#pragma once
#include<DxLib.h>
#include<memory>
#include"../Object/ObjectBase.h"
#include"../Player.h"
class PlayerAction;
class CollisionResolverBase
{
public:
	struct POSITION_INFO
	{
		VECTOR& moveDiff;	//移動前
		VECTOR& currentPos;	//現在
		VECTOR& movedPos;		//移動後
	};
	CollisionResolverBase(VECTOR& moveDiff, VECTOR& currentPos, VECTOR& movedPos
		, PlayerAction& _action
		, std::vector<ObjectBase::ColParam>& _colParam);

	virtual void Init(void) = 0;

	virtual ~CollisionResolverBase(void) = 0;
	virtual void Resolve(void);
	virtual void Resolve(const std::weak_ptr<Collider> _hitCol);


	//ゲッタ
	//地面と当たっているか
	inline const bool GetIsLandHit(void)const { return isLandHit_; }

	//側面と当たっているか
	inline const bool GetIsSide(void)const { return isSide_; }
protected:
	//上下に引いたラインの押し出し
	virtual void HitUpdownLine(void);
	
	//移動量ラインの押し出し
	virtual void HitMoveLine(void);

	//側面と当たった時(モデル以外)
	virtual void HitBodyShere(void);
	//側面と当たった時
	virtual void HitBodyShere(const std::weak_ptr<Collider> _hitCol);



	VECTOR& moveDiff_;	//移動前
	VECTOR& currentPos_;	//現在
	VECTOR& movedPos_;		//移動後
	//当たり判定関係
	std::vector<ObjectBase::ColParam>& colParam_;

	//当たった座標
	VECTOR hitPos_;

	//地面と当たっているか
	bool isLandHit_;

	//プレイヤーのサイドが当たっている
	bool isSide_;

	//プレイヤー
	PlayerAction& action_;
private:
};

