#include "PostEffectBlur.h"
#include <DxLib.h>
#include "../../Application.h"
#include "../PixelRenderer.h"
#include "../PixelMaterial.h"

PostEffectBlur::PostEffectBlur()
{
	blurStep_ = 0.0f; 
	invertStep_ = 1.0f;
}

PostEffectBlur::~PostEffectBlur()
{
}

void PostEffectBlur::Init()
{
	//エフェクトスクリーン
	postEffectScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);
	std::string filePath = Application::PATH_SHADER + "blur.cso";

	//マテリアル生成
	material_ = std::make_unique<PixelMaterial>(filePath, CONST_BUFF_FLOAT4_SIZE);

	//レンダー生成
	render_ = std::make_unique<PixelRenderer>(*material_);
}

void PostEffectBlur::UpdateConstBuffer()
{
	//ブラーの距離を更新
	blurStep_ += BLUR_SPEED * invertStep_;

	//条件値に達したら反転
	if (blurStep_ >= MAX_BLUR_DISTANCE ||
		blurStep_ <= 0.0f)
	{
		invertStep_ *= -1.0f;
	}

	//定数バッファに分割数(最大数は描画サイズ)を設定する
	COLOR_F* cbBuf = (COLOR_F*)GetBufferShaderConstantBuffer(material_->GetBuffer());
	cbBuf->r = 1.0f;
	cbBuf->g = 1.0f; 
	cbBuf->b = 1.0f; 
	cbBuf->a = 1.0f; 

	cbBuf++; //同一の定数バッファで、意味が異なる場合は、

	cbBuf->r = blurStep_;		//ブラーの距離
	cbBuf->g = Application::SCREEN_SIZE_X - 2;	//画像サイズX
	cbBuf->b = Application::SCREEN_SIZE_Y - 2;	//画像サイズY   

	// ピクセルシェーダー用の定数バッファを更新して書き込んだ内容を反映する
	UpdateShaderConstantBuffer(material_->GetBuffer());
}
