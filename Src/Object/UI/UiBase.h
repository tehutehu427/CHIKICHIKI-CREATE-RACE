#pragma once
#include "../../Common/Vector2.h"

class ResourceManager;

class UiBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	UiBase();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~UiBase();

	/// <summary>
	/// 読み込み
	/// </summary>
	virtual void Load() = 0;
	
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() = 0;

	//UIに関する情報
	struct UiInfo
	{
		Vector2 pos = {};	//座標
		Vector2 size = {};	//サイズ
		float rate = 1.0f;	//拡大率
		float angle = 0.0f;	//角度
	};

protected:

	ResourceManager& res_;
};

