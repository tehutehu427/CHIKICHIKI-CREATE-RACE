#pragma once
#include <memory>
#include <map>
#include <functional>
#include "../Common/AnimationController.h"
#include "../Common/Capsule.h"
#include "../../Manager/System/Camera.h"
#include"../Item/MoveHoriFloor.h"
#include"./Process/PMove.h"
#include"./Process/PlayerInput.h"
#include"./Process/PJump.h"
#include"./Process/PPunch.h"
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
	//移動
	//----------------------------------
	//スピード
	static constexpr float SPEED_MOVE = 5.0f;

	//----------------------------------
	//ジャンプ
	//----------------------------------
	//ジャンプ力
	static constexpr float POW_JUMP = 22.0f;

	//ジャンプ加速の倍率
	static constexpr float TIME_JUMP_IN = 4.0f;

	//デルタタイム
	static constexpr float DELTA_TIME = 1.0f / 60.0f;
	//----------------------------------
	//パンチ
	//----------------------------------
	//パンチ有効時間
	static constexpr float PUNCH_TIME_MAX = 1.5f;

	// 回転完了までの時間
	static constexpr float TIME_ROT = 1.0f;

	//パンチのローカル座標
	static constexpr VECTOR PUNCH_LOCAL_POS = { 0.0f,50.0f,40.0f };


	// 状態
	enum class STATE
	{
		NONE,
		PLAY,
		DEAD,
		END
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
		IDLE,
		RUN,
		FAST_RUN,
		JUMP,
		WARP_PAUSE,
		FLY,
		FALLING,
		VICTORY
	};
	//******************************************
	// コンストラクタ
	Player(int _playerNum,Transform _trans,PlayerInput::CNTL _cntl);

	// デストラクタ
	~Player(void) = default;

	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	void Release(void)override{}

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
	//******************************************
	//セッタ
	//******************************************
	//モデル情報をマネージャからセット
	void SetTrans(const Transform _trans) { transform_ = _trans; }

	//当たり判定
	void SetCollision(const bool _isCol) { isCol_ = _isCol; }

	//移動量セット(マネージャ用)
	void SetMovePow(const VECTOR _vec) { movePow_ = _vec; }

#ifdef DEBUG_ON
	const void SetCntl(PlayerInput::CNTL _cntl) { cntl_ = _cntl; }
	const int PlayerNum(void) { return playerNum_; }
#endif // DEBUG_ON

	

private:
	//******************************************
	//メンバ変数
	//******************************************
	// 移動後の座標
	VECTOR movedPos_;

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
	float cnt_;				//パンチカウント
	VECTOR punchPos_;			//攻撃座標


	//--------------------------------------------
	//******************************************
	//メンバ関数
	//******************************************
#ifdef DEBUG_ON
	void DrawDebug(void);
#endif // DEBUG_ON
	//アクション関係
	//------------------------------
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

	// 着地モーション終了
	bool IsEndLanding(void);



};

