#include "../common/Pixel/PixelShader2DHeader.hlsli"

cbuffer cbColor : register(b0)
{
    float4 g_color;

    // タイマー
    float g_time;

    // 画面分割 (横, 縦) → 今回は全画面処理なので未使用でもOK
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

    // ノイズ生成
    float noise = hash21(uv * 500.0 + g_time * float2(1.5, 0.2));

    // 砂嵐の風効果 → 横にスクロールさせる
    float wave = sin(uv.y * 50.0 + g_time * 10.0) * 0.02;
    float2 uvDistort = uv;
    uvDistort.x += wave;

    // 歪んだUVで再サンプリング
    float4 distortedCol = tex.Sample(texSampler, uvDistort);

    // ノイズを加算ブレンド（強度 0.3）
    col = lerp(col, distortedCol, 0.5);
    col.rgb += (noise - 0.5) * 0.3;

    return col;
}