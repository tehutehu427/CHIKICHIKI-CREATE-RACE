#pragma once
#include "CollisionResolverBase.h"
class CollisionModelResolver :
    public CollisionResolverBase
{
public:
	CollisionModelResolver(VECTOR& _moveDiff, VECTOR& _currentPos, VECTOR& _movedPos
		, PlayerAction& _action
		, std::vector<ObjectBase::ColParam>& _colParam);
    ~CollisionModelResolver(void)override;
	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name=""></param>
	void Init(void)override;

	/// <summary>
	/// 押し出し処理(外部ではこれを呼び出す)
	/// </summary>
	/// <param name="_hitCol"></param>
	void Resolve(const std::weak_ptr<Collider> _hitCol)override;
    void HitUpdownLine(void)override;
    void HitMoveLine(void)override;
    void HitBodyShere(const std::weak_ptr<Collider> _hitCol)override;
private:
    //定数
	//--------------------------------------------
	//接地しているときのラインのコライダ
	static constexpr int UP_AND_DOWN_LINE_COL_NO = 0;

	//プレイヤーの体の球
	static constexpr int BODY_SPHERE_COL_NO = 1;

	//現在の座標と移動後座標を結んだ線のコライダ
	static constexpr int MOVE_LINE_COL_NO = 2;

	//現在の座標と移動後座標を結んだ線のコライダ
	static constexpr int EYE_LINE_NO = 3;

	//当たり判定のめりこみ防止用
	static constexpr float POSITION_OFFSET = 1.0f;

	//オブジェクトの下に当たった時の跳ね返り減速セット
	static constexpr float DOWN_BOUNCE_DECELERATION = -10.0f;

	static constexpr float MOVE_LINE_Y_OFFSET = Player::RADIUS - 1.0f;
	//static constexpr float MOVE_LINE_Y_CHECK_VALUE = Player::RADIUS - 1.5f;
	static constexpr float MOVE_LINE_Y_CHECK_VALUE = Player::RADIUS + 1.5f;

	//当たり判定の押し出し回数
	static constexpr int COL_TRY_CNT_MAX = 10;

	//ヒットした法線方向へのオフセット
	static constexpr float HIT_NORMAL_OFFSET = 3.0f;
	//--------------------------------------------

};

