#include "SelectUIGlow.h"
#include <DxLib.h>
#include <string>
#include "../Application.h"
#include "../PixelMaterial.h"
#include "../PixelRenderer.h"

SelectUIGlow::SelectUIGlow()
{
}

SelectUIGlow::~SelectUIGlow()
{
}

void SelectUIGlow::Load()
{
	//ファイルパス指定
	std::string filePath = Application::PATH_SHADER + "RotateGlow.cso";

	//マテリアル生成
	material_ = std::make_unique<PixelMaterial>(filePath, CONST_BUFF_FLOAT4_SIZE);

	//レンダー生成
	render_ = std::make_unique<PixelRenderer>(*material_);
}

void SelectUIGlow::Update(const float _angle, const Vector2 _size)
{
	//定数バッファに分割数(最大数は描画サイズ)を設定する
	COLOR_F* cbBuf = (COLOR_F*)GetBufferShaderConstantBuffer(material_->GetBuffer());

	//色指定
	cbBuf->r = 1.0f;
	cbBuf->g = 1.0f;
	cbBuf->b = 0.0f;
	cbBuf->a = 1.0f;

	cbBuf++; 

	cbBuf->r = _size.x;		//画像サイズX
	cbBuf->g = _size.y;		//画像サイズY	
	cbBuf->b = _angle;		//角度
	cbBuf->a = 16.0f;		//ブラー距離	

	// ピクセルシェーダー用の定数バッファを更新して書き込んだ内容を反映する
	UpdateShaderConstantBuffer(material_->GetBuffer());
}