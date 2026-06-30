#pragma once
#include"Geometry.h"

class Model : public Geometry
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_pos">追従する親の座標</param>
	/// <param name="_rot">追従する親の回転</param>
	/// <param name="_modelId">追従する親のモデルID</param>
	Model(const VECTOR& _pos, const Quaternion& _rot, const int _modelId);

	/// <summary>
	/// コピーコンストラクタ
	/// </summary>
	/// <param name="_copyBase">コピー元</param>
	/// <param name="_pos">追従する親の座標</param>
	/// <param name="_rot">追従する親の回転</param>
	/// <param name="_modelId">追従する親のモデルID</param>
	Model(const Model& _copyBase, const VECTOR& _pos, const Quaternion& _rot);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~Model(void)override;

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
	/// 親のモデルIDの取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>親のモデルID</returns>
	inline const int GetParentModel(void)const { return parentModelId_; };

	/// <summary>
	/// 当たった時の情報の取得(線分)
	/// </summary>
	/// <param name=""></param>
	/// <returns>当たった時の情報</returns>
	inline const MV1_COLL_RESULT_POLY& GetHitLineInfo(void)const { return hitLineInfo_; }

	/// <summary>
	/// 当たった時の情報の設定(線分)
	/// </summary>
	/// <param name="_hitInfo">当たった時の情報</param>
	inline void SetHitLineInfo(const MV1_COLL_RESULT_POLY _hitInfo) { hitLineInfo_ = _hitInfo; }

	/// <summary>
	/// 当たった時の情報の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>当たった時の情報</returns>
	inline const MV1_COLL_RESULT_POLY_DIM& GetHitInfo(void)const { return hitInfo_; }

	/// <summary>
	/// 当たった時の情報の設定
	/// </summary>
	/// <param name="_hitInfo">当たった時の情報</param>
	inline void SetHitInfo(MV1_COLL_RESULT_POLY_DIM _hitInfo) { std::swap(hitInfo_, _hitInfo); }

private:

	//モデルの要素
	int parentModelId_;					//親のモデルID	aaaaaaaaa
	MV1_COLL_RESULT_POLY hitLineInfo_;	//当たった時の情報(線)
	MV1_COLL_RESULT_POLY_DIM hitInfo_;	//当たった時の情報(球、カプセル)
};
