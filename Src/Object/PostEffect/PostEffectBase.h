#pragma once
#include <memory>

class PixelRenderer;
class PixelMaterial;

class PostEffectBase
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PostEffectBase(void) ;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~PostEffectBase(void) = default;

	/// <summary>
	/// 読み込み処理
	/// </summary>
	virtual void Load(void) = 0;

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Init(void) = 0;

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw(void);

protected:

	//レンダラー
	std::unique_ptr<PixelRenderer> renderer_;

	//マテリアル
	std::unique_ptr<PixelMaterial> material_;
};
