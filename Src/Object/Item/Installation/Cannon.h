#pragma once
#include "../ItemBase.h"

class CannonShot;

class Cannon : public ItemBase
{
public:

	//弾
	static constexpr int SHOT_MAX = 3;				//弾の生成数
	static constexpr float SHOT_INTERVAL = 5.0f;	//弾の生成感覚

	//マップ
	static constexpr IntVector3 MAP_SIZE = { 2,2,2 };				//マップサイズ
	static constexpr VECTOR MAP_LOCALPOS = { 60.0f,0.0f,40.0f };	//マップとの相対座標

	//サイズ
	static constexpr VECTOR MODEL_SIZE = { 120.0f,100.0f,80.0f };	//モデルのサイズ

	//エイム
	static constexpr float  AIM_RADIUS = 500.0f;	//狙う範囲半径
	static constexpr float AIM_TIME_TURRET = 1.0f;	//対象に向くまでの時間
	static constexpr float AIM_TIME_BARREL = 1.0f;	//対象に向くまでの時間

	//砲身
	static constexpr VECTOR BARREL_LOCAL_POS = { 0.0f, 75.0f, -0.0f };	//砲身の相対座標
	static constexpr VECTOR BARREL_LOCAL_ROT = { 20.0f, 0.0f, 0.0f };	//砲身の相対回転

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

	/// <summary>
	/// 当たり判定後の処理
	/// </summary>
	/// <param name="_hitColTag">相手側の当たり判定</param>
	void OnHit(const std::weak_ptr<Collider> _hitCol)override;

	//モデルの色変え
	void ChangeModelColor(const COLOR_F _colorScale)override;

	//狙う対象の設定
	void SetTargetPos(const VECTOR _targetPos) { targetPos_ = _targetPos; }

private:

	//弾関係
	std::unique_ptr<CannonShot> shots_[SHOT_MAX];		//弾
	int shotNum_;										//弾の数
	float shotCreateCnt_;								//弾の生成間隔カウンタ
	
	//モデル関係
	Transform barrelTrans_;	//砲身用モデル情報
	VECTOR turretAddRot_;	//砲台の回転量
	VECTOR barrelAddRot_;	//砲身の回転量

	//対象
	VECTOR targetPos_;		//狙う対象の位置情報
	VECTOR targetVec_;		//狙う対象に対しての方向ベクトル

	//砲台の回転
	void RotateTurret(void);

	//砲身の回転
	void RotateBarrel(void);

	//弾の生成
	void CreateShot(void);

	//弾の削除
	void DeleteShot(void);

	//射程内かの判定
	bool IsWithinRange(void);
};

