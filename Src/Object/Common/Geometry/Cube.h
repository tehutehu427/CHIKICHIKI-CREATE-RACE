#pragma once

#include"Geometry.h"

class Transform;
class Model;
class Cube;
class Sphere;
class Capsule;

class Cube : public Geometry
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_parent">追従する親</param>
	Cube(const Transform& _parent);

	/// <summary>
	/// コピーコンストラクタ
	/// </summary>
	/// <param name="_copyBase">コピー元</param>
	/// <param name="_parent">追従する親</param>
	Cube(const Cube& _copyBase, const Transform& _parent);

	//デストラクタ
	~Cube(void)override;

	//描画
	inline void Draw(void)override;

	//各種当たり判定
	const bool IsHit(const Geometry& _geometry)const override;
	const bool IsHit(const Model& _model)const override;
	const bool IsHit(const Cube& _cube)const override;
	const bool IsHit(const Sphere& _sphere)const override;
	const bool IsHit(const Capsule& _capsule)const override;
	const bool IsHit(const Line& _line)const override;

	//サイズの半分の取得
	inline const VECTOR GetHalfSize(void)const { return halfSize_; }

	//サイズの半分の設定
	inline void SetHalfSize(const VECTOR& _halfSize) { halfSize_ = _halfSize; }

private:

	VECTOR halfSize_;	//箱の半分サイズ
	VECTOR axis_[3];	//回転軸(X,Y,Z)
};