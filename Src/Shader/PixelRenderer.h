#pragma once
#include <memory>
#include "../Common/Vector2.h"

class PixelMaterial;

class PixelRenderer
{
public:

	//頂点数
	static constexpr int NUM_VERTEX = 4;


	//頂点インデックス数
	static constexpr int NUM_VERTEX_IDX = 6;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pixelMaterial"></param>マテリアルクラス
	PixelRenderer(PixelMaterial& pixelMaterial);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PixelRenderer() = default;

	/// <summary>
	/// 自身で指定した範囲に描画する
	/// </summary>
	/// <param name="pos"></param>座標位置(左上)
	/// <param name="size"></param>範囲
	void Draw(Vector2 pos, Vector2 size);	

private:

	// 座標
	Vector2 pos_;

	// 描画サイズ
	Vector2 size_;

	// 頂点
	VERTEX2DSHADER vertexs_[NUM_VERTEX];

	// 頂点インデックス
	WORD indexes_[NUM_VERTEX_IDX];

	// ピクセルマテリアル
	PixelMaterial& pixelMaterial_;
	
	// 描画矩形の生成
	void MakeSquereVertex(Vector2 pos, Vector2 size);

};

