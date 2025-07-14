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

//描画するテクスチャ
Texture2D g_SrcTexture : register(t0);

cbuffer cbColor : register(b0)
{
    float4 g_color;
    float g_time; // タイマー
    float g_speed; // スクロール速度
}

//サンプラー：適切な色を決める
SamplerState g_SrcSampler
{
    AddressU = WRAP;
    AddressV = WRAP;
};

float4 main(PS_INPUT PSInput) : SV_TARGET
{
	// UV座標とテクスチャを参照して、最適な色を取得する
    float4 srcCol = g_SrcTexture.Sample(g_SrcSampler, PSInput.TexCoords0);
    
    // uv
    float2 uv = PSInput.TexCoords0;
    
    //ラインのスクロール速度
    float lineScrollSpeed = g_speed;
    
    //光らせるエリア
    float area = sin(uv.y * 2.0f - g_time * lineScrollSpeed);
    float isArea = step(0.996f, area * area);
    
    //縦に一定間隔で暗くするために色の減算を行う
    srcCol.rgb -= abs(sin(uv.y * 100.0f - g_time * 2.0f)) * 0.5f;

    //特定範囲だけ、 色加算を行い明るくする。
    srcCol.rgb += isArea * 0.3f;
    
    //特定の範囲にも下地の縞模様が入ってしまうので消す用にする
    srcCol.rgb -= (1.0f - isArea) * abs(sin(uv.y * 60.0f + g_time * 1.0f)) * 0.05f;
    srcCol.rgb -= (1.0f - isArea) * abs(sin(uv.y * 100.0f - g_time * 2.0f)) * 0.15f;

    return srcCol;
}