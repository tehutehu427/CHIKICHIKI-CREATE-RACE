#include "EffectBase.h"
#include <DxLib.h>
#include "../PixelMaterial.h"
#include "../PixelRenderer.h"

EffectBase::EffectBase()
{
	material_ = nullptr;
	render_ = nullptr;
}

EffectBase::~EffectBase()
{
}

void EffectBase::Draw(const int _texture, const Vector2 _pos, const Vector2 _size)
{
	//マテリアルにテクスチャを設定
	material_->SetTexture(_texture);

	//テクスチャアドレスを指定
	//material_->SetTextureAdrdress(PixelMaterial::TEXADDRESS::WRAP);

	// レンダーの描画
	render_->Draw(_pos, _size);
}