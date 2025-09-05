#include "PostEffectBase.h"
#include "../Renderer/PixelRenderer.h"
#include "../Renderer/PixelMaterial.h"

PostEffectBase::PostEffectBase()
{
	renderer_ = nullptr;
	material_ = nullptr;
}

void PostEffectBase::Draw()
{
	renderer_->Draw();
}
