#pragma once
#include <memory>

class PixelMaterial;
class PixelRenderer;

class PostEffectBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PostEffectBase();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~PostEffectBase();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

protected:

	//ポストエフェクトスクリーン
	int postEffectScreen_;

	//マテリアル
	std::unique_ptr<PixelMaterial> material_;

	//レンダー
	std::unique_ptr<PixelRenderer> render_;

	//バッファーの更新処理
	virtual void UpdateConstBuffer();

};

