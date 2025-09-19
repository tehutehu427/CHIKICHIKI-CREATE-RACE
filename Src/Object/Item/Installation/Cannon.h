#pragma once
#include<map>
#include "../ItemBase.h"

class CannonShot;
class ToonStyle;

class Cannon : public ItemBase
{

public:

	//弾
	static constexpr float SHOT_INTERVAL = 5.0f;	//弾の生成感覚

	//マップ
	static constexpr IntVector3 MAP_SIZE = { 2,2,2 };				//マップサイズ
	static constexpr IntVector3 HIT_SIZE = { 0,0,0 };				//当たり判定用サイズ
	static constexpr VECTOR MAP_LOCALPOS = { 60.0f,0.0f,40.0f };	//マップとの相対座標

	//モデルのサイズ
	static constexpr VECTOR MODEL_SIZE = { 120.0f,100.0f,80.0f };	

	//エイム
	static constexpr VECTOR INIT_TARGET_POS = { -1000.0f,-1000.0f,-1000.0f };	//初期ターゲット補正
	static constexpr float  AIM_RADIUS = 1200.0f;								//狙う範囲半径
	static constexpr float AIM_TIME_TURRET = 1.0f;								//砲台が対象に向くまでの時間
	static constexpr float AIM_TIME_BARREL = 1.0f;								//砲身が対象に向くまでの時間

	//砲身
	static constexpr VECTOR BARREL_LOCAL_POS = { 0.0f, 75.0f, -0.0f };	//砲身の相対座標
	static constexpr VECTOR BARREL_LOCAL_ROT = { 0.0f, 0.0f, 0.0f };	//砲身の相対回転

	//コライダ
	static constexpr int AIM_COL_NUM = 2;	//エイム範囲のコライダ番号

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name=""></param>
	Cannon(void);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~Cannon(void)override;

	/// <summary>
	/// 個人設定
	/// </summary>
	/// <param name=""></param>
	void SetParam(void)override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name=""></param>
	void Update(void)override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name=""></param>
	void Draw(void)override;

	/// <summary>
	/// 当たり判定サイズの取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>当たり判定</returns>
	const IntVector3 GetHitSize(void)const override { return MAP_SIZE + HIT_SIZE; }

	/// <summary>
	/// 当たり判定後の処理
	/// </summary>
	/// <param name="_hitColTag">相手側の当たり判定</param>
	void OnHit(const std::weak_ptr<Collider> _hitCol)override;

	/// <summary>
	/// モデルのカラーを設定
	/// </summary>
	/// <param name="_r">赤</param>
	/// <param name="_g">緑</param>
	/// <param name="_b">青</param>
	/// <param name="_a">アルファ値</param>
	void SetModelColor(const float _r, const float _g, const float _b, const float _a) override;

	/// <summary>
	/// アイテムの値リセット
	/// </summary>
	/// <param name=""></param>
	void ResetValue(void)override;

	/// <summary>
	/// 砲台の値合わせ
	/// </summary>
	/// <param name=""></param>
	void BarrelValueToTurret(void);

private:

	//シェーダー
	std::unique_ptr<ToonStyle> toonBarrel_;

	//弾関係
	std::unique_ptr<CannonShot> shot_;	//弾
	float shotCreateCnt_;					//弾の生成間隔カウンタ
	
	//モデル関係
	Transform barrelTrans_;	//砲身用モデル情報
	VECTOR turretAddRot_;	//砲台の回転量
	VECTOR barrelAddRot_;	//砲身の回転量

	//対象
	std::map<Collider::TAG,VECTOR> compPos_;	//狙う対象の比較用位置情報
	VECTOR targetPos_;							//狙う対象の位置情報

	//砲台の回転
	void RotateTurret(void);

	//砲身の回転
	void RotateBarrel(void);

	//弾の生成
	void CreateShot(void);

	//弾の削除
	void DeleteShot(void);

	//シェーダーの設定
	void InitShader(void) override;
};

