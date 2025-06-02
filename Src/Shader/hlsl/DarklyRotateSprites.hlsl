#include "../common/Pixel/PixelShader2DHeader.hlsli"

cbuffer cbColor : register(b0)
{
    float4 g_color; //カラー
    float g_sprite_index; //インデックス
    float g_angle; //角度
    float2 g_size; //サイズ
    float2 g_divs; //分割数
}

float2 RotateUV(float2 uv, float angleRad)
{
    float2 centeredUV = uv - 0.5;

    float cosA = cos(angleRad);
    float sinA = sin(angleRad);

    float2 rotatedUV;
    rotatedUV.x = centeredUV.x * cosA + centeredUV.y * sinA;
    rotatedUV.y = centeredUV.x * sinA - centeredUV.y * cosA;

    return rotatedUV + 0.5;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float2 uv = PSInput.uv;
    
    //分割--------------------------------------------------------
    
    //1枚のスプライトのサイズ
    float2 textuteSize = 1.0 / g_divs;
    
    //インデックスを2次元に変換
    float2 index2d;
    index2d.x = fmod(g_sprite_index, g_divs.x);
    index2d.y = floor(g_sprite_index / g_divs.y);
    
    //回転--------------------------------------------------------
    
    // UVを回転
    float2 localUv = RotateUV(uv, g_angle);
    
    // スプライト用のUVに変換
    float2 textureUv = localUv * textuteSize + index2d * textuteSize;
    
    // UV座標とテクスチャを参照して、最適な色を取得する
    float4 srcCol = tex.Sample(texSampler, textureUv);
    
    //範囲外のピクセルは描画しない
    if (localUv.x < 0.0 || localUv.x > 1.0 || localUv.y < 0.0 || localUv.y > 1.0)
    {
        clip(-1); // or discard;
    }

    //カラーをかける
    return srcCol * g_color;
}