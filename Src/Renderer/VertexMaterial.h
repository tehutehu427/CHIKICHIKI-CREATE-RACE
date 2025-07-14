#pragma once
#include <string>
#include <vector>
#include <DxLib.h>

class VertexMaterial
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="shaderFileName">シェーダーファイル名</param>
	/// <param name="constBufFloat4Size">定数バッファの確保サイズ(FLOAT4をいくつ作るか)</param>
	VertexMaterial(std::string shaderFileName, int constBufFloat4Size);

	// デストラクタ
	~VertexMaterial(void);

	// 定数バッファを追加
	void AddConstBuf(const FLOAT4& contBuf);

	// 定数バッファを更新
	void SetConstBuf(int idx, const FLOAT4& contBuf);

	// シェーダ
	int GetShader(void) const;

	// 定数バッファハンドル
	int GetConstBuf(void) const;

	// 定数バッファ
	const std::vector<FLOAT4>& GetConstBufs(void) const;

private:

	// シェーダ
	int shader_;

	// 定数バッファの確保サイズ(FLOAT4をいくつ作るか)
	int constBufFloat4Size_;

	// 定数バッファハンドル
	int constBuf_;

	// 定数バッファ
	std::vector<FLOAT4> constBufs_;
};

