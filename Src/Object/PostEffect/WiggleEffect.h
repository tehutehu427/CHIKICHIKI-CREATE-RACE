#pragma once
#include "../PostEffect/PostEffectBase.h"

class WiggleEffect : public PostEffectBase
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	WiggleEffect(void);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~WiggleEffect(void) override = default;

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load(void)override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init(void)override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(void)override;

private:

	//バッファーサイズ
	static constexpr int BUFFER_SIZE = 1;

	//タイマー
	float timer_;

};

