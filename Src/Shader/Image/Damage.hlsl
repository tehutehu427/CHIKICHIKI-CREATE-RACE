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
    float g_time;   // 画素の距離
    float g_sizeX;  // 画像サイズX
    float g_sizeY;  // 画像サイズY
}

//描画するテクスチャ
Texture2D g_SrcTexture : register(t0);

// サンプラー：適切な色を決める処理
SamplerState g_SrcSampler : register(s0);

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float2 uv = PSInput.TexCoords0;
    float4 color = g_SrcTexture.Sample(g_SrcSampler, uv);

    float2 centerDist = abs(uv - 0.5) * 2.0;
    float edgeValue = max(centerDist.x, centerDist.y);

    // グロー範囲と縁の太さを調整
    float edgeGlow = smoothstep(0.65, 0.85, edgeValue);

    // 時間によるゆらぎを加える
    float wave = sin(g_time * 3.0 + edgeValue * 20.0) * 0.05;
    edgeGlow += wave;

    edgeGlow = saturate(edgeGlow);

    // 赤く発光させる（強度も可変に）
    float glowStrength = 1.2 + sin(g_time) * 0.5;
    float4 glowColor = float4(edgeGlow * glowStrength, 0.0, 0.0, 1.0);

    return color + glowColor;
}