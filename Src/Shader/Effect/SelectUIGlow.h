#pragma once
#include "EffectBase.h"
class SelectUIGlow : public EffectBase
{
public:

	//float4を生成する数
	static constexpr int CONST_BUFF_FLOAT4_SIZE = 12;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SelectUIGlow();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SelectUIGlow();

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load()override;

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="_angle"></param>角度
	/// <param name="_size"></param>サイズ
	void Update(const float _angle, const Vector2 _size);

};

