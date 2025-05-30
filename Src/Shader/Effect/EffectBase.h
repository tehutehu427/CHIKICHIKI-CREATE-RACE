#pragma once
#include <memory>
#include "../../Common/Vector2.h"

class PixelMaterial;
class PixelRenderer;

class EffectBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	EffectBase();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~EffectBase();

	/// <summary>
	/// 読み込み
	/// </summary>
	virtual void Load() = 0;

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="_texture"></param>画像
	/// <param name="_pos"></param>座標
	virtual void Draw(const int _texture, const Vector2 _pos, const Vector2 _size);

protected:

	//マテリアル
	std::unique_ptr<PixelMaterial> material_;

	//レンダー
	std::unique_ptr<PixelRenderer> render_;

};

