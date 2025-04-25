#pragma once
class GravityManager;
class PJump
{
public:
	PJump(std::weak_ptr<PlayerInput>_input);
	~PJump(void) = default;


	void Init(void);
	void Update(const VECTOR& _dirUp,const VECTOR& _dirDown,const bool& _isEndLanding);
	void DrawDebug(void);

	////ゲッタ
	////--------------------------------------
	//const bool GetIsJump(void)const  { return isJump_; }
	//const VECTOR GetJumpPow(void)const { return jumpPow_; }
	//
	////--------------------------------------
	//
	////セッタ
	////--------------------------------------
	//void SetIsJump(const bool _isJump) { isJump_ = _isJump; }
	//void SetStepJump(const float _step) { stepJump_ = _step; }


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
	//操作
	std::weak_ptr<PlayerInput>input_;





	//オブジェクトの上方向
	VECTOR dirUp_;
	//オブジェクトの下方向
	VECTOR dirDown_;

	//着地モーションかどうか
	bool isEndLanding_;
};

