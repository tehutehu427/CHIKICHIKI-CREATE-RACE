#pragma once

class SelectScene;

class SelectStage
{
public:

	static constexpr int COL = 2;
	static constexpr int LINE = 2;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SelectStage();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SelectStage();

	/// <summary>
	/// 読み込み
	/// </summary>
	void Load();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="_parent">親クラス</param>
	void Update(SelectScene& _parent);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:

	//選択してるステージインデックス
	int stageIndex_;

};

