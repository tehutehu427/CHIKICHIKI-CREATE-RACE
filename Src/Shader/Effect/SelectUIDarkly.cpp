#include "SelectUIDarkly.h"
#include <DxLib.h>
#include <string>
#include "../Application.h"
#include "../PixelMaterial.h"
#include "../PixelRenderer.h"

SelectUIDarkly::SelectUIDarkly()
{
}

SelectUIDarkly::~SelectUIDarkly()
{
}

void SelectUIDarkly::Load()
{
	//ファイルパス指定
	std::string filePath = Application::PATH_SHADER + "DarklyRotateSprites.cso";

	//マテリアル生成
	material_ = std::make_unique<PixelMaterial>(filePath, CONST_BUFF_FLOAT4_SIZE);

	//レンダー生成
	render_ = std::make_unique<PixelRenderer>(*material_);
}

void SelectUIDarkly::Update(const int _imgIndex, const float _angle, const Vector2 _size, const Vector2 _div)
{
	//定数バッファに分割数(最大数は描画サイズ)を設定する
	COLOR_F* cbBuf = (COLOR_F*)GetBufferShaderConstantBuffer(material_->GetBuffer());

	//色指定
	cbBuf->r = 0.2f;
	cbBuf->g = 0.2f;
	cbBuf->b = 0.2f;
	cbBuf->a = 1.0f;

	cbBuf++;

	cbBuf->r = _imgIndex;	//画像インデックス
	cbBuf->g = _angle;		//角度
	cbBuf->b = _size.x;		//サイズX
	cbBuf->a = _size.y;		//サイズY

	cbBuf++;

	cbBuf->r = _div.x;		//分割数X
	cbBuf->g = _div.y;		//分割数Y

	// ピクセルシェーダー用の定数バッファを更新して書き込んだ内容を反映する
	UpdateShaderConstantBuffer(material_->GetBuffer());
}
