#pragma once
class RoundDisplay
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	RoundDisplay();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~RoundDisplay();

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
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// ナンバーインデックスに値を追加
	/// </summary>
	/// <param name="_addIndex">追加値</param>
	void AddNumberIndex(const int _addIndex);

private:

	//ラウンド画像
	int imgRound_;

	//ナンバー画像
	int* imgNumbers_;

	//ナンバー用インデックス
	int numberIndex_;

	//ナンバーの描画桁数
	int numberDigit_;

	//数字用インデックスを返す
	int GetDivNumberIndex(const int _num);

};

