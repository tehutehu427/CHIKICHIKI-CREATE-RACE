#pragma once
#include"../Object/Common/Transform.h"
#include"./PlayerInput.h"
class PlayerInput;
class PPunch
{
public:
	//定数
	//*************************************
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





};

