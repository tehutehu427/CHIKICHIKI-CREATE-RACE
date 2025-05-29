//ピクセルシェーダの入力
struct PS_INPUT
{
	//座標(プロダクション空間)
    float4 Position : SV_POSITION;

	//ディフーズカラー
    float4 Diffuse : COLOR0; //末尾はゼロ

	//テクスチャ座標
    float2 TexCoords0 : TEXCOORDS0; //末尾はゼロ
};
    
cbuffer cbColor : register(b0)
{
    float4 g_color;
}

//描画するテクスチャ
Texture2D g_SrcTexture : register(t0);

// サンプラー：適切な色を決める処理
SamplerState g_SrcSampler : register(s0);

float4 main(PS_INPUT PV_INPUT) : SV_TARGET
{
    float2 uv = PV_INPUT.TexCoords0;
    float4 baseColor = g_SrcTexture.Sample(g_SrcSampler, uv);

    // ブラー設定
    float directions = 16.0f;
    float quality = 3.0f;
    float2 radius = float2(0.1f, 0.1f);
    float twoPi = 6.2831853f;

    float4 glowColor = float4(0, 0, 0, 0);

    // 放射ブラー合成
    for (float d = 0.0f; d < twoPi; d += twoPi / directions)
    {
        float2 dir = float2(cos(d), sin(d));
        for (float i = 1.0f; i <= quality; i += 1.0f)
        {
            float2 offset = dir * radius * (i / quality);
            glowColor += g_SrcTexture.Sample(g_SrcSampler, uv + offset);
        }
    }

    // 平均化
    glowColor /= (directions * quality);

    // ===== 色合成 =====
    // 赤く光らせる（C++から渡された g_color を使って）
    return (baseColor + glowColor) * g_color;
}