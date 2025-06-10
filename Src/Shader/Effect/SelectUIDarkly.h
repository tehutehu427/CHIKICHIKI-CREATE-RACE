#pragma once
#include "EffectBase.h"
class SelectUIDarkly : public EffectBase
{
public:

	//float4を生成する数
	static constexpr int CONST_BUFF_FLOAT4_SIZE = 12;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SelectUIDarkly();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SelectUIDarkly();

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load()override;

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="_imgIndex"></param>画像インデックス
	/// <param name="_angle"></param>角度
	/// <param name="_size"></param>サイズ
	/// <param name="_div"></param>分割数
	void Update(const int _imgIndex, const float _angle, const Vector2 _size, const Vector2 _div);
};

