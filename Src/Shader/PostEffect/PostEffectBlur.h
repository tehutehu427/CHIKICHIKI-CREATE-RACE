#pragma once
#include "PostEffectBase.h"

class PostEffectBlur : public PostEffectBase
{
public:

	//ブラーを掛ける最大距離数
	static constexpr float MAX_BLUR_DISTANCE = 10.0f;

	//ブラー速度
	static constexpr float BLUR_SPEED = 0.1f;

	//float4を生成する数
	static constexpr int CONST_BUFF_FLOAT4_SIZE = 8;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PostEffectBlur();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PostEffectBlur() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;

private:

	//ブラー用ステップ
	float blurStep_;

	//反転用
	float invertStep_;

	/// <summary>
	/// バッファーの更新処理
	/// </summary>
	void UpdateConstBuffer() override;

};

