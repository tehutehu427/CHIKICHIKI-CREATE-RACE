#pragma once
#include "PostEffectBase.h"

class PostEffectScanLine : public PostEffectBase
{
public:

	//float4を生成する数
	static constexpr int CONST_BUFF_FLOAT4_SIZE = 8;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PostEffectScanLine();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PostEffectScanLine() override;

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

