#pragma once
#include <memory>
#include <map>
#include <functional>
#include "../Common/AnimationController.h"
#include"../../Manager/Game/CollisionManager.h"
#include "../Object/item/ItemBase.h"
#include"./Process/PlayerInput.h"
#include "../ObjectBase.h"
#include"../Scene/Game/GameScene.h"

#define DEBUG_ON
class Camera;
class PlayerInput;
class Player :public ObjectBase
{
public:
	//******************************************
	//定数
	//******************************************
	//プレイヤー情報
	static constexpr VECTOR CAPSULE_TOP = { 0.0f, 110.0f, 0.0f };
	static constexpr VECTOR CAPSULE_DOWN = { 0.0f, 0.0f, 0.0f };
	//半径
	static constexpr float RADIUS = 25.0f;
	//プレイヤーの大きさ
	static constexpr VECTOR MODEL_SCL = { 1.0f,1.0f,1.0f };
	//プレイヤー１のX座標
	static constexpr float PLAYER_ONE_POS_X = -20.0f;
	//座標の間隔
	static constexpr float DISTANCE_POS = 50.0f;
	//当たり判定の押し出し回数
	static constexpr int COL_TRY_CNT_MAX = 10;


	//落ちるアニメーションのスタート
	static constexpr float FALL_ANIM_START = 32.0f;
	//落ちるアニメーションの終わり
	static constexpr float FALL_ANIM_END = 59.0f;

	//死ぬ判定の座標の基準
	static constexpr float DEATH_POS_Y = -600.0f;
	//移動
	//----------------------------------
	//移動スピード
	static constexpr float MOVE_SPEED = 7.0f;
	//ぶっ飛ぶスピード
	static constexpr float FLY_AWAY_SPEED = 12.0f;
	//落ちているときの重力制限(jumpPowに加算しているのでjumpPowに適用)
	static constexpr float LIMIT_GRAVITY = -20.0f;

	//----------------------------------
	//ジャンプ
	//----------------------------------
	//ジャンプ力
	static constexpr float POW_JUMP = 20.0f;
	//ジャンプ加速の倍率
	static constexpr float TIME_JUMP_SCALE = 1.0f;
	//ジャンプ時間
	static constexpr float TIME_JUMP = 3.0f;

	//----------------------------------
	//パンチ
	//----------------------------------
	//パンチ有効時間
	static constexpr float PUNCH_TIME_MAX = 0.5f;
	//パンチクールタイム
	static constexpr float PUNCH_COOL_TIME = 0.5f;
	// 回転完了までの時間
	static constexpr float TIME_ROT = 0.1f;
	//パンチの当たり判定時間中フラグを始めるアニメーションステップ
	static constexpr float PUNCH_HIT_START_ANIM_STEP = 22.0f;
	//パンチの当たり判定時間中フラグを終えるアニメーションステップ
	static constexpr float PUNCH_HIT_END_ANIM_STEP = 35.0f;
	//パンチの範囲
	static constexpr float PUNCH_RADIUS = 20.0f;
	//吹き飛び効果時間
	static constexpr float PUNCHED_TIME = 0.2f;

	//--------------------------------------------------
	//当たり判定
	//--------------------------------------------------
	//当たり判定のめりこみ防止用
	static constexpr float POSITION_OFFSET = 0.1f;

	//***********************************
	//アニメーション関連
	//***********************************
	static constexpr float DEFAULT_SPD = 60.0f;

	//当たり判定を行う範囲
	static constexpr int COL_RANGE = 1;


	enum class FLOOR_COL
	{
		NONE
		,SIDE
		,TOP
	};

	enum class ATK_ACT
	{
		NONE,
		MOVE,
		PUNCH,
		JUMP
	};

	// アニメーション種別
	enum class ANIM_TYPE
	{
		NONE=0,
		IDLE=1,
		WALK=2,
		FALL=4,
		DAMAGE = 8,
		PUNCH = 12,
		JUMP = 13,
		LAND=14,
	};

