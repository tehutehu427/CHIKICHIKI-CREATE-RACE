//VS・PS共通
#include "../Common/VertexToPixelHeader.hlsli"

//IN
#define PS_INPUT VertexToPixelLit

//PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

// トゥーンの明るさ（高）
static const float TOON_HIGH = 0.66f;

// トゥーンの明るさ（中）
static const float TOON_MID = 0.33f;

// トゥーンシェード（明るい）
static const float SHADE_HIGH = 1.0f;

// トゥーンシェード（中間）
static const float SHADE_MID = 0.6f;

// トゥーンシェード（暗い）
static const float SHADE_LOW = 0.3f;

// 定数バッファ（例）
cbuffer cbParam : register(b4)
{
    //カラー
    float4 g_color;
    
    // 光の色
    float4 g_light_color;
    
    // 影の色
    float4 g_shadow_color;
    
    //環境光
    float4 g_ambient_color;
    
    // 光の方向（ワールド空間）
    float3 g_light_dir;
}

float4 main(PS_INPUT PSInput) : SV_TARGET0
{
    float4 texColor = diffuseMapTexture.Sample(diffuseMapSampler, PSInput.uv);
    

    if (texColor.a < 0.01f)
    {
        discard;
    }

    float3 normal = normalize(PSInput.normal);
    float3 lightDir = normalize(g_light_dir);

    // ====================
    // トゥーンライティング
    // ====================
    float diff = saturate(dot(normal, -lightDir)); // 光の当たり具合
    float shade;

    // 3段階のトゥーン
    if (diff > TOON_HIGH)
    {
        shade = SHADE_HIGH;
    }
    else
    {
        if (diff > TOON_MID)
        {
            shade = SHADE_MID;
        }
        else
        {
            shade = SHADE_LOW;
        }
    }
    float3 color = lerp(g_shadow_color.rgb, g_light_color.rgb, shade) * texColor.rgb * g_color.rgb + g_ambient_color.rgb;

    return float4(color, texColor.a * g_color.a);
}
