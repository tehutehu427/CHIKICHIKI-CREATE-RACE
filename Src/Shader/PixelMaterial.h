#pragma once
#include <vector>
#include <string>

class PixelMaterial
{
public:

	/// <summary>
	/// テクスチャアドレス
	/// </summary>
	enum class TEXADDRESS
	{
		WRAP,
		MIRROR,
		CLAMP,
		BORDER,
		MAX
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="shaderFileName"></param>
	/// <param name="constBufFloat4Size"></param>
	PixelMaterial(std::string shaderFileName, int constBufFloat4Size);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PixelMaterial() = default;

	/// <summary>
	/// テクスチャを設定する
	/// </summary>
	/// <param name="texture"></param>テクスチャ
	void SetTexture(int texture);

	/// <summary>
	/// テクスチャアドレスを設定
	/// </summary>
	/// <param name="address"></param>テクスチャアドレス
	void SetTextureAdrdress(const TEXADDRESS address);

	/// <summary>
	///　シェーダを適用させる
	/// </summary>
	void ShaderApply();

	/// <summary>
	/// シェーダーの解除
	/// </summary>
	void ShaderUnapply();

	/// <summary>
	/// バッファーを適用
	/// </summary>
	void ConstBufApply();

	/// <summary>
	/// バッファーを解除
	/// </summary>
	void ConstBufUnapply();

	/// <summary>
	/// テクスチャを適用
	/// </summary>
	void TexuresApply();

	/// <summary>
	/// テクスチャを解除
	/// </summary>
	void TexuresUnapply();

	/// <summary>
	/// テクスチャのアドレスモードを適用
	/// </summary>
	void TextureAddressModeApply();

	//シェーダを渡す
	inline const int GetShader()const { return shader_; }

	//バッファーを返す
	inline const int GetBuffer()const { return constBuf_; }

private:

	// シェーダハンドル
	int shader_;

	// 定数バッファの確保サイズ(FLOAT4をいくつ作るか)
	int constBufFloat4Size_;

	// 定数バッファハンドル
	int constBuf_;

	// テクスチャアドレス
	TEXADDRESS texAddress_;

	// 定数バッファ
	std::vector<FLOAT4> constBufs_;

	// 画像
	std::vector<int> textures_;
};

