#include "WiggleEffect.h"
#include "../Renderer/PixelRenderer.h"
#include "../Renderer/PixelMaterial.h"
#include "../Application.h"

WiggleEffect::WiggleEffect()
{
	renderer_ = nullptr;
	material_ = nullptr;
}

WiggleEffect::~WiggleEffect()
{
}

void WiggleEffect::Load()
{
	material_ = std::make_unique<PixelMaterial>(Application::PATH_SHADER + "Wiggle.cso", BUFFER_SIZE);
	renderer_ = std::make_unique<PixelRenderer>(*material_);
}

void WiggleEffect::Init()
{

}

void WiggleEffect::Draw()
{
}
