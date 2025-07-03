#include "../common/Pixel/PixelShader2DHeader.hlsli"

float4 main(PS_INPUT PSInput) : SV_TARGET
{
	// UV座標とテクスチャを参照して、最適な色を取得する
    float4 srcCol = tex.Sample(texSampler, PSInput.uv);
    
    // 輝度計算（NTSC係数）
    float gray = dot(srcCol.rgb, float3(0.299, 0.587, 0.114));
   
    // グレースケールカラーを返す（RGB 全て同じ値）
    return float4(gray, gray, gray, srcCol.a);
}
