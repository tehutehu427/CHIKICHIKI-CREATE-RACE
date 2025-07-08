// VS・PS共通
#include "../Common/VertexToPixelHeader.hlsli"

// IN
#define PS_INPUT VertexToPixelLit

// PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

// =========================
// テクスチャ & サンプラ設定
// =========================

// テクスチャ配列（最大4枚を想定）
Texture2D g_Texture : register(t0);
SamplerState g_Sampler : register(s0);

// =========================
// 定数バッファ
// =========================
cbuffer cbParam : register(b4)
{
    float4 g_color; // カラー
    
    float4 g_light_color; // 光の色
    
    float4 g_shadow_color; // 影の色
    
    float4 g_ambient_color; // 環境光
    
    float3 g_light_dir; // 光の方向（ワールド空間）
    
    float g_textures_index;
}

// =========================
// メイン関数
// =========================
float4 main(PS_INPUT PSInput) : SV_TARGET0
{   
    // テクスチャカラーを取得（t0 にバインドされたテクスチャ）
    float4 texColor = g_Texture.Sample(g_Sampler, PSInput.uv);
    
    if(g_textures_index < 0.0f)
    {
        texColor = PSInput.diffuse;
    }
   
    // アルファテスト（透過カットオフ）
    if (texColor.a < 0.01f)
    {
        discard;
    }

    // 法線と光方向を正規化
    float3 normal = normalize(PSInput.normal);
    float3 lightDir = normalize(g_light_dir);

    // トゥーンライティング（3段階）
    float diff = saturate(dot(normal, -lightDir));
    float shade;
    if (diff > 0.66f)
    {
        shade = 1.0f;
    }
    else if (diff > 0.33f)
    {
        shade = 0.6f;
    }
    else
    {
        shade = 0.3f;
    }

    // 陰影・色計算
    float3 color = lerp(g_shadow_color.rgb, g_light_color.rgb, shade)
                 * texColor.rgb * g_color.rgb + g_ambient_color.rgb;

    // カラー出力
    return float4(color, texColor.a * g_color.a);
}