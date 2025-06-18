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

	// デストラクタ
	~Line(void)override;

	// 描画
	void Draw(void)override;

	//各種当たり判定
	const bool IsHit(const Geometry& _geometry)const override;
	const bool IsHit(const Model& _model)const override;
	const bool IsHit(const Cube& _cube)const override;
	const bool IsHit(const Sphere& _sphere)const override;
	const bool IsHit(const Capsule& _capsule)const override;
	const bool IsHit(const Line& _line)const override;

	//1つ目の点の座標を取得
	inline const VECTOR GetLocalPosPoint1(void)const { return localPosPoint1_; }

	//2つ目の点の座標を取得
	inline const VECTOR GetLocalPosPoint2(void)const { return localPosPoint2_; }

	// 回転済みの1つ目の点の座標を取得
	inline const VECTOR GetPosPoint1(void) const { return GetRotPos(localPosPoint1_); }

	// 回転済みの2つ目の点の座標を取得
	inline const VECTOR GetPosPoint2(void) const { return GetRotPos(localPosPoint2_); }

	//1つ目の点の座標の設定
	inline void SetLocalPosPoint1(const VECTOR _pos) { localPosPoint1_ = _pos; }

	//2つ目の点の座標の設定
	inline void SetLocalPosPoint2(const VECTOR _pos) { localPosPoint2_ = _pos; }

	//当たった時の情報取得
	inline const MV1_COLL_RESULT_POLY& GetHitInfo(void)const { return hitInfo_; }

	//当たった時の情報設定
	inline void SetHitInfo(const MV1_COLL_RESULT_POLY _hitInfo) { hitInfo_ = _hitInfo; }

private:

	VECTOR localPosPoint1_;	//1つ目の点の座標
	VECTOR localPosPoint2_;	//2つ目の点の座標

	MV1_COLL_RESULT_POLY hitInfo_;	//当たった時の情報
};