#include "../common/Pixel/PixelShader2DHeader.hlsli"

cbuffer cbColor : register(b0)
{
    // タイマー
    float g_time;

    // 画面分割 (横, 縦) → 今回は未使用でもOK
    float2 g_screenDiv;

    float dummy;
}

// 擬似乱数関数（ハッシュベース）
float hash21(float2 p)
{
    p = frac(p * float2(123.34, 345.45));
    p += dot(p, p + 34.345);
    return frac(p.x * p.y);
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float2 uv = PSInput.uv;

    // 元の色
    float4 col = tex.Sample(texSampler, uv);

    // ノイズ生成（時間をシードに入れて毎フレーム変化させる）
    float2 noiseUV = uv * 600.0 + float2(g_time * 50.0, g_time * 123.0);
    float noise = hash21(noiseUV);

    // ノイズを適用（強度を小さく）
    col.rgb += (noise - 0.5) * 0.2;

    // ほんのりオレンジを混ぜる
    float3 orangeTint = float3(1.0, 0.6, 0.3);
    col.rgb = lerp(col.rgb, orangeTint, 0.1);

    return col;
}