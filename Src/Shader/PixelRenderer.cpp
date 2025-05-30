#include <DxLib.h>
#include <cassert>
#include "PixelRenderer.h"
#include "PixelMaterial.h"
#include "../Application.h"

PixelRenderer::PixelRenderer(PixelMaterial& pixelMaterial) :
	pixelMaterial_(pixelMaterial)
{
	for (int i = 0; i < NUM_VERTEX; i++) { vertexs_[i] = {}; }
	for (int i = 0; i < NUM_VERTEX_IDX; i++) { indexes_[i] = {}; }
	pos_ = {};
	size_ = {};
}

void PixelRenderer::MakeSquereVertex(Vector2 pos, Vector2 size)
{
	// 毎回頂点データを作成するのは無駄ですが、
	// シェーダー追加時の作業を減らすため、毎フレーム作成

	int cnt = 0;
	float sX = static_cast<float>(pos.x);
	float sY = static_cast<float>(pos.y);
	float eX = static_cast<float>(pos.x + size.x - 1);
	float eY = static_cast<float>(pos.y + size.y - 1);

	// ４頂点の初期化
	for (int i = 0; i < NUM_VERTEX; i++)
	{
		vertexs_[i].rhw = 1.0f;
		vertexs_[i].dif = GetColorU8(255, 255, 255, 255);
		vertexs_[i].spc = GetColorU8(255, 255, 255, 255);
		vertexs_[i].su = 0.0f;
		vertexs_[i].sv = 0.0f;
	}

	// 左上
	vertexs_[cnt].pos = VGet(sX, sY, 0.0f);
	vertexs_[cnt].u = 0.0f;
	vertexs_[cnt].v = 0.0f;
	cnt++;

	// 右上
	vertexs_[cnt].pos = VGet(eX, sY, 0.0f);
	vertexs_[cnt].u = 1.0f;
	vertexs_[cnt].v = 0.0f;
	cnt++;

	// 右下
	vertexs_[cnt].pos = VGet(eX, eY, 0.0f);
	vertexs_[cnt].u = 1.0f;
	vertexs_[cnt].v = 1.0f;
	cnt++;

	// 左下
	vertexs_[cnt].pos = VGet(sX, eY, 0.0f);
	vertexs_[cnt].u = 0.0f;
	vertexs_[cnt].v = 1.0f;

	/*
	　～～～～～～
		0-----1
		|     |
		|     |
		3-----2
	　～～～～～～
		0-----1
		|  ／
		|／
		3
	　～～～～～～
			  1
		   ／ |
		 ／   |
		3-----2
	　～～～～～～
	*/


	// 頂点インデックス
	cnt = 0;
	indexes_[cnt++] = 0;
	indexes_[cnt++] = 1;
	indexes_[cnt++] = 3;

	indexes_[cnt++] = 1;
	indexes_[cnt++] = 2;
	indexes_[cnt++] = 3;
}

void PixelRenderer::Draw(Vector2 pos, Vector2 size)
{
	//画面初期化
	//ClearDrawScreen();

	//オリジナルシェーダーを使用
	MV1SetUseOrigShader(true);

	// シェーダー適用
	pixelMaterial_.ShaderApply();

	// テクスチャ適用
	pixelMaterial_.TexuresApply();

	// バッファー適用
	pixelMaterial_.ConstBufApply();

	// ポリゴン生成
	MakeSquereVertex(pos, size);

	//テクスチャアドレスモードを適用
	pixelMaterial_.TextureAddressModeApply();

	// 描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	DrawPolygonIndexed2DToShader(vertexs_, 4, indexes_, 2);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//バッファー解除
	pixelMaterial_.ConstBufUnapply();

	//テクスチャ解除
	pixelMaterial_.TexuresUnapply();

	//シェーダー解除
	pixelMaterial_.ShaderUnapply();

	//オリジナルシェーダーを解除
	MV1SetUseOrigShader(false);
}