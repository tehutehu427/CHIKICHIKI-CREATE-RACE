#pragma once
#include"../Object/Common/Transform.h"
class PPunch
{
public:
	//’è”
	//*************************************
	//ƒpƒ“ƒ`—LŒøŠÔ
	static constexpr float PUNCH_TIME_MAX = 1.5f;
	PPunch(void);
	~PPunch(void)=default;

	void Init(void);
	void Update(Transform& _pPos);
	void DrawDebug(void);
private:
	bool isPunch_;
	float cnt_;
	VECTOR pos_;

};

