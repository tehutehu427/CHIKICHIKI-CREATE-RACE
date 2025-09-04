#include "../common/Pixel/PixelShader2DHeader.hlsli"

cbuffer cbColor : register(b4)
{
    //タイマー
    float g_time;
    float3 dummy;
    
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float2 uv = PSInput.uv;

    // サイン波で画面全体をくねくねさせる
    float waveX = sin(uv.y * 20.0 + g_time * 2.0) * 0.02;
    float waveY = cos(uv.x * 15.0 + g_time * 2.0) * 0.02;
    uv.x += waveX;
    uv.y += waveY;

    // テクスチャの色を取得
    return tex.Sample(texSampler, uv);
}