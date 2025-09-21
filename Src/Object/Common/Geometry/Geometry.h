#pragma once
#include<DxLib.h>
#include"../../../Common/Quaternion.h"

class Model;
class Cube;
class Sphere;
class Capsule;
class Line;

class Geometry
{

public:

	//通常色
	static constexpr int NORMAL_COLOR = 0x000000;

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	virtual~Geometry(void) = 0;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name=""></param>
	virtual void Draw(void) = 0;

	/// <summary>
	/// 形状当たり判定
	/// </summary>
	/// <param name="_geometry">形状情報</param>
	/// <returns>当たり判定(true:当たった)</returns>
	virtual const bool IsHit(Geometry& _geometry) = 0;

	/// <summary>
	/// モデルとの当たり判定
	/// </summary>
	/// <param name="_geometry">モデル情報</param>
	/// <returns>当たり判定(true:当たった)</returns>
	virtual const bool IsHit(Model& _model) = 0;

	/// <summary>
	/// 箱との当たり判定
	/// </summary>
	/// <param name="_geometry">箱情報</param>
	/// <returns>当たり判定(true:当たった)</returns>
	virtual const bool IsHit(Cube& _cube) = 0;

	/// <summary>
	/// 球との当たり判定
	/// </summary>
	/// <param name="_geometry">球情報</param>
	/// <returns>当たり判定(true:当たった)</returns>
	virtual const bool IsHit(Sphere& _sphere) = 0;

	/// <summary>
	/// カプセルとの当たり判定
	/// </summary>
	/// <param name="_geometry">カプセル情報</param>
	/// <returns>当たり判定(true:当たった)</returns>
	virtual const bool IsHit(Capsule& _capsule) = 0;

	/// <summary>
	/// 線分との当たり判定
	/// </summary>
	/// <param name="_geometry">線分情報</param>
	/// <returns>当たり判定(true:当たった)</returns>
	virtual const bool IsHit(Line& _line) = 0;

	/// <summary>
	/// ヒット後の処理
	/// </summary>
	/// <param name=""></param>
	virtual void HitAfter(void);

	/// <summary>
	/// 親の座標を返す
	/// </summary>
	/// <param name=""></param>
	/// <returns>親の座標</returns>
	inline const VECTOR& GetColPos(void)const { return pos_; }

	/// <summary>
	/// 親の回転を返す
	/// </summary>
	/// <param name=""></param>
	/// <returns>親の回転</returns>
	inline const Quaternion& GetColRot(void)const { return quaRot_; }

protected:

	/// <summary>
	/// コンストラクタ(外部で作る必要のない基底なのでprotected)
	/// </summary>
	/// <param name="_pos">追従する親の座標</param>
	/// <param name="_rot">追従する親の回転</param>
	Geometry(const VECTOR& _pos, const Quaternion& _rot);

	/// <summary>
	/// 相対座標を回転させてワールド座標で取得する
	/// </summary>
	/// <param name="_localPos">相対座標</param>
	/// <returns>回転後座標</returns>
	const VECTOR GetRotPos(const VECTOR& _localPos) const;
	
	//親情報
	const VECTOR& pos_;			//親の座標
	const Quaternion& quaRot_;	//親の回転
};