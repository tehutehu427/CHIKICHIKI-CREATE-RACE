#pragma once
#include "PostEffectBase.h"

class PostEffectMonotone : public PostEffectBase
{
public:

	//float4を生成する数
	static constexpr int CONST_BUFF_FLOAT4_SIZE = 4;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PostEffectMonotone();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PostEffectMonotone() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init()override;

};

