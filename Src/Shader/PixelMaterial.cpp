#include <Dxlib.h>
#include <cassert>
#include "PixelMaterial.h"
#include "PixelRenderer.h"
#include "PostEffect/PostEffectBase.h"

PixelMaterial::PixelMaterial(std::string shaderFileName, int constBufFloat4Size):
	constBufFloat4Size_(constBufFloat4Size)
{
	//初期化
	constBuf_ = 0;
	texAddress_ = TEXADDRESS::MAX;	
	constBufs_.clear();
	textures_.clear();

	//読み込み
 	shader_ = LoadPixelShader(shaderFileName.c_str());
	assert(shader_ != -1);	//正しく読み込まれない場合強制終了
	constBuf_ = CreateShaderConstantBuffer(sizeof(float) * constBufFloat4Size_);

}

void PixelMaterial::SetTexture(int texture)
{
	textures_.push_back(texture);
}

void PixelMaterial::SetTextureAdrdress(const TEXADDRESS address)
{
	texAddress_ = address;
}

void PixelMaterial::ShaderApply()
{
	SetUsePixelShader(shader_);
}

void PixelMaterial::ShaderUnapply() 
{
	SetUsePixelShader(-1);
}

void PixelMaterial::ConstBufApply()
{
	SetShaderConstantBuffer(constBuf_, DX_SHADERTYPE_PIXEL, 0);
}

void PixelMaterial::ConstBufUnapply() 
{
	SetShaderConstantBuffer(-1, DX_SHADERTYPE_PIXEL, 0);
}

void PixelMaterial::TexuresApply()
{
	//空の場合処理をやめる
	if (textures_.empty()) { return; }

	int index = 0;
	for (auto& texture : textures_)
	{
		SetUseTextureToShader(index, texture);
		index++;
	}
}

void PixelMaterial::TexuresUnapply()
{
	for (int i = textures_.size() -1; i >= 0; i--)
	{
		SetUseTextureToShader(i, -1);
	}

	//テクスチャの中身を削除
	textures_.clear();
}

void PixelMaterial::TextureAddressModeApply()
{
	switch (texAddress_)
	{
	case TEXADDRESS::WRAP:
		SetTextureAddressModeUV(DX_TEXADDRESS_WRAP, DX_TEXADDRESS_WRAP);
		break;

	case TEXADDRESS::BORDER:
		SetTextureAddressModeUV(DX_TEXADDRESS_BORDER, DX_TEXADDRESS_BORDER);
		break;

	case TEXADDRESS::MIRROR:
		SetTextureAddressModeUV(DX_TEXADDRESS_MIRROR, DX_TEXADDRESS_MIRROR);
		break;

	case TEXADDRESS::CLAMP:
		SetTextureAddressModeUV(DX_TEXADDRESS_CLAMP, DX_TEXADDRESS_CLAMP);
		break;

	default:
		break;
	}
}
