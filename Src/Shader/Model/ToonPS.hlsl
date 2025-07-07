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
    
    // 光の色
    float4 g_light_color;
    
    // 影の色
    float4 g_shadow_color;
    
    //環境光
    float4 g_ambient_color;
    
    // 光の方向（ワールド空間）
    float3 g_light_dir;
    float g_textures_index;
}

float4 main(PS_INPUT PSInput) : SV_TARGET0
{
    float4 texColor = diffuseMapTexture.Sample(diffuseMapSampler, PSInput.uv);
    
    if (g_textures_index < 0.0f)
    {
        texColor = PSInput.diffuse;
    }
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
    if (diff > 0.66f)
    {
        shade = 1.0f;
    }
    else
    {
        if (diff > 0.33f)
        {
            shade = 0.6f;
        }
        else
        {
            shade = 0.3f;
        }
    }
    float3 color = lerp(g_shadow_color.rgb, g_light_color.rgb, shade) * texColor.rgb * g_color.rgb + g_ambient_color.rgb;

    return float4(color, texColor.a * g_color.a);
}
