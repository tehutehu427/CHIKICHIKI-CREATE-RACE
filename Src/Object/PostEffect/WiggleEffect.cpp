#include "WiggleEffect.h"
#include "../Renderer/PixelRenderer.h"
#include "../Renderer/PixelMaterial.h"
#include "../Manager/Game/PlayerManager.h"
#include "../Manager/System/DateBank.h"
#include "../Common/Vector2.h"
#include "../Application.h"

WiggleEffect::WiggleEffect()
{
	renderer_ = nullptr;
	material_ = nullptr;
	timer_ = 0.0f;
}

WiggleEffect::~WiggleEffect()
{
}

void WiggleEffect::Load()
{
	material_ = std::make_unique<PixelMaterial>("Wiggle.cso", BUFFER_SIZE);
	renderer_ = std::make_unique<PixelRenderer>(*material_);
}

void WiggleEffect::Init()
{
	//バッファー追加
	material_->AddConstBuf(FLOAT4{ 1.0f,0.0f,0.0f,0.0f });
	
	//テクスチャ設定
	material_->AddTextureBuf(SceneManager::GetInstance().GetMainScreen());

	//矩形の設定
	renderer_->MakeSquereVertex(
		Vector2(0, 0),
		Vector2(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y));

}

void WiggleEffect::Draw()
{
	auto& scn = SceneManager::GetInstance();

	//タイマー
	timer_ += scn.GetDeltaTime();

	//バッファー設定
	material_->SetConstBuf(0, FLOAT4{ timer_,0.0f,0.0f,0.0f });

	//スクリーンの設定
	material_->SetTextureBuf(0, scn.GetMainScreen());

	//描画
	renderer_->Draw();
}
