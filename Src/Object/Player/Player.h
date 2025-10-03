#pragma once
#include "./PlayerInput.h"	//こいつを消したい
#include "../Manager/System/KeyConfig.h"
#include "../ObjectBase.h"

//#define DEBUG_ON
class AnimationController;
class Camera;
class PlayerAction;
class PlayerOnHit;
class DateBank;
class Shadow;
class Camera;
class ModelMaterial;
class ModelRenderer;
class EffectController;

class Player :public ObjectBase
{

public:

	//半径
	static constexpr float RADIUS = 25.0f;
	//デフォルトのアニメーションスピード
	static constexpr float DEFAULT_ANIM_SPD = 60.0f;
	//強化後のパンチされた時のノックバックカウント
	static constexpr float BUFF_KNOCKBACK_CNT_MAX = 0.3f;
	//強化後のパンチ吹っ飛びスピード
	static constexpr float BUFF_KNOCKBACK_SPEED_MAX = 20.0f;
	//強化後のジャンプ力
	static constexpr float BUFF_JUMP_POW_MAX = 40.0f;
	//強化後の移動スピード
	static constexpr float BUFF_MOVE_SPEED = 20.0f;
	//強化後のダッシュスピード
	static constexpr float BUFF_DASH_SPEED = 30.0f;
	//残機
	static constexpr float RESPAWN_CNT = 1;
	//接地しているときのラインのコライダ
	static constexpr int UP_AND_DOWN_LINE_COL_NO = 0;
	//プレイヤーの体の球
	static constexpr int BODY_SPHERE_COL_NO = 1;
	//現在の座標と移動後座標を結んだ線のコライダ
	static constexpr int MOVE_LINE_COL_NO = 2;
	//プレイヤーの手の座標
	static constexpr int HAND_SPHERE_COL_NO = 4;

	//プレイヤーの生存状態
	enum class PLAYER_STATE
	{
		ALIVE		//生存
		,DEATH		//死亡
		,GOAL		//ゴール
	};

	// アニメーション種別
	enum class ANIM_TYPE
	{
		NONE = 0,			//何もしない
		IDLE = 1,			//停止
		WALK = 2,			//歩き
		FALL = 4,			//落ちている
		HAND_UP = 5,		//手を挙げている
		HAND_WAVE = 6,		//手を振る
		PUNCH = 12,			//パンチ
		JUMP = 13,			//ジャンプ
		LAND=14,			//着地
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_playerNum">プレイヤー番号</param>
	/// <param name="_cntl">コントローラー識別番号</param>
	/// <param name="_tag">プレイヤーごとのタグ</param>
	Player(int _playerNum,KeyConfig::TYPE _cntl, const Collider::TAG _tag);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player(void);

	/// <summary>
	/// 読み込み
	/// </summary>
	/// <param name=""></param>
	void Load(void)override;
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name=""></param>
	void Init(void)override;
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
	/// 当たり判定後の処理
	/// </summary>
	/// <param name="_hitColTag">相手側の当たり判定</param>
	void OnHit(const std::weak_ptr<Collider> _hitCol)override;

	/// <summary>
	/// プレイヤー番号取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>プレイヤー番号</returns>
	inline const int GetPlayerNum(void)const { return playerNum_; }

	/// <summary>
	/// 使用するコントローラーの取得
	/// </summary>
	/// <param name="">使用するコントローラー</param>
	/// <returns></returns>
	inline const KeyConfig::TYPE GetCntl(void)const { return cntl_; }

	/// <summary>
	/// コントローラー番号の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>コントローラー番号</returns>
	inline const KeyConfig::JOYPAD_NO GetPadNum(void)const { return padNum_; }

	/// <summary>
	/// ゴール判定の取得(ディレイ時間も含めて)
	/// </summary>
	/// <param name=""></param>
	/// <returns>ゴール時間</returns>
	inline const bool IsGoal(void)const{ return state_ == PLAYER_STATE::GOAL && finishDelay_ >= GOAL_DELAY; }

