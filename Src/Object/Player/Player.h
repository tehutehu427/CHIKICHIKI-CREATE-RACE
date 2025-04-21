#pragma once
#include <memory>
#include <map>
#include <functional>
#include "../Common/AnimationController.h"
#include "../Common/Capsule.h"
#include "../../Manager/System/Camera.h"
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
class Player :public ObjectBase
{
public:
	//******************************************
	//定数
	//******************************************
	//スピード
	static constexpr float SPEED_MOVE = 5.0f;
	static constexpr float SPEED_RUN = 10.0f;

	//プレイヤーの中心からの

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
	void Release(void)override;

	//ゲッタ
	//******************************************
	//カメラ
	const std::shared_ptr<Camera> GetCamera(void)const { return camera_; }

	//プレイヤー番号
	const int GetPlayerNum(void)const { return playerNum_; }

	//カプセル情報
	const std::shared_ptr<Capsule> GetCapsule(void)const { return capsule_; }
	//******************************************
	//セッタ
	//******************************************
	//モデル情報をマネージャからセット
	void SetTrans(const Transform _trans) { transform_ = _trans; }

	//当たり判定
	void SetCollision(const bool _isCol) { isCol_ = _isCol; }


private:
	//******************************************
	//メンバ変数
	//******************************************
	//状態管理
	//----------------------------------------------
	STATE state_;
	// 状態管理(状態遷移時初期処理)
	std::map<STATE, std::function<void(void)>> stateChanges_;
	// 状態管理(更新ステップ)
	std::function<void(void)> stateUpdate_;
	//--------------------------------------------
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

	//カメラ
	std::shared_ptr<Camera> camera_;

	//操作関連
	//移動
	std::shared_ptr<PMove> pMove_;
	//ジャンプ
	std::shared_ptr<PJump> pJump_;
	//パンチ
	std::shared_ptr<PPunch> pPunch_;

	//プレイヤー単体が持っているもの
	//プレイヤー番号
	int playerNum_;

	//他プレイヤーとの当たりフラグ　true:当たっている
	bool isCol_;

	//--------------------------------------------
	//******************************************
	//メンバ関数
	//******************************************
#ifdef DEBUG_ON
	void DrawDebug(void);
#endif // DEBUG_ON

	// 状態遷移
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStatePlay(void);

	//更新ステップ
	void UpdateNone(void);
	void UpdatePlay(void);

	//重力による移動量
	void CalcGravityPow(void);

	//当たり判定
	void Collision(void);

	// 着地モーション終了
	bool IsEndLanding(void);

};

