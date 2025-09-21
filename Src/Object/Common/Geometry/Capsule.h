#pragma once
#include"Geometry.h"

class Capsule : public Geometry
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_pos">追従する親の座標</param>
	/// <param name="_rot">追従する親の回転</param>
	/// <param name="_localPosTop">上側の相対座標</param>
	/// <param name="_localPosDown">下側の相対座標</param>
	/// <param name="_radius">半径</param>
	Capsule(const VECTOR& _pos, const Quaternion& _rot, const VECTOR _localPosTop, const VECTOR _localPosDown, const float _radius);

	/// <summary>
	/// コピーコンストラクタ
	/// </summary>
	/// <param name="_copyBase">コピー元</param>
	/// <param name="_pos">追従する親の座標</param>
	/// <param name="_rot">追従する親の回転</param>
	Capsule(const Capsule& _copyBase, const VECTOR& _pos, const Quaternion& _rot);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~Capsule(void)override;

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
	/// ヒット後の処理
	/// </summary>
	/// <param name=""></param>
	void HitAfter(void)override;

	/// <summary>
	/// 親Transformからの相対位置を取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>上部の点の相対座標</returns>
	inline const VECTOR GetLocalPosTop(void) const { return localPosTop_; }
	
	/// <summary>
	/// 親Transformからの相対位置を取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>下部の点の相対座標</returns>
	inline const VECTOR GetLocalPosDown(void) const { return localPosDown_; }

	/// <summary>
	/// 親Transformからの相対位置をセット
	/// </summary>
	/// <param name="_pos">座標</param>
	inline void SetLocalPosTop(const VECTOR& _pos) { localPosTop_ = _pos; }

	/// <summary>
	/// 親Transformからの相対位置をセット
	/// </summary>
	/// <param name="_pos">座標</param>
	inline void SetLocalPosDown(const VECTOR& _pos) { localPosDown_ = _pos; }

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>上部の点のワールド座標</returns>
	inline const VECTOR GetPosTop(void) const { return GetRotPos(localPosTop_); }
	
	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>下部の点のワールド座標</returns>
	inline const VECTOR GetPosDown(void) const { return GetRotPos(localPosDown_); }

	/// <summary>
	/// 半径の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>半径</returns>
	inline const float GetRadius(void) const { return radius_; }

	/// <summary>
	/// 半径の設定
	/// </summary>
	/// <param name="_radius">半径</param>
	inline void SetRadius(float _radius) { radius_ = _radius; }

	/// <summary>
	/// 高さの取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>高さ</returns>
	inline const float GetHeight(void) const { return localPosTop_.y; }

	/// <summary>
	/// カプセルの中心座標の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>カプセルの中心座標</returns>
	inline const VECTOR GetCenter(void) const;

	/// <summary>
	/// 当たった時の情報の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>当たったポリゴン情報</returns>
	inline const MV1_COLL_RESULT_POLY_DIM& GetHitInfo(void)const { return hitInfo_; }

	/// <summary>
	/// 当たった時の情報設定
	/// </summary>
	/// <param name="_hitInfo">当たったポリゴン情報</param>
	inline void SetHitInfo(MV1_COLL_RESULT_POLY_DIM _hitInfo) { std::swap(hitInfo_, _hitInfo); }

private:

	//カプセルの要素
	VECTOR localPosTop_;	// 親Transformからの相対位置(上側)
	VECTOR localPosDown_;	// 親Transformからの相対位置(下側)
	float radius_;			// 半径

	//当たった時の情報(球、カプセル)
	MV1_COLL_RESULT_POLY_DIM hitInfo_;	
};
