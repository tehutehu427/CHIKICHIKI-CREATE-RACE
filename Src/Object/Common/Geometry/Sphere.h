#pragma once

#include"Geometry.h"

class Transform;
class Model;
class Cube;
class Sphere;
class Capsule;

class Sphere : public Geometry
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_parent">追従する親</param>
	/// <param name="_radius">半径</param>
	Sphere(const Transform& _parent, const float _radius);

	/// <summary>
	/// コピーコンストラクタ
	/// </summary>
	/// <param name="_copyBase">コピー元</param>
	/// <param name="_parent">追従する親</param>
	Sphere(const Sphere& _copyBase, const Transform& _parent);

	//デストラクタ
	~Sphere(void)override;

	//描画
	void Draw(void)override;

	//各種当たり判定
	const bool IsHit(const Geometry& _geometry)const override;
	const bool IsHit(const Model& _model)const override;
	const bool IsHit(const Cube& _cube)const override;
	const bool IsHit(const Sphere& _sphere)const override;
	const bool IsHit(const Capsule& _capsule)const override;
	const bool IsHit(const Line& _line)const override;

	//半径の取得
	inline const float GetRadius(void)const { return radius_; }

	//半径の設定
	inline void SetRadius(const float _radius) { radius_ = _radius; }

private:

	float radius_;	//半径
};