#include <DxLib.h>
#include "PostEffectMonotone.h"
#include "../../Manager/System/SceneManager.h"
#include "../../Application.h"
#include "../PixelRenderer.h"
#include "../PixelMaterial.h"

PostEffectMonotone::PostEffectMonotone()
{
}

PostEffectMonotone::~PostEffectMonotone()
{
}

void PostEffectMonotone::Init()
{
	//エフェクトスクリーン
	postEffectScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);
	std::string filePath = Application::PATH_SHADER + "Monotone.cso";

	//マテリアル生成
	material_ = std::make_unique<PixelMaterial>(filePath, CONST_BUFF_FLOAT4_SIZE);

	//レンダー生成
	render_ = std::make_unique<PixelRenderer>(*material_);
}
