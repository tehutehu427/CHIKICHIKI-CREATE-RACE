#include "../common/Pixel/PixelShader2DHeader.hlsli"

cbuffer cbColor : register(b0)
{
    float4 g_color;         //カラー
    float g_sprite_index;   //インデックス
    float g_angle;          //角度
    float2 g_size;          //サイズ
    float2 g_divs;          //分割数
    float g_distance;       //ブラー距離
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
    float directions = 16.0f;
    float quality = 3.0f;
    float2 radius = float2(0.01f, 0.01f);
    //float2 radius = float2(g_distance / g_size.x, g_distance / g_size.y); // 半径
    float twoPi = 6.2831853f;
    
    //分割--------------------------------------------------------
    
    //1枚のスプライトのサイズ
    float2 textuteSize = 1.0 / g_divs;
    
    //インデックスを2次元に変換
    float2 index2d;
    index2d.x = fmod(g_sprite_index, g_divs.x);
    index2d.y = floor(g_sprite_index / g_divs.x);
    
    //回転--------------------------------------------------------
    
    // UVを回転
    float2 localUv = RotateUV(uv, g_angle);	
    
    // スプライト用のUVに変換
    float2 textureUv = localUv * textuteSize + index2d * textuteSize;    
    
    // UV座標とテクスチャを参照して、最適な色を取得する
    float4 srcCol = tex.Sample(texSampler, textureUv);
    
    // 全方向に向けてぼかす
    for (float d = 0.0f; d < twoPi; d += twoPi / directions)
    {
        float2 dir = float2(cos(d), sin(d)); // 単位方向ベクトル

        for (float i = 1.0f; i <= quality; i += 1.0f)
        {
            float2 offset = dir * radius * (i / quality); // ブラーの段階的距離
            srcCol += tex.Sample(texSampler, textureUv + offset);
        }
    }
   
    // 平均化
    srcCol /= (7.0f * directions * quality);
    
    //範囲外を除外------------------------------------------------
    
    ////範囲外のピクセルは描画しない
    //if (localUv.x < 0.0 || localUv.x > 1.0 || localUv.y < 0.0 || localUv.y > 1.0)
    //{
    //    clip(-1); // or discard;
    //}

    // 完全に赤で塗る
    return srcCol * g_color;
}