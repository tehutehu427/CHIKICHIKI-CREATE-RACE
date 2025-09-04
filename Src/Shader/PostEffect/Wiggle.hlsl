#include "../common/Pixel/PixelShader2DHeader.hlsli"

cbuffer cbColor : register(b0)
{
    //カラー
    float4 g_color; 
    
    //生存判定(1 = 生存, 0 = 沈黙)
    float4 g_aliveFlags;
    
    //タイマー
    float g_time;
    
    //画面分割(縦,横)
    float2 g_screenDiv;
    
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float2 uv = PSInput.uv;

    // どの画面に属するかを判定
    int px = (int) (uv.x * g_screenDiv.x);
    int py = (int) (uv.y * g_screenDiv.y);
    int playerIndex = py * 2 + px;

    // 生死フラグでエフェクトを切り替え
    if (g_aliveFlags[playerIndex] == 1)
    {
        // サイン波でくねくね
        float waveX = sin(uv.y * 20.0 + g_time * 2.0) * 0.01;
        float waveY = cos(uv.x * 15.0 + g_time * 2.0) * 0.01;
        uv.x += waveX;
        uv.y += waveY;
    }

    // テクスチャの色を取得
    return tex.Sample(texSampler, uv);
}