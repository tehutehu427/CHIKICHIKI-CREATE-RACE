//VS・PS共通
#include "../Common/VertexToPixelHeader.hlsli"

//IN
#define PS_INPUT VertexToPixelLit

//PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

// 定数バッファ（例）
cbuffer cbParam : register(b4)
{
    //カラー
    float4 g_color;
}

float4 main(PS_INPUT PSInput) : SV_TARGET0
{
    float4 texColor = diffuseMapTexture.Sample(diffuseMapSampler, PSInput.uv);
    if (texColor.a < 0.01f)
    {
        discard;
    }
    return texColor * g_color;
}
