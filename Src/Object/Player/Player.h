#pragma once
#include <memory>
#include <map>
#include <functional>
#include "../Common/AnimationController.h"
#include "../Common/Capsule.h"
#include "../../Manager/Camera.h"
#include"./Process/PMove.h"
#include"./Process/PJump.h"
#include "../ObjectBase.h"

#define DEBUG_ON
class Camera;
class PMove;
class PJump;
class Player :public ObjectBase
{
public:
	//******************************************
	//定数
	//******************************************
	//スピード
	static constexpr float SPEED_MOVE = 5.0f;
	static constexpr float SPEED_RUN = 10.0f;

	// 状態
	enum class STATE
	{
		NONE,
		PLAY,
		WARP_RESERVE,
		WARP_MOVE,
		DEAD,
		VICTORY,
		END
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
	Player(void);

	// デストラクタ
	~Player(void) = default;

	void Load(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;

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

	//オブジェクト関連
	//--------------------------------------------
	// アニメーション
	std::shared_ptr<AnimationController> animationController_;

	//カプセル
	std::shared_ptr<Capsule> capsule_;

	//カメラ
	std::shared_ptr<Camera> camera_;

	//操作関連
	std::shared_ptr<PMove> pMove_;
	std::shared_ptr<PJump> pJump_;
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

