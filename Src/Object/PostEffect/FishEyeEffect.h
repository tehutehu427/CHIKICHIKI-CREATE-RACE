#pragma once
#include "PostEffectBase.h"

class FishEyeEffect : public PostEffectBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	FishEyeEffect();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~FishEyeEffect() override;

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load()override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init()override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw()override;

private:

	//バッファーサイズ
	static constexpr int BUFFER_SIZE = 1;

	//画面分割
	float screenDivX_;
	float screenDivY_;

};

