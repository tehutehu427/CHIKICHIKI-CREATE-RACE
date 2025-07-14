#pragma once
#include "EditorPaletteBase.h"

class MultiPalette : public EditorPaletteBase
{
public:

	static constexpr float RATE_SPEED = 0.05f;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="editControllers"></param>エディットコントローラーの配列
	MultiPalette(std::vector<std::unique_ptr<EditController>>& editControllers);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~MultiPalette(void) override;

	/// <summary>
	/// 読み込み処理
	/// </summary>
	/// <param name=""></param>
	void Load(void) override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name=""></param>
	void Init(void) override;

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name=""></param>
	void Draw(void)	override;

private:

	//タイトル画像
	int imgTitle_;

	//状態別更新
	void UpdateSelect()override;		//選ぶ
};

