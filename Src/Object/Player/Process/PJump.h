#pragma once
class GravityManager;
class PJump
{
public:
	PJump(void);
	~PJump(void) = default;

	//ジャンプ力
	static constexpr float POW_JUMP = 16.0f;

	//ジャンプ受付時間
	static constexpr float TIME_JUMP_IN = 0.5f;

	void Init(void);
	void Update(const VECTOR& _dirUp,const VECTOR& _dirDown,const bool& _isEndLanding);
	void DrawDebug(void);

	//ゲッタ
	//--------------------------------------
	const bool GetIsJump(void)const  { return isJump_; }
	const VECTOR GetJumpPow(void)const { return jumpPow_; }
	
	//--------------------------------------
	
	//セッタ
	//--------------------------------------
	void SetIsJump(const bool _isJump) { isJump_ = _isJump; }
	void SetStepJump(const float _step) { stepJump_ = _step; }


	//--------------------------------------
private:
	//**************************************
	//メンバ関数
	//**************************************
	//ジャンプ
	void Jump(void);

	//重力の計算
	void CalcGravity(void);
	//**************************************
	//メンバ変数
	//**************************************
	// ジャンプ判定
	bool isJump_;
	// ジャンプの入力受付時間
	float stepJump_;
	// ジャンプ量
	VECTOR jumpPow_;

	//ジャンプ減衰量
	float jumpDeceralation_;

	//落下カウント
	float fallCnt_;

	//オブジェクトの上方向
	VECTOR dirUp_;
	//オブジェクトの下方向
	VECTOR dirDown_;

	//着地モーションかどうか
	bool isEndLanding_;
};

