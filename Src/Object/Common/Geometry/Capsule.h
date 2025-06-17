#pragma once

#include"Geometry.h"

class Transform;
class Model;
class Cube;
class Sphere;
class Capsule;

class Capsule : public Geometry
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_parent">追従する親</param>
	/// <param name="_localPosTop">上側の相対座標</param>
	/// <param name="_localPosDown">下側の相対座標</param>
	/// <param name="_radius">半径</param>
	Capsule(const Transform& _parent, const VECTOR _localPosTop, const VECTOR _localPosDown, const float _radius);

	/// <summary>
	/// コピーコンストラクタ
	/// </summary>
	/// <param name="_copyBase">コピー元</param>
	/// <param name="_parent">追従する親</param>
	Capsule(const Capsule& _copyBase, const Transform& _parent);

	// デストラクタ
	~Capsule(void)override;

	// 描画
	void Draw(void)override;

	//各種当たり判定
	const bool IsHit(const Geometry& _geometry)const override;
	const bool IsHit(const Model& _model)const override;
	const bool IsHit(const Cube& _cube)const override;
	const bool IsHit(const Sphere& _sphere)const override;
	const bool IsHit(const Capsule& _capsule)const override;
	const bool IsHit(const Line& _line)const override;

	// 親Transformからの相対位置を取得
	inline const VECTOR GetLocalPosTop(void) const { return localPosTop_; }
	inline const VECTOR GetLocalPosDown(void) const { return localPosDown_; }

	// 親Transformからの相対位置をセット
	inline void SetLocalPosTop(const VECTOR& _pos) { localPosTop_ = _pos; }
	inline void SetLocalPosDown(const VECTOR& _pos) { localPosDown_ = _pos; }

	// ワールド座標を取得
	inline const VECTOR GetPosTop(void) const { return GetRotPos(localPosTop_); }
	inline const VECTOR GetPosDown(void) const { return GetRotPos(localPosDown_); }

	// 半径
	inline const float GetRadius(void) const { return radius_; }
	inline void SetRadius(float _radius) { radius_ = _radius; }

	// 高さ
	inline const float GetHeight(void) const { return localPosTop_.y; }

	// カプセルの中心座標
	inline const VECTOR GetCenter(void) const;

private:

	// 親Transformからの相対位置(上側)
	VECTOR localPosTop_;

	// 親Transformからの相対位置(下側)
	VECTOR localPosDown_;

	// 半径
	float radius_;

};
