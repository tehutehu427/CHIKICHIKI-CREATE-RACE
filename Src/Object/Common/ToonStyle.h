#pragma once
#include <memory>
#include <string>
#include "../../Renderer/ModelMaterial.h"
#include "../../Renderer/ModelRenderer.h"

class ToonStyle
{
public:

	/// <summary>
	/// メッシュ種類
	/// </summary>
	enum class MESH_TYPE
	{
		MESH,
		SKIN_MESH,
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ToonStyle();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ToonStyle();

	/// <summary>
	/// 読み込み処理
	/// </summary>
	/// <param name="_modelId">モデル</param>
	/// <param name="_type">メッシュの種類(アニメーションをするのはSKIN_MESH)</param>
	void Load(int _modelId, const MESH_TYPE _type);

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// アウトラインの色を指定
	/// </summary>
	/// <param name="_r">赤値</param>
	/// <param name="_g">緑値</param>
	/// <param name="_b">青値</param>
	void OutlineColor(const float _r, const float _g, const float _b);

private:

	//モデル
	int model_;

	//マテリアル
	std::unique_ptr<ModelMaterial> toonMaterial_;	//トゥーン用
	std::unique_ptr<ModelMaterial> outlineMaterial_;//アウトライン用

	//レンダー
	std::unique_ptr<ModelRenderer> toonRenderer_;	//トゥーン用
	std::unique_ptr<ModelRenderer> outlineRenderer_;//アウトライン用

};

