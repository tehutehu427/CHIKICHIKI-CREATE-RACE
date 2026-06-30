#include "ToonStyle.h"
#include "../../Renderer/ModelMaterial.h"
#include "../../Renderer/ModelRenderer.h"

ToonStyle::ToonStyle(void)
{
	type_ = MESH_TYPE::NO_TEXTURE;
	model_ = -1;
	drawFunc_ = std::bind(&ToonStyle::ToonDrawModel, this);
	outlineMaterial_ = nullptr;
	outlineRenderer_ = nullptr;
	toonMaterial_ = nullptr;
	toonRenderer_ = nullptr;
}

void ToonStyle::Load(int _modelId, const MESH_TYPE _type)
{
	model_ = _modelId;
	type_ = _type;

	std::string outlineVSName = "OutlineMeshVS.cso";
	std::string toonLightingVSName = "ToonMeshVS.cso";
	std::string toonLightingPSName = "ToonPS.cso";

	//シェーダーを設定
	if (type_ == MESH_TYPE::SKIN_MESH)
	{
		outlineVSName = "OutlineSkinMeshVS.cso";
		toonLightingVSName = "ToonSkinMeshVS.cso";
	}
	else if(type_ == MESH_TYPE::CHICKEN)
	{
		outlineVSName = "ChickenOutlineVS.cso";
	}
	else if (type_ == MESH_TYPE::NO_TEXTURE)
	{
		toonLightingPSName = "ToonNoTexturePS.cso";
	}

	//アウトライン
	outlineMaterial_ = std::make_unique<ModelMaterial>(outlineVSName.c_str(), 1, "OutlinePS.cso", 1);
	outlineRenderer_ = std::make_unique<ModelRenderer>(model_, *outlineMaterial_);

	//トゥーンライト
	if (type_ == MESH_TYPE::CHICKEN)
	{ 
		drawFunc_ = std::bind(&ToonStyle::NormalDrawModel, this);
		return;
	}
	toonMaterial_ = std::make_unique<ModelMaterial>(toonLightingVSName.c_str(), 0, toonLightingPSName, 5);
	toonRenderer_ = std::make_unique<ModelRenderer>(model_, *toonMaterial_);
}

void ToonStyle::Init(void)
{
	//アウトライン定数バッファの設定
	outlineMaterial_->AddConstBufVS(FLOAT4{ 5.0f,0.0f,0.0f,0.0f });	//輪郭線の太さ
	outlineMaterial_->AddConstBufPS(FLOAT4{	0.0f,0.0f,0.0f,1.0f });	//輪郭線カラー(通常は黒)

	//トゥーンライト定数バッファの設定
	if (type_ == MESH_TYPE::CHICKEN)
	{
		return;
	}

	toonMaterial_->AddConstBufPS(FLOAT4{ 1.0f,1.0f, 1.0f, 1.0f });		//色
	toonMaterial_->AddConstBufPS(FLOAT4{ 1.0f,1.0f, 1.0f, 1.0f });		//光の色
	toonMaterial_->AddConstBufPS(FLOAT4{ 0.4f, 0.3f, 0.3f, 1.0f });		//影の色
	toonMaterial_->AddConstBufPS(FLOAT4{ 0.1f, 0.1f, 0.1f, 1.0f });		//環境光
	toonMaterial_->AddConstBufPS(FLOAT4{ GetLightDirection().x,GetLightDirection().y, GetLightDirection().z, 0.0f });//ライト方向
}

void ToonStyle::Draw(void)
{
	//モデル描画のZBufferを無効にする
	MV1SetWriteZBuffer(model_, false);

	//アウトライン描画
	outlineRenderer_->Draw();

	//モデル描画のZBufferを戻す
	MV1SetWriteZBuffer(model_, true);
	
	//本体の描画
	drawFunc_();
}

void ToonStyle::SetModelColor(const float _r, const float _g, const float _b, const float _a)
{
	if (type_ == MESH_TYPE::CHICKEN)
	{
		MV1SetEmiColorScale(model_, COLOR_F{ _r,_g,_b,_a });
		return;
	}
	else
	{
		toonMaterial_->SetConstBufPS(0, FLOAT4{ _r,_g,_b,_a });
		return;
	}
}

void ToonStyle::SetOutlineColor(const float _r, const float _g, const float _b, const float _a)
{
	outlineMaterial_->SetConstBufPS(0, FLOAT4{ _r,_g,_b,_a });
}

void ToonStyle::NormalDrawModel(void)
{
	//通常描画
	MV1DrawModel(model_);
}

void ToonStyle::ToonDrawModel(void)
{
	//描画
	toonRenderer_->Draw();
}