	/// <summary>
	/// コールした瞬間(ディレイ時間なし)
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	inline const bool GetIsGoalMoment(void)const { return state_ == PLAYER_STATE::GOAL; }

	/// <summary>
	/// 死んだ判定(ディレイ時間も含めて)
	/// </summary>
	/// <param name=""></param>
	/// <returns>死んだ判定</returns>
	inline bool IsDeath(void)const{ return state_ == PLAYER_STATE::DEATH && finishDelay_ >= DEATH_DELAY; }

	//死んだ瞬間(ディレイ時間なし)
	inline const bool GetIsDeathMoment(void)const { return state_ == PLAYER_STATE::DEATH; }
	/// <summary>
	/// ゴールタイムの取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>ゴールタイム</returns>
	inline const float GetGoalTime(void)const { return goalTime_; }

	/// <summary>
	/// プレイヤー残機の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>プレイヤー残機数</returns>
	inline const int GetLiveCnt(void)const { return respawnCnt_; }

	/// <summary>
	/// 地面に当たっているか
	/// </summary>
	/// <param name=""></param>
	/// <returns>地面に当たっている</returns>
	const bool GetIsLandHit(void)const;
	/// <summary>
	/// スライム床と当たっているかを当たり判定押し出しクラスから取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>スライム床と当たっているかの判定</returns>
	const bool GetIsSlimeFloor(void)const;

	/// <summary>
	/// コイン枚数の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>コイン枚数</returns>
	const int GetCoinNum(void)const;

	/// <summary>
	/// ばねジャンプ力を当たり判定押し出しクラスから取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>バネジャンプ力</returns>
	const float GetSpringJumpPow(void);

	/// <summary>
	/// 座標のセット
	/// </summary>
	/// <param name="_worldPos">ワールド座標</param>
	void SetPos(const VECTOR _worldPos);

	/// <summary>
	/// 残機のセット
	/// </summary>
	/// <param name="_respawnCnt">リスポーン時間</param>
	void SetRespawnCnt(const int _respawnCnt) { respawnCnt_ = _respawnCnt; }

	/// <summary>
	/// //ジャンプ減衰量(ジャンプ力の設定)
	/// </summary>
	/// <param name="_jumpDecel">ジャンプ力</param>
	void SetJumpDecelMax(const float _jumpDecel=20.0f);

	/// <summary>
	/// パンチで吹っ飛ぶ時間
	/// </summary>
	/// <param name="_punchCnt">吹っ飛び力</param>
	void SetPunchPow(const float _knockBackCnt=0.1f, const float _knockBackSpd= 12.0f);

	/// <summary>
	/// 移動スピード
	/// </summary>
	/// <param name="_spd">スピード</param>
	void SetSpeed(const float _moveSpd= 6.0f,const float _dashSpd= 6.0f);

	/// <summary>
	/// リスポーン座標
	/// </summary>
	/// <param name="_pos"></param>
	void SetRespawnPos(const VECTOR _pos) { respawnPos_ = _pos; }

	/// <summary>
	/// モデル色を変更
	/// </summary>
	/// <param name="_colorScale">モデルの色</param>
	void ChangeModelColor(const COLOR_F _colorScale)override;
	
	/// <summary>
	/// パンチの当たり判定を作る
	/// </summary>
	/// <param name=""></param>
	void MakePunchCol(void);

	/// <summary>
	/// パンチの当たり判定を消す
	/// </summary>
	/// <param name=""></param>
	void KillPunchCol(void);

	/// <summary>
	/// プレイヤーのステータスを上げる
	/// </summary>
	/// <param name=""></param>
	void BuffPlayer(void);

private:

