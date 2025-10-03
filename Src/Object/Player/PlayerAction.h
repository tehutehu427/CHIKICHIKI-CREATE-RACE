#pragma once
#include"../Manager/System/SoundManager.h"

class Player;
class PlayerInput;
class AnimationController;

class PlayerAction
{

public:

	//パンチ有効時間
	static constexpr float PUNCH_TIME_MAX = 0.5f;

	//アクション種類
	enum class ACTION_TYPE
	{
		NONE,	//何もなし
		ACTION_INPUT,	//入力
		MOVE,	//移動
		DASHMOVE,//ダッシュ
		PUNCH,	//パンチ
		KNOCKBACK,//パンチされた状態
		JUMP
	};

	enum class ACT_SE
	{
		DASH,	//ダッシュ
		JUMP,	//ジャンプ
		PUNCH,	//パンチ
		PUNCH_HIT,//パンチヒット
		SLIME,	//スライム
	};

	PlayerAction(Player& _player, SceneManager& _scnMng, AnimationController& _animationController);
	~PlayerAction(void);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name=""></param>
	void Init(void);

	/// <summary>
	/// 読み込み
	/// </summary>
	void Load(void);

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name=""></param>
	void Update(void);

	/// <summary>
	/// 状態遷移
	/// </summary>
	/// <param name="_act">遷移したい状態</param>
	void ChangeAction(ACTION_TYPE _act);

	/// <summary>
	/// 座標を足して回転
	/// </summary>
	/// <param name="_followPos">追従対象の座標</param>
	/// <param name="_followRot">追従対象の角度</param>
	/// <param name="_localPos">相対座標</param>
	VECTOR AddPosRotate(const VECTOR _followPos, const Quaternion _followRot, const VECTOR _localPos);

	/// <summary>
	/// 移動量
	/// </summary>
	/// <param name=""></param>
	/// <returns>移動量</returns>
	inline const VECTOR GetMovePow(void) { return movePow_; }

	/// <summary>
	/// ジャンプ力
	/// </summary>
	/// <param name=""></param>
	/// <returns>ジャンプ力</returns>
	inline const VECTOR GetJumpPow(void) { return jumpPow_; }

	/// <summary>
	/// プレイヤーの角度Y
	/// </summary>
	/// <param name=""></param>
	/// <returns>プレイヤーの角度Y</returns>
	inline const Quaternion GetPlayerRotY(void) { return playerRotY_; }

	/// <summary>
	/// プレイヤーの角度Y
	/// </summary>
	/// <param name=""></param>
	/// <returns>プレイヤーの角度Y</returns>
	inline const VECTOR& GetPunchPos(void) { return punchPos_; }

	/// <summary>
	/// パンチ中判定
	/// </summary>
	/// <param name=""></param>
	/// <returns>パンチ中判定</returns>
	inline bool GetIsHitPunch(void) { return isPunchHitTime_; }

	/// <summary>
	/// ジャンプ中
	/// </summary>
	/// <param name=""></param>
	/// <returns>ジャンプ中</returns>
	inline const bool GetIsJump(void) { return isJump_; }

	/// <summary>
	/// 状態
	/// </summary>
	/// <param name=""></param>
	/// <returns>状態</returns>
	inline const ACTION_TYPE GetAct(void)const { return act_; }
	
	/// <summary>
	/// ジャンプ減衰量の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>ジャンプ減衰量の取得</returns>
	inline float GetJumpDecel(void) { return jumpDeceralation_; }

	/// <summary>
	/// 移動スピードとダッシュスピードの設定
	/// </summary>
	/// <param name="_moveSpd">通常移動スピード</param>
	/// <param name="_dashSpd"></param>
	void SetSpeed(const float _moveSpd, const float _dashSpd);

	/// <summary>
	/// ジャンプ力
	/// </summary>
	/// <param name="_jumpPow">ジャンプ力</param>
	inline void SetJumpPow(const VECTOR _jumpPow) { jumpPow_ = _jumpPow; };

