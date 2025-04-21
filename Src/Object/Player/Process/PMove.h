#pragma once
class PMove
{
public:
	//**********************************************
	//定数
	//**********************************************
	//スピード
	static constexpr float SPEED_MOVE = 5.0f;
	static constexpr float SPEED_RUN = 10.0f;

	// 回転完了までの時間
	static constexpr float TIME_ROT = 1.0f;
	//**********************************************
	PMove(void);
	~PMove(void);
	void Init(void);
	void Update(const std::shared_ptr<Camera>& _camera,bool& _isJump,Transform& _trans);
	//回転
	void Rotate(void);
	//ゲッタ
	//-----------------------------------------
	//移動量
	const VECTOR GetMovePow(void) { return movePow_; }

	//Y軸の角度
	const Quaternion GetPlayerRotY(void) { return playerRotY_; }
	//-----------------------------------------
	//セッタ
	//-----------------------------------------
	 
	//-----------------------------------------

private:
	//**********************************************
	//メンバ変数
	//**********************************************
	// 移動スピード
	float speed_;
	// 移動方向
	VECTOR moveDir_;
	// 移動量
	VECTOR movePow_;

	//回転
	Quaternion playerRotY_;
	Quaternion goalQuaRot_;
	float stepRotTime_;


	//**********************************************
	//メンバ関数
	//**********************************************
	//開店後の角度取得
	void SetGoalRotate(double rotRad, const std::shared_ptr<Camera>& _camera);
	


	//**********************************************
};

