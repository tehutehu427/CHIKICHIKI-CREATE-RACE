#pragma once

#include"Geometry.h"

class Transform;
class Model;
class Cube;
class Sphere;
class Capsule;

class Model : public Geometry
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_parent">追従する親</param>
	Model(const Transform& _parent);

	/// <summary>
	/// コピーコンストラクタ
	/// </summary>
	/// <param name="_copyBase">コピー元</param>
	/// <param name="_parent">追従する親</param>
	Model(const Model& _copyBase, const Transform& _parent);

	//デストラクタ
	~Model(void)override;

	//描画
	void Draw(void)override;

	//各種当たり判定
	const bool IsHit(const Geometry& _geometry)const override;
	const bool IsHit(const Model& _model)const override;
	const bool IsHit(const Cube& _cube)const override;
	const bool IsHit(const Sphere& _sphere)const override;
	const bool IsHit(const Capsule& _capsule)const override;
	const bool IsHit(Line& _line) override;

	//親モデルIDの取得
	inline const int GetParentModel(void)const;

	//当たった時の情報取得
	inline const MV1_COLL_RESULT_POLY& GetHitInfo(void)const { return hitInfo_; }

	//当たった時の情報設定
	inline void SetHitInfo(const MV1_COLL_RESULT_POLY _hitInfo) { hitInfo_ = _hitInfo; }

private:

	MV1_COLL_RESULT_POLY hitInfo_;	//当たった時の情報
};