	/// <summary>
	/// 移動量
	/// </summary>
	/// <param name="_movePow">移動量</param>
	inline void SetMovePow(const VECTOR _movePow) { movePow_ = _movePow; }

	/// <summary>
	/// 空中かどうか
	/// </summary>
	/// <param name="_isJump"></param>
	inline void SetIsJump(const bool _isJump) { isJump_ = _isJump; }

	/// <summary>
	/// ジャンプ減速
	/// </summary>
	/// <param name="_decel">減速度</param>
	inline void SetJumpDecel(const float _decel) { jumpDeceralation_ = _decel; }

	/// <summary>
	/// ジャンプ減速量最大値
	/// </summary>
	/// <param name="_decelMax">ジャンプ減速量最大値</param>
	inline void SetJumpDecelMax(const float _decelMax) { jumpDecelMax_ = _decelMax; }

	/// <summary>
	/// ジャンプ時間
	/// </summary>
	/// <param name="_step"></param>
	inline void SetStepJump(const float _step) { stepJump_ = _step; }

	/// <summary>
	/// 方向
	/// </summary>
	/// <param name="_dir">方向</param>
	inline void SetDir(const VECTOR _dir) { dir_ = _dir; }

	/// <summary>
	/// パンチ吹っ飛び力
	/// </summary>
	/// <param name="_punchedCnt">パンチ吹っ飛び力</param>
	inline void SetKnockbackCnt(const float _punchedCnt) { punchedCnt_ = _punchedCnt; }

	/// <summary>
	/// ノックバック速度
	/// </summary>
	/// <param name="_KnockSpd">ノックバック速度</param>
	inline void SetKnockBackSpd(const float _KnockSpd) { knockBackSpd_ = _KnockSpd; }

	/// <summary>
	/// 再生しているリソースをすべて止める
	/// </summary>
	/// <param name="">再生しているリソースをすべて止める</param>
	void StopResource(void);

#ifdef _DEBUG
	//デバッグ描画
	void DrawDebug(void);
#endif // _DEBUG

private:

	//重力の割合
	static constexpr float GRAVITY_PER = 20.0f;
	// 回転完了までの時間
	static constexpr float TIME_ROT = 0.1f;

	//移動関連
	static constexpr float MOVE_SPEED = 6.0f;			//移動スピード
	static constexpr float DASH_SPEED = 11.0f;			//ダッシュスピード
	static constexpr float FLY_AWAY_SPEED = 12.0f;		//ぶっ飛ぶスピード
	static constexpr float LIMIT_GRAVITY = -20.0f;		//落ちているときの重力制限(jumpPowに加算しているのでjumpPowに適用)
	static constexpr float SLIME_FLOOR_MOVE_SPD = 3.0f;	//スライム床上での移動速度(通常)
	static constexpr float DASH_ANIM_SPEED = 200.0f;	//ダッシュアニメーションスピード
	static constexpr float DASH_SE_TIME = 0.2f;			//ダッシュSE間隔

	//ジャンプ関連
	static constexpr float TIME_JUMP_SCALE = 1.0f;				//ジャンプ加速の倍率
	static constexpr float SLIME_FLOOR_JUMP_POW = 10.0f;		//スライム床上でのジャンプ力
	static constexpr float POW_JUMP = 20.0f;					//ジャンプ力
	static constexpr float JUMP_ANIM_LOOP_START_FRAME = 23.0f;	//ジャンプアニメーションループ開始
	static constexpr float JUMP_ANIM_LOOP_END_FRAME = 25.0f;	//ジャンプアニメーションループ完了
	static constexpr float JUMP_ANIM_ATTACK_BLEND_TIME = 5.0f;	//ジャンプアニメーションループ中のスピード
	//ジャンプ開始と終了のアニメステップ
	static constexpr float JUMP_ANIM_START_FRAME = 10.0f;		//開始ステップ
	static constexpr float JUMP_ANIM_END_FRAME = 60.0f;			//終了ステップ

