#pragma once
class EditorExplanation
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	EditorExplanation();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EditorExplanation();

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

	//描画位置
	static constexpr int POS_X = 120;
	static constexpr int POS_Y = 200;

	//拡大率
	static constexpr float RATE = 0.8f;

	//画像
	int imgPadExplanation_;
	int imgKeyExplanation_;
};

