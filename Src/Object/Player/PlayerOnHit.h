#pragma once
#include<DxLib.h>
#include<vector>
#include<map>
#include<functional>
#include<memory>
#include"../ObjectBase.h"

class PlayerAction;

class PlayerOnHit
{

public:

	/// <summary>
	/// 再生させたいSE以外すべて止める
	/// </summary>
	/// <param name="_action">アクションクラス</param>
	/// <param name="_colParam">当たり判定情報</param>
	/// <param name="_trans">モデル情報</param>
	/// <param name="_tag">当たり判定タグ</param>
	PlayerOnHit(PlayerAction& _action, std::vector<ObjectBase::ColParam>&_colParam,Transform& _trans,Collider::TAG _tag);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PlayerOnHit(void);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name=""></param>
	void Init(void);

	/// <summary>
	/// 当たった時のSEなどの読み込み
	/// </summary>
	/// <param name=""></param>
	void Load(void);

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	/// <param name=""></param>
	void Update(void);

	/// <summary>
	/// 当たった処理の更新
	/// </summary>
	void ColUpdate(const std::weak_ptr<Collider> _hitCol);

	/// <summary>
	/// デバッグ表示
	/// </summary>
	void DrawDebug(void);

	/// <summary>
	/// 移動後座標の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>移動後座標</returns>
	inline const VECTOR GetMovedPos(void) const { return movedPos_; }

	/// <summary>
	/// 死亡判定の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>死亡判定</returns>
	inline const bool GetIsDeath(void)const { return isDeath_; }

	/// <summary>
	/// ゴール判定の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>ゴール判定</returns>
	inline const bool GetIsGoal(void)const { return isGoal_; }

	/// <summary>
	/// 地面との当たり判定の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>地面との当たり判定</returns>
	inline const bool GetIsLandHit(void)const { return isLandHit_; }
	/// <summary>
	/// 頭上の当たり判定
	/// </summary>
	/// <param name=""></param>
	/// <returns>頭上の当たり判定</returns>
	inline const bool GetIsOverHead(void)const { return isHitOverHead_; }

	/// <summary>
	/// スライム床との当たり判定
	/// </summary>
	/// <param name=""></param>
	/// <returns>スライム床との当たり判定</returns>
	inline const bool GetIsSlimeHit(void)const { return isHitSlimeFloor_; }

	/// <summary>
	/// コインの枚数
	/// </summary>
	/// <param name=""></param>
	/// <returns>コインの枚数</returns>
	inline const int GetCoinNum(void)const { return coinNum_; }

	/// <summary>
	/// バネジャンプ力
	/// </summary>
	/// <param name=""></param>
	/// <returns>バネジャンプ力</returns>
	inline const float GetSpringJumpPow(void) { return springJumpPow_; }

	/// <summary>
	/// 移動後座標のセット
	/// </summary>
	/// <param name="_movedPos">移動後座標のセット</param>
	void SetMovedPos(const VECTOR& _movedPos) { movedPos_ = _movedPos; }

	/// <summary>
	/// コイン枚数
	/// </summary>
	/// <param name="_num">コイン枚数</param>
	inline void SetCoinNum(const int _num) { coinNum_ = _num; }

private:

	//プレイヤーの大きさ
	static constexpr float RADIUS = 25.0f;
	//当たり判定の押し出し回数
	static constexpr int COL_TRY_CNT_MAX = 10;
	//ヒットした法線方向へのオフセット
	static constexpr float HIT_NORMAL_OFFSET = 3.0f;
	//接地しているときのラインのコライダ
	static constexpr int UP_AND_DOWN_LINE_COL_NO = 0;
	//プレイヤーの体の球
	static constexpr int BODY_SPHERE_COL_NO = 1;
	//現在の座標と移動後座標を結んだ線のコライダ
	static constexpr int MOVE_LINE_COL_NO = 2;
	//プレイヤーの手の座標
	static constexpr int HAND_SPHERE_COL_NO = 4;
	//ラインの長さ
	static constexpr float LINE_RANGE = 10.0f;
	//プレイヤーの上下の座標
	static constexpr VECTOR LOCAL_UP_POS = { 0.0f,RADIUS + LINE_RANGE,0.0f };//プレイヤーの上
	static constexpr VECTOR LOCAL_DOWN_POS = { 0.0f,-RADIUS - LINE_RANGE,0.0f };//プレイヤーの下
	//スライム床上でのジャンプ力
	static constexpr float SPRING_JUMP_POW = 30.0f;
	//当たり判定のめりこみ防止用
	static constexpr float POSITION_OFFSET = 1.0f;
	//オブジェクトの下に当たった時の跳ね返り減速セット
	static constexpr float DOWN_BOUNCE_DECELERATION = -10.0f;

	//移動量ラインオフセット
	static constexpr float MOVE_LINE_Y_OFFSET = Player::RADIUS - 1.0f;
	//移動量更新条件の移動ラインの長さ
	static constexpr float MOVE_LINE_Y_CHECK_VALUE = Player::RADIUS + 1.5f;

	//プレイヤーの目の高さ
	static constexpr VECTOR EYE_HEIGHT = { 0.0f, 15.0f, 0.0f };
	//プレイヤーのアイラインの長さ
	static constexpr VECTOR EYE_RANGE = { 0.0f, 15.0f, 100.0f };

	//バネジャンプによる増加ジャンプ力
	static constexpr float ADD_SPRING_JUMP_POW = 10;

	//プレイヤー
	PlayerAction& action_;
	//移動量
	VECTOR movedPos_;
	//移動前
	VECTOR moveDiff_;
	//プレイヤーの情報
	Transform& trans_;

	//当たり判定ごとの更新
	std::map<Collider::TAG, std::function<void(const std::weak_ptr<Collider> _hitCol)>>colUpdates_;
	//当たり判定関係
	std::vector<ObjectBase::ColParam>& colParam_;
	Collider::TAG tag_;	//プレイヤーの当たり判定タグ

	//ゴール判定
	bool isGoal_;
	//死亡判定
	bool isDeath_;
	//地面と当たっているか
	bool isLandHit_;
	//プレイヤーの頭上が当たっているか
	bool isHitOverHead_;
	//スライム床と当たっているか
	bool isHitSlimeFloor_;
	//プレイヤーの横がわが当たっている
	bool isSide_;
	//コインの枚数
	int coinNum_;
	//バネジャンプ力
	float springJumpPow_;

	//モデルの当たった時の共通処理
	void HitModelCommon(const std::weak_ptr<Collider> _hitCol);			

	//オブジェクト毎の当たった時にされる処理
	void CollNone(void);												//当たっても何もしない(プレイヤー側で何も起きない)
	void CollFloor(const std::weak_ptr<Collider> _hitCol);				//通常床
	void CollMoveFloor(const std::weak_ptr<Collider> _hitCol);			//動く床
	void CollSlimeFloor(const std::weak_ptr<Collider> _hitCol);			//スライム床
	void CollKillerItemSpecific(const std::weak_ptr<Collider> _hitCol);	//アイテムの特定の場所に当たったら死ぬ
	void CollKillerItemAll(void);										//すべての場所に当たったら死ぬ
	void CollWind(const std::weak_ptr<Collider> _hitCol);				//風
	void ColPunch(const std::weak_ptr<Collider> _hitCol);				//パンチの当たり処理
	void ColSpring(const std::weak_ptr<Collider> _hitCol);				//ばねとの当たり判定
	void ColGoal(const std::weak_ptr<Collider> _hitCol);				//ゴールした時の処理
	void CollCoin(void);												//コインと当たった時
};