	//重力の割合
	static constexpr float GRAVITY_PER = 20.0f;
	//プレイヤー１のX座標
	static constexpr float PLAYER_ONE_POS_X = -20.0f;
	//座標の間隔
	static constexpr float DISTANCE_POS = 50.0f;
	//プレイヤーのローカル角度
	static constexpr float MODEL_LOCAL_DEG = 180.0f;
	//落ちるアニメーションのスタート
	static constexpr float FALL_ANIM_START = 32.0f;
	//落ちるアニメーションの終わり
	static constexpr float FALL_ANIM_END = 59.0f;
	//死ぬ判定の座標の基準
	static constexpr float DEATH_POS_Y = -600.0f;
	//死んだときのパッド振動時間
	static constexpr int DEATH_PAD_VIBRATION_TIME = 300;
	//死んだときのパッド振動の強さ
	static constexpr int DEATH_PAD_VIBRATION_POW = 300;
	//プレイヤーの大きさ
	static constexpr VECTOR MODEL_SCL = { 1.0f,1.0f,1.0f };
	//パンチの範囲
	static constexpr float PUNCH_RADIUS = 50.0f;
	//当たり判定のラインの長さ
	static constexpr float LINE_RANGE = 10.0f;
	//移動量ラインオフセット
	static constexpr float MOVE_LINE_Y_OFFSET = Player::RADIUS - 1.0f;
	//移動量更新条件の移動ラインの長さ
	static constexpr float MOVE_LINE_Y_CHECK_VALUE = Player::RADIUS + 1.5f;
	//プレイヤーの当たり判定ラインの上端と下端
	static constexpr VECTOR LOCAL_UP_POS = { 0.0f,RADIUS+ LINE_RANGE,0.0f };		//上端
	static constexpr VECTOR LOCAL_DOWN_POS = { 0.0f,-RADIUS- LINE_RANGE,0.0f };		//下端
	//ゲームオーバーの待機時間
	static constexpr float DEATH_DELAY = 2.0f;
	//ゴール時の待機時間
	static constexpr float GOAL_DELAY = 1.0f;

	//入力デバイス
	KeyConfig::TYPE cntl_;
	//ゲームパッド番号
	KeyConfig::JOYPAD_NO padNum_;

	//オブジェクト関連
	std::unique_ptr<PlayerAction>action_;	//行動系
	std::unique_ptr<PlayerOnHit>onHitCol_;	//当たった時の処理クラス

	//アウトライン
	std::unique_ptr<ModelMaterial> material_;	//マテリアル
	std::unique_ptr<ModelRenderer> renderer_;	//レンダラ

	//影
	std::unique_ptr<Shadow>shadow_;
	// アニメーション
	std::unique_ptr<AnimationController> animationController_;
	//エフェクト
	std::unique_ptr<EffectController> effect_;
	//カメラ
	std::weak_ptr<Camera>camera_;
	//プレイヤー番号
	int playerNum_;
	//計測用
	float time_;	
	//ゴール時間格納
	float goalTime_;
	//残機
	int respawnCnt_;
	//プレイヤーの状態(生存状態)
	PLAYER_STATE state_;	
	//プレイヤーの状態遷移
	std::map<PLAYER_STATE, std::function<void(void)>>changeStates_;
	//状態更新
	std::function<void(void)>stateUpdate_;
	//ゲーム終了時の待機時間
	float finishDelay_;
	//プレイヤーの当たり判定タグ
	Collider::TAG tag_;	
	//リスポーン位置
	VECTOR respawnPos_;	
	//移動量
	VECTOR movedPos_;
	//移動前
	VECTOR moveDiff_;

#ifdef DEBUG_ON
	void DrawDebug(void);
#endif // DEBUG_ON
	/// 状態遷移
	void ChangeState(PLAYER_STATE _state);
	//座標関連の更新
	void UpdatePost(void);

	//更新系
	void UpdateAlive(void);		//生存
	void UpdateDeath(void);		//死亡
	void UpdateGoal(void);		//ゴール

	//遷移系
	void ChangeAlive(void);		//生存
	void ChangeDeath(void);		//死亡
	void ChangeGoal(void);		//ゴール

	//アクション関係
	void Action(void);
	//タイム更新
	void TimeUpdate(void);
	//リスポーン処理
	void Respawn(void);
};

