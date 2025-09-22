#pragma once
#include <memory>
#include <string>
#include <functional>
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
		CHICKEN,		//チキン
		MESH,			//通常
		NO_TEXTURE,		//テクスチャなし
		SKIN_MESH,		//スキンメッシュ
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ToonStyle(void);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ToonStyle(void) = default;

	/// <summary>
	/// 読み込み処理
	/// </summary>
	/// <param name="_modelId">モデル</param>
	/// <param name="_type">メッシュの種類(アニメーションをするのはSKIN_MESH)</param>
	void Load(int _modelId, const MESH_TYPE _type);

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(void);
	
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(void);

	/// <summary>
	/// アウトラインの色を指定
	/// </summary>
	/// <param name="_r">赤値</param>
	/// <param name="_g">緑値</param>
	/// <param name="_b">青値</param>
	/// <param name="_a">アルファ値</param>
	void SetOutlineColor(const float _r, const float _g, const float _b, const float _a);

	/// <summary>
	/// モデルカラーを設定
	/// </summary>
	/// <param name="_r">赤値</param>
	/// <param name="_g">緑値</param>
	/// <param name="_b">青値</param>
	/// <param name="_a">アルファ値</param>
	void SetModelColor(const float _r, const float _g, const float _b, const float _a);

private:

	//モデル
	int model_;

	//種類
	MESH_TYPE type_;

	//マテリアル
	std::unique_ptr<ModelMaterial> toonMaterial_;	//トゥーン用
	std::unique_ptr<ModelMaterial> outlineMaterial_;//アウトライン用

	//レンダー
	std::unique_ptr<ModelRenderer> toonRenderer_;	//トゥーン用
	std::unique_ptr<ModelRenderer> outlineRenderer_;//アウトライン用

	//描画管理
	std::function<void(void)> drawFunc_;

	//通常描画
	void NormalDrawModel(void);

	//トゥーン描画
	void ToonDrawModel(void);

};