	struct CUBE
	{
		VECTOR centerPos;
		VECTOR leftPos;
		VECTOR rightPos;
		VECTOR upPos;
		VECTOR downPos;
	};

	

	//******************************************
	// コンストラクタ
	Player(int _playerNum,PlayerInput::CNTL _cntl);

	// デストラクタ
	~Player(void);

	void Load(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;

	//ゲッタ
	//******************************************
	//プレイヤー番号
	inline const int GetPlayerNum(void)const { return playerNum_; }

	inline const PlayerInput::CNTL GetCntl(void) { return cntl_; }

	inline const VECTOR GetMovePow(void) { return movePow_; }
	//入力
	inline const std::weak_ptr<PlayerInput> GetInput(void)const { return input_; }

	//パンチ中ゲッタ
	inline const bool GetIsPunch(void) { return isPunchHitTime_; }

	//パンチ座標
	inline const VECTOR GetPunchPos(void) { return punchPos_; }

	//プレイヤー座標
	inline const VECTOR GetPos(void)const { return trans_.pos; }

	//移動後のプレイヤー座標
	inline const VECTOR GetMovedPos(void)const { return movedPos_; }

	//当たり判定を確認しているマップ座標
	inline const IntVector3 GetColPos(void)const { return colPos_; }

	//死んだ判定
	inline bool IsDeath(void);

	//当たったアイテム
	const ItemBase::ITEM_TYPE GetHitItemType(void)const { return hitItemType_; }

	//プレイヤー番号ゲット
	inline const int PlayerNum(void) { return playerNum_; }

	//******************************************
	//セッタ
	//******************************************
	//モデル情報をマネージャからセット
	void SetTrans(const Transform _trans) { trans_ = _trans; }

	//当たり判定
	void SetCollision(const bool _isCol) { isCol_ = _isCol; }

	//移動量セット(マネージャ用)
	void SetMovePow(const VECTOR _vec) { movePow_ = _vec; }

	//パンチされたフラグ
	void SetIsPunched(const bool _isPunched) { isPunched_ = _isPunched; }

	//方向
	void SetDir(const VECTOR _dir) { dir_ = _dir; }

	/// <summary>
	/// 座標
	/// </summary>
	/// <param name="_worldPos">ワールド座標</param>
	void SetPos(const VECTOR _worldPos) { trans_.pos = _worldPos; };
	//コントローラーセット
	void SetCntl(PlayerInput::CNTL _cntl) { cntl_ = _cntl; }

	//アイテム当たり判定情報セッタ
	//void SetItemHitColInfo(const GameScene::ITEM_COL_INFO _colInfo) { colInfo_ = _colInfo; }
	//当たった時の処理
	void HitAction(Collider::COL_TAG _tag, bool _isHit, VECTOR _hitPos, VECTOR _itemPos);

	//当たり判定
	void Collision(void);

#ifdef DEBUG_ON

	int tag_;	//当たり判定のタグ
	//デバッグキューブのサイズ
	static constexpr float CUBE_W = 200.0F;
	static constexpr float CUBE_H = 10.0F;
	static constexpr float CUBE_D = 200.0F;
	int GetTag(void) { return tag_; }
#endif // DEBUG_ON

	

private:
	//******************************************
	//メンバ変数
	//******************************************
	// 移動後の座標
	VECTOR movedPos_;

	//移動座標
	VECTOR moveDiff_;

	//アイテムとのローカル座標
	VECTOR itemLocalPos_;

	//入力デバイス
	PlayerInput::CNTL cntl_;

	//ゲームパッド番号
	InputManager::JOYPAD_NO padNum_;

	//アイテムの支点
	std::vector<IntVector3> itemLPos_;

	//当たり判定で調べる座標
	IntVector3 colPos_;

	

	//オブジェクト関連
	//--------------------------------------------
	// アニメーション
	std::shared_ptr<AnimationController> animationController_;

	//操作入力
	std::shared_ptr<PlayerInput> input_;

	//プレイヤー単体が持っているもの
	int playerNum_;			//プレイヤー番号

	//当たり判定関連
	//----------------------------------------------------
	//using ITEM_COL = GameScene::ITEM_COL_INFO;
	//他プレイヤーとの当たりフラグ　true:当たっている
	bool isCol_;

	//当たっているアイテムタイプ
	ItemBase::ITEM_TYPE hitItemType_;	

	//当たっているオブジェクトごとの処理
	//状態遷移用
	std::map<CollisionManager::COL_TAG, std::function<void(void)>>colChanges_;
	//更新
	std::function<void(void)> colUpdate_;

	//アイテムのどこと当たっているか
	//GameScene::ITEM_COL_INFO colInfo_;
	//----------------------------------------------------

	//アクション関係
	//----------------------------------------
	//移動
	//------------------------
	float speed_;			// 移動スピード
	VECTOR moveDir_;		// 移動方向
	VECTOR movePow_;		// 移動量
	VECTOR dir_;			//方向

	//回転
	Quaternion playerRotY_;
	Quaternion goalQuaRot_;
	float stepRotTime_;

	//ジャンプ
	//-----------------------
	bool isJump_;			// ジャンプ判定
	float stepJump_;		// ジャンプの入力受付時間
	VECTOR jumpPow_;		// ジャンプ量
	float jumpDeceralation_;	//ジャンプ減衰量
	float fallCnt_;			//落下カウント

	//パンチ
	//-----------------------
	bool isPunch_;			//パンチ中フラグ
	bool isPunchHitTime_;		//パンチ当たり判定の時間フラグ
	float punchCnt_;				//パンチカウント
	float punchCoolCnt_;			//パンチクールタイム
	VECTOR punchPos_;			//攻撃座標
	bool isPunched_;			//他プレイヤーからのパンチを受けたか
	float punchedCnt_;			//パンチ効果時間カウント

	//当たり判定
	VECTOR gravHitPosUp_;	//重力上方向の座標
	VECTOR gravHitPosDown_;	//重力下方向

#ifdef DEBUG_ON
	VECTOR cubeMovePos_;
	VECTOR cubePos_;
	CUBE cube_;
#endif // DEBUG_ON



	//--------------------------------------------
	//******************************************
	//メンバ関数
	//******************************************
#ifdef DEBUG_ON
	void DrawDebug(void);
#endif // DEBUG_ON
	//アクション関係
	//------------------------------
	void Action(void);
	//移動
	void Move(void);

	//回転
	void Rotate(void);

	//最終的に動かしたい角度の設定
	void SetGoalRotate(double _deg);



	//ジャンプ
	void Jump(void);

	//パンチ
	void Punch(void);
	//------------------------------
	
	/// <summary>
	/// 座標を足して回転
	/// </summary>
	/// <param name="_followPos">追従対象の座標</param>
	/// <param name="_followRot">追従対象の角度</param>
	/// <param name="_localPos">相対座標</param>
	VECTOR AddPosRotate(VECTOR _followPos, Quaternion _followRot,VECTOR _localPos);

	//アイテム都の当たり判定
	void HitItem(const IntVector3 _colPos);



	//地面との当たり判定(動いてる床とか)
	void UpDownColl(void);

	//周囲との当たり判定
	void ArroundColl(Transform _itemTrans);




	//当たった時の処理
	void Onhit(CollisionManager::COL_TAG)override;

	//当たった時の処理
	//当たっていない
	void OnHitNone(void);
	// 
	//床
	void OnHitFloor(const IntVector3 _colPos);
	//即死アイテム
	void OnHitDeathItem(const IntVector3 _colPos);


#ifdef DEBUG_ON
	void CubeMove(void);
	bool CollCube(void);
#endif // DEBUG_ON

};

