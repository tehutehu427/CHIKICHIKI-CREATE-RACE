#pragma once
#include <memory>
#include <map>
#include <functional>
#include "../Common/AnimationController.h"
#include "../Common/Capsule.h"
#include "../../Manager/System/Camera.h"
#include"./Process/PlayerInput.h"
#include "../ObjectBase.h"

#define DEBUG_ON
class Camera;
class PMove;
class PJump;
class PPunch;
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
	static constexpr float RADIUS = 20.0f;
	//移動
	//----------------------------------
	//移動スピード
	static constexpr float MOVE_SPEED = 3.0f;

	//ぶっ飛ぶスピード
	static constexpr float FLY_AWAY_SPEED = 30.0f;

	//----------------------------------
	//ジャンプ
	//----------------------------------
	//ジャンプ力
	static constexpr float POW_JUMP = 10.0f;

	//ジャンプ加速の倍率
	static constexpr float TIME_JUMP_SCALE = 1.0f;

	//ジャンプ時間
	static constexpr float TIME_JUMP = 3.0f;
	//----------------------------------
	//パンチ
	//----------------------------------
	//パンチ有効時間
	static constexpr float PUNCH_TIME_MAX = 1.5f;

	//パンチクールタイム
	static constexpr float PUNCH_COOL_TIME = 0.5f;

	// 回転完了までの時間
	static constexpr float TIME_ROT = 1.0f;

	//パンチのローカル座標
	static constexpr VECTOR PUNCH_LOCAL_POS = { 0.0f,50.0f,40.0f };

	//パンチの範囲
	static constexpr float PUNCH_RADIUS = 40.0f;

	//吹き飛び効果時間
	static constexpr float PUNCHED_TIME = 0.2f;

	//***********************************
	//アニメーション関連
	//***********************************
	static constexpr float DEFAULT_SPD = 60.0f;


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
		DAMAGE = 8,
		PUNCH = 11,
		JUMP = 12,
		LAND=13,
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
	Player(int _playerNum,Transform _trans,PlayerInput::CNTL _cntl);

	// デストラクタ
	~Player(void) = default;

	void Load(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;

	//ゲッタ
	//******************************************
	//プレイヤー番号
	const int GetPlayerNum(void)const { return playerNum_; }

	//カプセル情報
	const std::weak_ptr<Capsule> GetCapsule(void)const { return capsule_; }

	const PlayerInput::CNTL GetCntl(void) { return cntl_; }

	const VECTOR GetMovePow(void) { return movePow_; }
	//入力
	const std::weak_ptr<PlayerInput> GetInput(void)const { return input_; }

	//パンチ中ゲッタ
	const bool GetIsPunch(void) { return isPunch_; }

	//パンチ座標
	const VECTOR GetPunchPos(void) { return punchPos_; }

	//プレイヤー座標
	const VECTOR GetPos(void)const { return trans_.pos; }
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

#ifdef DEBUG_ON
	const void SetCntl(PlayerInput::CNTL _cntl) { cntl_ = _cntl; }
	const int PlayerNum(void) { return playerNum_; }

	//デバッグキューブのサイズ
	static constexpr float CUBE_W = 200.0F;
	static constexpr float CUBE_H = 10.0F;
	static constexpr float CUBE_D = 200.0F;
#endif // DEBUG_ON

	

private:
	//******************************************
	//メンバ変数
	//******************************************
	// 移動後の座標
	VECTOR movedPos_;

	//移動座標
	VECTOR moveDiff_;

	//入力デバイス
	PlayerInput::CNTL cntl_;

	//ゲームパッド番号
	InputManager::JOYPAD_NO padNum_;

	//オブジェクト関連
	//--------------------------------------------
	// アニメーション
	std::shared_ptr<AnimationController> animationController_;

	//カプセル
	std::shared_ptr<Capsule> capsule_;

	//操作入力
	std::shared_ptr<PlayerInput> input_;

	//プレイヤー単体が持っているもの
	int playerNum_;			//プレイヤー番号

	//他プレイヤーとの当たりフラグ　true:当たっている
	bool isCol_;

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
	float punchCnt_;				//パンチカウント
	float punchCoolCnt_;			//パンチクールタイム
	VECTOR punchPos_;			//攻撃座標
	bool isPunched_;			//他プレイヤーからのパンチを受けたか
	float punchedCnt_;			//パンチ効果時間カウント

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

	//重力による移動量
	void CalcGravityPow(void);

	//当たり判定
	void Collision(void);

#ifdef DEBUG_ON
	void CubeMove(void);
	bool CollCube(void);
#endif // DEBUG_ON

};

