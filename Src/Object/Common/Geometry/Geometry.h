#pragma once

#include<DxLib.h>
#include"../Common/Quaternion.h"

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

	//デストラクタ
	virtual~Geometry(void) = 0;

	//描画
	virtual void Draw(void) = 0;

	//各種当たり判定
	virtual const bool IsHit(const Geometry& _geometry)const = 0;
	virtual const bool IsHit(const Model& _model)const = 0;
	virtual const bool IsHit(const Cube& _cube)const = 0;
	virtual const bool IsHit(const Sphere& _sphere)const = 0;
	virtual const bool IsHit(const Capsule& _capsule)const = 0;
	virtual const bool IsHit(Line& _line);

	//親情報を返す
	inline const VECTOR& GetColPos(void)const { return pos_; }
	inline const Quaternion& GetColRot(void)const { return quaRot_; }

protected:

	/// <summary>
	/// コンストラクタ(外部で作る必要のない基底なのでprotected)
	/// </summary>
	/// <param name="_pos">追従する親の座標</param>
	/// <param name="_rot">追従する親の回転</param>
	Geometry(const VECTOR& _pos, const Quaternion& _rot);

	// 相対座標を回転させてワールド座標で取得する
	const VECTOR GetRotPos(const VECTOR& _localPos) const;

	const VECTOR& pos_;			//親の座標
	const Quaternion& quaRot_;	//親の回転
};