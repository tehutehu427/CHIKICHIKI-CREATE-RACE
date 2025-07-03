#include "VertexMaterial.h"
#include "../Application.h"

VertexMaterial::VertexMaterial(std::string shaderFileName, int constBufFloat4Size)
{
	// 頂点シェーダのロード
	shader_ = LoadVertexShader((Application::PATH_SHADER + shaderFileName).c_str());

	// 頂点定数バッファの確保サイズ(FLOAT4をいくつ作るか)
	constBufFloat4Size_ = constBufFloat4Size_;

	// 頂点シェーダー用の定数バッファを作成
	constBuf_ = CreateShaderConstantBuffer(sizeof(FLOAT4) * constBufFloat4Size_);

}

VertexMaterial::~VertexMaterial(void)
{
	DeleteShader(shader_);
	DeleteShaderConstantBuffer(constBuf_);
}

void VertexMaterial::AddConstBuf(const FLOAT4& contBuf)
{
	if (constBufFloat4Size_ > constBufs_.size())
	{
		constBufs_.emplace_back(contBuf);
	}
}

void VertexMaterial::SetConstBuf(int idx, const FLOAT4& contBuf)
{
	if (idx >= constBufs_.size())
	{
		return;
	}

	constBufs_[idx] = contBuf;
}

int VertexMaterial::GetShader(void) const
{
	return shader_;
}

int VertexMaterial::GetConstBuf(void) const
{
	return constBuf_;
}

const std::vector<FLOAT4>& VertexMaterial::GetConstBufs(void) const
{
	return constBufs_;
}
