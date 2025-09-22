#pragma once
#include"Geometry.h"

class Cube : public Geometry
{

public:

	//バウンディングボックス
	struct OBB
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
	/// <param name="_min">親から見た、箱の最小地点</param>
	/// <param name="_max">親から見た、箱の最大地点</param>
	Cube(const VECTOR& _pos, const Quaternion& _rot, const VECTOR _min, const VECTOR _max);

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_pos">追従する親の座標</param>
	/// <param name="_rot">追従する親の回転</param>
	/// <param name="_halfSize">箱の半分サイズ</param>
	Cube(const VECTOR& _pos, const Quaternion& _rot, const VECTOR _halfSize);

	/// <summary>
	/// コピーコンストラクタ
	/// </summary>
	/// <param name="_copyBase">コピー元</param>
	/// <param name="_pos">追従する親の座標</param>
	/// <param name="_rot">追従する親の回転</param>
	Cube(const Cube& _copyBase, const VECTOR& _pos, const Quaternion& _rot);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~Cube(void)override;

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
	/// 回転バウンティボックスの取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>回転バウンティボックス</returns>
	inline const OBB& GetObb(void)const { return obb_; }

	/// <summary>
	/// 回転バウンティボックスの設定
	/// </summary>
	/// <param name="_obb">回転バウンティボックス</param>
	inline void SetObb(const OBB& _obb) { obb_ = _obb; }

	/// <summary>
	/// 箱の最小地点の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>箱の最小地点</returns>
	inline const VECTOR GetVecMin(void)const { return obb_.vMin; }

	/// <summary>
	/// 箱の最大地点の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>箱の最大地点</returns>
	inline const VECTOR GetVecMax(void)const { return obb_.vMax; }

	/// <summary>
	/// 箱の最小地点の設定
	/// </summary>
	/// <param name="_min">箱の最小地点</param>
	inline void SetVecMin(const VECTOR& _min) { obb_.vMin = _min; }

	/// <summary>
	/// 箱の最大地点の設定
	/// </summary>
	/// <param name="_max">箱の最大地点</param>
	inline void SetVecMax(const VECTOR& _max){ obb_.vMax = _max; }

	/// <summary>
	/// サイズの半分の設定
	/// </summary>
	/// <param name="_halfSize">サイズの半分</param>
	inline void SetHalfSize(const VECTOR& _halfSize);

private:

	//回転バウンディングボックス
	OBB obb_;			

	/// <summary>
	/// 箱の回転情報の取得
	/// </summary>
	/// <param name="_num">回転軸の番号</param>
	/// <returns>箱の回転情報</returns>
	inline const VECTOR GetAxis(const int _num)const { return obb_.axis[_num]; }

	/// <summary>
	/// クォータニオンから回転軸を計算
	/// </summary>
	/// <param name=""></param>
	void UpdateObbAxis(void);

	/// <summary>
	/// 各頂点の計算（ワールド座標）
	/// </summary>
	/// <param name="outVertices">頂点</param>
	void CalculateVertices(VECTOR outVertices[8]) const;
};