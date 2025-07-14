#pragma once

class VertexMaterial;

class VertexRenderer
{
public:

	// ピクセルシェーダ用オリジナル定数バッファの使用開始スロット
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_VS = 7;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	VertexRenderer(VertexMaterial& material);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~VertexRenderer();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(const int& modelId);

private:

	//モデルのマテリアル
	VertexMaterial& modelMaterial_;

};

