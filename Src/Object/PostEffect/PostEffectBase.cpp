#include "PostEffectBase.h"
#include "../Renderer/PixelRenderer.h"
#include "../Renderer/PixelMaterial.h"

PostEffectBase::PostEffectBase(void)
{
	renderer_ = nullptr;
	material_ = nullptr;
}

void PostEffectBase::Draw(void)
{
	renderer_->Draw();
}
