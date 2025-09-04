#include "FishEyeEffect.h"
#include "../Renderer/PixelRenderer.h"
#include "../Renderer/PixelMaterial.h"
#include "../Manager/Game/PlayerManager.h"
#include "../Manager/System/DateBank.h"
#include "../Common/Vector2.h"
#include "../Application.h"

FishEyeEffect::FishEyeEffect()
{
	screenDivX_ = 0.0f;
	screenDivY_ = 0.0f;
}

FishEyeEffect::~FishEyeEffect()
{
}

void FishEyeEffect::Load()
{
	material_ = std::make_unique<PixelMaterial>("FishEye.cso", BUFFER_SIZE);
	renderer_ = std::make_unique<PixelRenderer>(*material_);
}

void FishEyeEffect::Init()
{
	const Vector2 SCREEN_DIV[PlayerManager::PLAYER_NUM_MAX] =
	{
		{1,1},
		{2,1},
		{2,2},
		{2,2}
	};

	//画面分割の設定
	int index = DateBank::GetInstance().GetPlayerNum() - 1;
	screenDivX_ = static_cast<float>(SCREEN_DIV[index].x);
	screenDivY_ = static_cast<float>(SCREEN_DIV[index].y);

	//バッファー追加
	material_->AddConstBuf(FLOAT4{ screenDivX_,screenDivY_,0.0f,0.0f });

	//テクスチャ設定
	material_->AddTextureBuf(SceneManager::GetInstance().GetMainScreen());

	//矩形の設定
	renderer_->MakeSquereVertex(
		Vector2(0, 0),
		Vector2(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y));
}

void FishEyeEffect::Draw()
{
	renderer_->Draw();
}
