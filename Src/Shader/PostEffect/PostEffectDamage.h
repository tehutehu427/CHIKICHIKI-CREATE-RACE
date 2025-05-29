#pragma once
#include "PostEffectBase.h"

class PostEffectDamage : public PostEffectBase
{
public:

	//float4を生成する数
	static constexpr int CONST_BUFF_FLOAT4_SIZE = 8;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PostEffectDamage();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PostEffectDamage() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;

private:

	/// <summary>
	/// バッファーの更新処理
	/// </summary>
	void UpdateConstBuffer() override;
};