	//パンチ関連
	static constexpr int HAND_FRAME_NUM = 10;					//モデルの手のフレームＩＤ
	static constexpr float PUNCH_COOL_TIME = 0.5f;				//パンチクールタイム
	static constexpr float PUNCH_HIT_START_ANIM_STEP = 20.0f;	//パンチの当たり判定時間中フラグを始めるアニメーションステップ
	static constexpr float PUNCH_HIT_END_ANIM_STEP = 40.0f;		//パンチの当たり判定時間中フラグを終えるアニメーションステップ
	static constexpr float PUNCHED_TIME = 0.1f;					//吹き飛び効果時間
	static constexpr int KNOCKBACK_PAD_VIBRATIION_TIME = 300;	//ノックバックのパッド振動時間
	static constexpr int KNOCKBACK_PAD_VIBRATIION_POW = 300;	//ノックバックのパッド振動の強さ

	// シーンマネージャ参照
	SceneManager& scnMng_;
	//プレイヤー
	Player& player_;
	//アニメーションコントローラー
	AnimationController& animationController_;
	//状態遷移
	std::map<ACTION_TYPE, std::function<void(void)>>changeAction_;
	//状態更新
	std::function<void(void)>actionUpdate_;
	//操作入力
	std::unique_ptr<PlayerInput> input_;
	//エフェクト
	std::unique_ptr<EffectController> effect_;
	//状態
	ACTION_TYPE act_;
	//素材関連
	std::map<ACT_SE, SoundManager::SRC>actSE_;	//アクションごとのサウンド
	float dashSeCnt_;							//ダッシュSEのカウント

	//移動
	float speed_;			//移動スピード
	VECTOR moveDir_;		//移動方向
	VECTOR movePow_;		//移動量
	VECTOR dir_;			//方向
	float moveSpd_;			//通常移動量
	float dashSpd_;			//ダッシュ移動量

	//回転
	Quaternion playerRotY_;		//プレイヤーY角度
	Quaternion goalQuaRot_;		//目的の回転
	float stepRotTime_;			//補完時間

	//ジャンプ
	bool isJump_;				//ジャンプ判定
	float stepJump_;			//ジャンプの入力受付時間
	VECTOR jumpPow_;			//ジャンプ量
	float jumpDeceralation_;	//ジャンプ減衰量
	float jumpDecelMax_;		//ジャンプ減衰量最大値

	//パンチ
	bool isPunchHitTime_;		//パンチ当たり判定の時間フラグ
	float punchCnt_;				//パンチカウント
	float punchCoolCnt_;			//パンチクールタイム
	VECTOR punchPos_;			//攻撃座標
	float punchedCnt_;			//パンチ効果時間カウント
	VECTOR punchedPos_;			//パンチされた場所の記憶する
	float knockBackSpd_;		//ノックバック速度
	int cameraNo_;				//カメラ番号

	//更新系
	void UpdateNone(void);			//何もしない
	void UpdateActionInput(void);	//入力
	void UpdateMove(void);			//移動
	void UpdatePunch(void);			//パンチ
	void UpdateJump(void);			//ジャンプ
	void UpdateKnockBack(void);		//ノックバック
	//遷移系
	void ChangeNone(void);			//何もしない
	void ChangeActionInput(void);	//入力
	void ChangeMove(void);			//移動
	void ChangeDashMove(void);		//ダッシュ
	void ChangePunch(void);			//パンチ
	void ChangeKnockBack(void);		//ノックバック
	void ChangeJump(void);			//ジャンプ

	//入力方向に応じて方向を決める
	void MoveDirFronInput(void);
	//毎フレーム移動方向とスピードを更新する
	void UpdateMoveDirAndPow(void);
	//移動速度を決める
	void DesideSpeed(void);

	//ジャンプ処理
	void Jump(void);		
	//ジャンプができる条件
	bool CheckJumpInput(void);

	/// <summary>
	/// 再生させたいSE以外すべて止める
	/// </summary>
	/// <param name="_se">現在再生させたいSE</param>
	void StopSe(const ACT_SE _se);

	//回転
	void Rotate(void);
	/// <summary>
	///	回転させたい角度のセット
	/// </summary>
	/// <param name="_deg">回転させたい角度</param>
	void SetGoalRotate(double _deg);
};

