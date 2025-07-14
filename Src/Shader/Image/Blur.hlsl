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
    //float uv_dis;       //UVの距離
    float g_distance;   // 画素の距離
    float g_sizeX;      // 画像サイズX
    float g_sizeY;      // 画像サイズY
}

//描画するテクスチャ
Texture2D g_SrcTexture : register(t0);

// サンプラー：適切な色を決める処理
SamplerState g_SrcSampler : register(s0);

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float2 uv = PSInput.TexCoords0;
    float4 color = g_SrcTexture.Sample(g_SrcSampler, uv);   // 中心色
    float directions = 16.0f;                               // 方向数
    float quality = 3.0f;                                   // 各方向でのサンプル数
    float twoPi = 6.28318530718f;

    float2 radius = float2(g_distance / g_sizeX, g_distance / g_sizeY); // ブラー半径（UVスケール）

    // 全方向に向けてぼかす
    for (float d = 0.0f; d < twoPi; d += twoPi / directions)
    {
        float2 dir = float2(cos(d), sin(d)); // 単位方向ベクトル

        for (float i = 1.0f; i <= quality; i += 1.0f)
        {
            float2 offset = dir * radius * (i / quality); // ブラーの段階的距離
            color += g_SrcTexture.Sample(g_SrcSampler, uv + offset);
        }
    }

    // 平均化
    color /= (1.0f + directions * quality);

    return color;
}