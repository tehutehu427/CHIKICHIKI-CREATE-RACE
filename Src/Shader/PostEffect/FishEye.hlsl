#include "../common/Pixel/PixelShader2DHeader.hlsli"

cbuffer cbColor : register(b4)
{
    // 画面分割 (横, 縦)
    float2 g_screenDiv;
    
    // タイマー（演出用に使いたければ利用可能）
    float g_time;
    float dummy;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float2 uv = PSInput.uv;

    // どの分割に属するかを判定
    int px = (int) (uv.x * g_screenDiv.x);
    int py = (int) (uv.y * g_screenDiv.y);

    // 各分割領域のサイズ
    float2 cellSize = 1.0 / g_screenDiv;

    // このプレイヤー画面の左上座標
    float2 cellMin = float2(px, py) * cellSize;

    // このプレイヤー画面の中心座標
    float2 center = cellMin + cellSize * 0.5;

    // 中心からのベクトル
    float2 offset = uv - center;

    // 分割領域の大きさで正規化
    offset /= cellSize * 0.5;

    // 距離を計算
    float r = length(offset);

    // 魚眼効果（樽型歪曲）
    float k = 0.4; // 歪みの強さ（大きくすると歪む）
    float scale = 1.0 + k * r * r;

    offset /= scale;

    // UV座標に戻す
    uv = center + offset * (cellSize * 0.5);

    // テクスチャの色を取得
    return tex.Sample(texSampler, uv);
}