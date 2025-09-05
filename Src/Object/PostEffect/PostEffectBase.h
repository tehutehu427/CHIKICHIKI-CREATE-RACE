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
	PostEffectBase() ;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~PostEffectBase() = default;

	/// <summary>
	/// 読み込み処理
	/// </summary>
	virtual void Load() = 0;

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw();

protected:

	//レンダラー
	std::unique_ptr<PixelRenderer> renderer_;

	//マテリアル
	std::unique_ptr<PixelMaterial> material_;

};
