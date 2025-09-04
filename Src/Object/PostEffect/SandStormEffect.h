#pragma once
#include "PostEffectBase.h"

class SandStormEffect : public PostEffectBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SandStormEffect();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SandStormEffect() override;

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

	//タイマー
	float timer_;

};


