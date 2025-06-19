#pragma once
#include<DxLib.h>
#include<memory>
#include<functional>
#include<map>

class Player;
class PlayerAction
{
public:
	enum class ATK_ACT
	{
		NONE,	//何もなし
		INPUT,	//入力
		MOVE,	//移動
		PUNCH,	//パンチ
		KNOCKBACK,//パンチされた状態
		JUMP
	};
	PlayerAction(Player& _player);
	~PlayerAction(void);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name=""></param>
	void Init(void);

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name=""></param>
	void Update(void);
	



private:
	//*******************************************
	//定数
	//*******************************************
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

	//吹き飛び効果時間
	static constexpr float PUNCHED_TIME = 0.2f;

	//-------------------------------------------------
	//メンバ変数
	//-------------------------------------------------
	//状態遷移
	std::map<ATK_ACT, std::function<void(void)>>changeAction_;

	//状態更新
	std::function<void(void)>actionUpdate_;

	//状態
	ATK_ACT act_;

	//地面との当たり判定
	bool isLandHit_;
	//移動
	//------------------------
	float speed_;			// 移動スピード
	VECTOR moveDir_;		// 移動方向
	VECTOR movePow_;		// 移動量
	VECTOR dir_;			//方向

	float stepRotTime_;

	//ジャンプ
	//-----------------------
	bool isJump_;			// ジャンプ判定
	float stepJump_;		// ジャンプの入力受付時間
	VECTOR jumpPow_;		// ジャンプ量
	float jumpDeceralation_;	//ジャンプ減衰量

	//パンチ
	//-----------------------
	bool isPunchHitTime_;		//パンチ当たり判定の時間フラグ
	float punchCnt_;				//パンチカウント
	float punchCoolCnt_;			//パンチクールタイム
	VECTOR punchPos_;			//攻撃座標
	float punchedCnt_;			//パンチ効果時間カウント
	//-------------------------------------------------
	//メンバ関数
	//-------------------------------------------------
	//何もしない
	void NoneUpdate(void);

	//入力
	void ActionInputUpdate(void);
	void ChangeInput(void);

	//何もなし
	void ChangeNone(void);

	//移動状態の更新
	void MoveUpdate(void);
	//入力方向に応じて方向を決める
	void MoveDirFronInput(void);
	//移動に変更する
	void ChangeMove(void);
	//毎フレーム移動方向とスピードを更新する
	void UpdateMoveDirAndPow(void);


	//ジャンプ
	void JumpUpdate(void);
	void Jump(void);
	void ChangeJump(void);

	//パンチ
	void Punch(void);
	void ChangePunch(void);

	//ノックバック
	void KnockBack(void);
	void ChangeKnockBack(void);

	//回転
	void Rotate(void);
	//最終的に動かしたい角度の設定
	void SetGoalRotate(double _deg);
};

