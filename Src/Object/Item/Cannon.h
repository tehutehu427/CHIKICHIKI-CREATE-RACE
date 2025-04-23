#pragma once
#include "ItemBase.h"

class Cannon : public ItemBase
{
public:

	//マップサイズ
	static constexpr IntVector3 MAP_SIZE = { 2,2,2 };

	//狙う範囲半径
	static constexpr float  AIM_RADIUS = 200.0f;

	//砲台の角度調整用補間座標
	//static constexpr VECTOR TURRET_RELATIVE_POS = { 55.0f,0.0f,110.0f };
	//砲身の相対座標補間用
	//static constexpr VECTOR BARREL_RELATIVE_POS = { 0.0f, 85.0f, 135.0f };
	static constexpr VECTOR BARREL_LOCAL_POS = { 0.0f, 75.0f, -0.0f };

	//コンストラクタ
	Cannon();
	//デストラクタ
	~Cannon()override;

	//個人設定
	void SetParam(void)override;
	//更新
	void Update(void)override;
	//描画
	void Draw(void)override;
	//解放
	void Release(void)override;

	//狙う対象の設定
	void SetTargetPos(const VECTOR _targetPos) { targetPos_ = _targetPos; }

private:

	//砲身用モデル情報
	Transform barrelTrans_;

	//砲台の回転量
	VECTOR turretAddRot_;

	//砲身の回転量
	VECTOR barrelAddRot_;

	//狙う対象の位置情報
	VECTOR targetPos_;

	/// <summary>
	/// 回転
	/// </summary>
	/// <param name="_trans">回転させるモデル情報</param>
	/// <param name="_addAxis">加える回転情報</param>
	/// <param name="_relativePos">モデル自体の相対座標</param>
	void Rotate(Transform& _trans, const VECTOR _addAxis, const VECTOR _relativePos = {0.0f,0.0f,0.0f})const;
};

