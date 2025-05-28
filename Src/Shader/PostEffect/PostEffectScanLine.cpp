#include "PostEffectScanLine.h"
#include <DxLib.h>
#include "../../Application.h"
#include "../../Manager/System/SceneManager.h"
#include "../PixelMaterial.h"
#include "../PixelRenderer.h"

PostEffectScanLine::PostEffectScanLine()
{
}

PostEffectScanLine::~PostEffectScanLine()
{
}

void PostEffectScanLine::Init()
{
	//エフェクトスクリーン
	postEffectScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);
	std::string filePath = Application::PATH_SHADER + "ScanLine.cso";

	//マテリアル生成
	material_ = std::make_unique<PixelMaterial>(filePath, CONST_BUFF_FLOAT4_SIZE);

	//レンダー生成
	render_ = std::make_unique<PixelRenderer>(*material_);
}

void PostEffectScanLine::UpdateConstBuffer()
{
	//定数バッファに分割数(最大数は描画サイズ)を設定する
	COLOR_F* cbBuf = (COLOR_F*)GetBufferShaderConstantBuffer(material_->GetBuffer());
	cbBuf->r = 1.0f;
	cbBuf->g = 1.0f; //元がfloat4のCOLOR_Fを使用しているので、
	cbBuf->b = 1.0f; //ポインタ加算して、5番目のfloatを、
	cbBuf->a = 1.0f; //cbBuf->r として使用していますが、

	cbBuf++; //同一の定数バッファで、意味が異なる場合は、

	cbBuf->r += SceneManager::GetInstance().GetDeltaTime();				//タイマー
	cbBuf->g = 1.0f;		//スクロール速度

	// ピクセルシェーダー用の定数バッファを更新して書き込んだ内容を反映する
	UpdateShaderConstantBuffer(material_->GetBuffer());
}
