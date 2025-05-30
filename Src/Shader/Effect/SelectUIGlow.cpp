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
	std::string filePath = Application::PATH_SHADER + "GlowRotateSprite.cso";

	//マテリアル生成
	material_ = std::make_unique<PixelMaterial>(filePath, CONST_BUFF_FLOAT4_SIZE);

	//レンダー生成
	render_ = std::make_unique<PixelRenderer>(*material_);
}

void SelectUIGlow::Update(const int _index, const float _angle, const Vector2 _size, const Vector2 _divs)
{
	//定数バッファに分割数(最大数は描画サイズ)を設定する
	COLOR_F* cbBuf = (COLOR_F*)GetBufferShaderConstantBuffer(material_->GetBuffer());

	if (_index >= 5)
	{
		return;
	}

	//色指定
	cbBuf->r = 1.0f;
	cbBuf->g = 1.0f;
	cbBuf->b = 1.0f;
	cbBuf->a = 1.0f;

	cbBuf++; 

	cbBuf->r = _index;		//インデックス
	cbBuf->g = _angle;		//角度
	cbBuf->b = _size.x;		//画像サイズX
	cbBuf->b = _size.y;		//画像サイズY

	cbBuf++;

	cbBuf->r = _divs.x;		//分割数X
	cbBuf->g = _divs.y;		//分割数Y

	// ピクセルシェーダー用の定数バッファを更新して書き込んだ内容を反映する
	UpdateShaderConstantBuffer(material_->GetBuffer());
}