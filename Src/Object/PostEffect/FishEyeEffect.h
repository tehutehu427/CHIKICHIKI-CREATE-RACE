#pragma once
#include "PostEffectBase.h"

class FishEyeEffect : public PostEffectBase
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	FishEyeEffect(void);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~FishEyeEffect(void) override = default;

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load(void) override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init(void) override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(void) override;

private:

	//バッファーサイズ
	static constexpr int BUFFER_SIZE = 1;

	//画面分割
	float screenDivX_;
	float screenDivY_;
};

