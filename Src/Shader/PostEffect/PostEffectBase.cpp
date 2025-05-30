#include <DxLib.h>
#include "PostEffectBase.h"
#include "../../Manager/System/SceneManager.h"
#include "../../Application.h"
#include "../PixelMaterial.h"
#include "../PixelRenderer.h"

PostEffectBase::PostEffectBase()
{
	postEffectScreen_ = -1;
	material_ = nullptr;
	render_ = nullptr;
}

PostEffectBase::~PostEffectBase()
{
	DeleteGraph(postEffectScreen_);
}

void PostEffectBase::Update()
{
	//バッファー設定の更新
	UpdateConstBuffer();
}

void PostEffectBase::Draw()
{
	//ポストエフェクトを描画
	int mainScreen = SceneManager::GetInstance().GetMainScreen();

	//マテリアルにテクスチャを設定
	material_->SetTexture(mainScreen);

	//自身のスクリーンを設定
	SetDrawScreen(postEffectScreen_);

	// レンダーの描画
	render_->Draw({ 0,0 }, { Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y });

	//ポストエフェクトを描画
	SetDrawScreen(mainScreen);
	DrawGraph(0, 0, postEffectScreen_, false);
}

void PostEffectBase::UpdateConstBuffer()
{
	//デフォルトの設定
	// ピクセルシェーダー用の定数バッファのアドレスを取得
	COLOR_F* cbBuf = (COLOR_F*)GetBufferShaderConstantBuffer(material_->GetBuffer());
	cbBuf->r = 1.0f;
	cbBuf->g = 1.0f;
	cbBuf->b = 1.0f;
	cbBuf->a = 1.0f;

	// ピクセルシェーダー用の定数バッファを更新して書き込んだ内容を反映する
	UpdateShaderConstantBuffer(material_->GetBuffer());
}
