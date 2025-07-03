#include "ToonStyle.h"
#include "../../Renderer/ModelMaterial.h"
#include "../../Renderer/ModelRenderer.h"


ToonStyle::ToonStyle()
{
	model_ = -1;
	outlineMaterial_ = nullptr;
	outlineRenderer_ = nullptr;
	toonMaterial_ = nullptr;
	toonRenderer_ = nullptr;
}

ToonStyle::~ToonStyle()
{
}

void ToonStyle::Load(int _modelId, const MESH_TYPE _type)
{
	model_ = _modelId;

	std::string outlineVSName = "OutlineMeshVS.cso";
	std::string toonLightingVSName = "ToonMeshVS.cso";

	//シェーダーを設定
	if (_type == MESH_TYPE::SKIN_MESH)
	{
		outlineVSName = "OutlineSkinMeshVS.cso";
		toonLightingVSName = "ToonSkinMeshVS.cso";
	}

	//アウトライン
	outlineMaterial_ = std::make_unique<ModelMaterial>(outlineVSName.c_str(), 1, "OutlinePS.cso", 1);
	outlineRenderer_ = std::make_unique<ModelRenderer>(model_, *outlineMaterial_);

	//トゥーンライト
	toonMaterial_ = std::make_unique<ModelMaterial>(toonLightingVSName.c_str(), 0, "ToonPS.cso", 3);
	toonRenderer_ = std::make_unique<ModelRenderer>(model_, *toonMaterial_);
}

void ToonStyle::Init()
{
	//アウトライン定数バッファの設定
	outlineMaterial_->AddConstBufVS(FLOAT4{ 2.0f,0.0f,0.0f,0.0f });	//輪郭線の太さ
	outlineMaterial_->AddConstBufPS(FLOAT4{	0.0f,0.0f,0.0f,1.0f });	//輪郭線カラー(通常は黒)

	//トゥーンライト定数バッファの設定
	toonMaterial_->AddConstBufPS(FLOAT4{ 1.0f,1.0f, 1.0f, 1.0f });		//光の色
	toonMaterial_->AddConstBufPS(FLOAT4{ 0.4f, 0.3f, 0.3f, 1.0f });		//影の色
	toonMaterial_->AddConstBufPS(FLOAT4{ GetLightDirection().x,GetLightDirection().y, GetLightDirection().z, 0.0f });//ライト方向
}

void ToonStyle::Draw()
{
	//モデル描画のZBufferを無効にする
	MV1SetWriteZBuffer(model_, false);

	//アウトライン描画
	outlineRenderer_->Draw();

	//モデル描画のZBufferを戻す
	MV1SetWriteZBuffer(model_, true);
	
	//描画
	toonRenderer_->Draw();
}

void ToonStyle::OutlineColor(const float _r, const float _g, const float _b)
{
	outlineMaterial_->SetConstBufPS(0, FLOAT4{ _r,_g,_b,1.0f });
}