#pragma once
#include <memory>

class PixelRenderer;
class PixelMaterial;

class WiggleEffect
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	WiggleEffect();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~WiggleEffect();

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private:

	static constexpr int BUFFER_SIZE = 1;

	//レンダラー
	std::unique_ptr<PixelRenderer> renderer_;
	
	//マテリアル
	std::unique_ptr<PixelMaterial> material_;

};

