#pragma once

#include<DxLib.h>
class Transform;
class Model;
class Cube;
class Sphere;
class Capsule;
class Line;

class Geometry
{
public:

	//通常色
	static constexpr int NORMAL_COLOR = 0xffffff;

	/// <summary>
	/// コンストラクタ(外部で作る必要のない基底なのでprotected)
	/// </summary>
	/// <param name="_parent">追従する親</param>
	Geometry(const Transform& _parent);

	//デストラクタ
	virtual~Geometry(void) = 0;

	//描画
	inline virtual void Draw(void) = 0;

	//各種当たり判定
	virtual const bool IsHit(const Geometry& _geometry)const = 0;
	virtual const bool IsHit(const Model& _model)const = 0;
	virtual const bool IsHit(const Cube& _cube)const = 0;
	virtual const bool IsHit(const Sphere& _sphere)const = 0;
	virtual const bool IsHit(const Capsule& _capsule)const = 0;
	virtual const bool IsHit(const Line& _line)const;
	virtual const bool IsHit(Line& _line);

	//親情報を返す
	inline const Transform& GetTransParent(void)const { return transformParent_; }

protected:

	// 相対座標を回転させてワールド座標で取得する
	const VECTOR GetRotPos(const VECTOR& _localPos) const;

	const Transform& transformParent_;	//親の情報
};