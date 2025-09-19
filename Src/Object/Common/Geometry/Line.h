#pragma once
#include"Geometry.h"

class Line : public Geometry
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_pos">追従する親の座標</param>
	/// <param name="_rot">追従する親の回転</param>
	/// <param name="_localPosPoint1">1つ目の点の座標</param>
	/// <param name="_localPosPoint2">2つ目の点の座標</param>
	Line(const VECTOR& _pos, const Quaternion& _rot, const VECTOR _localPosPoint1, const VECTOR _localPosPoint2);

	/// <summary>
	/// コピーコンストラクタ
	/// </summary>
	/// <param name="_copyBase">コピー元</param>
	/// <param name="_pos">追従する親の座標</param>
	/// <param name="_rot">追従する親の回転</param>
	Line(const Line& _copyBase, const VECTOR& _pos, const Quaternion& _rot);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~Line(void)override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name=""></param>
	void Draw(void)override;

	/// <summary>
	/// 形状当たり判定
	/// </summary>
	/// <param name="_geometry">形状情報</param>
	/// <returns>当たり判定(true:当たった)</returns>
	const bool IsHit(Geometry& _geometry) override;

	/// <summary>
	/// モデルとの当たり判定
	/// </summary>
	/// <param name="_geometry">モデル情報</param>
	/// <returns>当たり判定(true:当たった)</returns>
	const bool IsHit(Model& _model) override;

	/// <summary>
	/// 箱との当たり判定
	/// </summary>
	/// <param name="_geometry">箱情報</param>
	/// <returns>当たり判定(true:当たった)</returns>
	const bool IsHit(Cube& _cube) override;

	/// <summary>
	/// 球との当たり判定
	/// </summary>
	/// <param name="_geometry">球情報</param>
	/// <returns>当たり判定(true:当たった)</returns>
	const bool IsHit(Sphere& _sphere) override;

	/// <summary>
	/// カプセルとの当たり判定
	/// </summary>
	/// <param name="_geometry">カプセル情報</param>
	/// <returns>当たり判定(true:当たった)</returns>
	const bool IsHit(Capsule& _capsule) override;

	/// <summary>
	/// 線分との当たり判定
	/// </summary>
	/// <param name="_geometry">線分情報</param>
	/// <returns>当たり判定(true:当たった)</returns>
	const bool IsHit(Line& _line) override;

	/// <summary>
	/// 1つ目の点の座標を取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>1つ目の点の座標</returns>
	inline const VECTOR GetLocalPosPoint1(void)const { return localPosPoint1_; }

	/// <summary>
	/// 2つ目の点の座標を取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>2つ目の点の座標</returns>
	inline const VECTOR GetLocalPosPoint2(void)const { return localPosPoint2_; }

	/// <summary>
	/// 回転済みの1つ目の点の座標を取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>回転済みの1つ目の点の座標</returns>
	inline const VECTOR GetPosPoint1(void) const { return GetRotPos(localPosPoint1_); }

	/// <summary>
	/// 回転済みの2つ目の点の座標を取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>回転済みの2つ目の点の座標</returns>
	inline const VECTOR GetPosPoint2(void) const { return GetRotPos(localPosPoint2_); }

	/// <summary>
	/// 1つ目の点の座標の設定
	/// </summary>
	/// <param name="_pos">1つ目の点の座標</param>
	inline void SetLocalPosPoint1(const VECTOR _pos) { localPosPoint1_ = _pos; }

	/// <summary>
	/// 2つ目の点の座標の設定
	/// </summary>
	/// <param name="_pos">2つ目の点の座標</param>
	inline void SetLocalPosPoint2(const VECTOR _pos) { localPosPoint2_ = _pos; }

	/// <summary>
	/// 当たった時の情報の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>当たった時の情報</returns>
	inline const MV1_COLL_RESULT_POLY& GetHitInfo(void)const { return hitInfo_; }

	/// <summary>
	/// 当たった時の情報の設定
	/// </summary>
	/// <param name="_hitInfo">当たった時の情報</param>
	inline void SetHitInfo(const MV1_COLL_RESULT_POLY _hitInfo) { hitInfo_ = _hitInfo; }

private:

	//線分の要素
	VECTOR localPosPoint1_;	//1つ目の点の座標
	VECTOR localPosPoint2_;	//2つ目の点の座標

	//当たった時の情報(モデル)
	MV1_COLL_RESULT_POLY hitInfo_;	
};