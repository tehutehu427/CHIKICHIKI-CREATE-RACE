#include "VertexRenderer.h"
#include<DxLib.h>
#include "VertexMaterial.h"

VertexRenderer::VertexRenderer(VertexMaterial& material):
	modelMaterial_(material)
{
}

VertexRenderer::~VertexRenderer()
{
}

void VertexRenderer::Draw(const int& modelId)
{
	// オリジナルシェーダ設定(ON)
	MV1SetUseOrigShader(true);

	// ピクセルシェーダ設定
	SetUsePixelShader(modelMaterial_.GetShader());

	size_t size;

	// 定数バッファハンドル
	int constBuf = modelMaterial_.GetConstBuf();

	FLOAT4* constBufsPtr = (FLOAT4*)GetBufferShaderConstantBuffer(constBuf);
	const auto& constBufs = modelMaterial_.GetConstBufs();

	size = constBufs.size();
	for (int i = 0; i < size; i++)
	{
		if (i != 0)
		{
			constBufsPtr++;
		}
		constBufsPtr->x = constBufs[i].x;
		constBufsPtr->y = constBufs[i].y;
		constBufsPtr->z = constBufs[i].z;
		constBufsPtr->w = constBufs[i].w;
	}

	// 定数バッファの設定
	UpdateShaderConstantBuffer(constBuf);

	// 頂点シェーダー用の定数バッファを定数バッファレジスタにセット
	SetShaderConstantBuffer(constBuf, DX_SHADERTYPE_VERTEX, CONSTANT_BUF_SLOT_BEGIN_VS);

	// 頂点シェーダー設定
	SetUseVertexShader(modelMaterial_.GetShader());

	// 描画
	MV1DrawModel(modelId);

	// オリジナルシェーダ設定(OFF)
	MV1SetUseOrigShader(false);
}
