#pragma once

#include"Geometry.h"

class Cube : public Geometry
{
public:

	//バウンディングボックス
	struct BB
	{
		VECTOR vMin;
		VECTOR vMax;
		VECTOR axis[3];
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_pos">追従する親の座標</param>
	/// <param name="_rot">追従する親の回転</param>
	Cube(const VECTOR& _pos, const Quaternion& _rot);

	/// <summary>
	/// コピーコンストラクタ
	/// </summary>
	/// <param name="_copyBase">コピー元</param>
	/// <param name="_pos">追従する親の座標</param>
	/// <param name="_rot">追従する親の回転</param>
	Cube(const Cube& _copyBase, const VECTOR& _pos, const Quaternion& _rot);

	//デストラクタ
	~Cube(void)override;

	//描画
	void Draw(void)override;

	//各種当たり判定
	const bool IsHit(Geometry& _geometry)override;
	const bool IsHit(Model& _model)override;
	const bool IsHit(Cube& _cube)override;
	const bool IsHit(Sphere& _sphere)override;
	const bool IsHit(Capsule& _capsule)override;
	const bool IsHit(Line& _line)override;

	//サイズの半分の取得
	inline const VECTOR GetHalfSize(void)const { return halfSize_; }

	//サイズの半分の設定
	inline void SetHalfSize(const VECTOR& _halfSize) { halfSize_ = _halfSize; }

private:

	BB bb_;				//バウンディングボックス

	VECTOR halfSize_;	//箱の半分サイズ
};