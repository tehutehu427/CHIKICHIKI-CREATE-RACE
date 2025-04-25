#pragma once
#include"../Object/Common/Transform.h"
#include"./PlayerInput.h"
class PlayerInput;
class PPunch
{
public:
	//定数
	//*************************************
	//パンチ有効時間
	static constexpr float PUNCH_TIME_MAX = 1.5f;
	PPunch(std::weak_ptr<PlayerInput>_input);
	~PPunch(void)=default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name=""></param>
	void Init(void);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="_pPos"></param>
	void Update(Transform& _pPos);

	/// <summary>
	/// デバッグ表示
	/// </summary>
	/// <param name=""></param>
	void DrawDebug(void);
private:


	//操作
	std::weak_ptr<PlayerInput>input_;

	//パンチ中フラグ
	bool isPunch_;

	//パンチカウント
	float cnt_;

	//攻撃座標
	VECTOR pos_;



};

