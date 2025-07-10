#pragma once
#include <vector>
#include <string>

class YesNoResponder
{
public:

	enum class RESPON
	{
		NONE = -1,
		YES = 0,
		NO = 1,
		MAX = 2
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	YesNoResponder();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~YesNoResponder();

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
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// 選択結果を返す
	/// </summary>
	/// <returns>選択結果</returns>
	const RESPON GetRespon() const { return respon_; }

private:

	//フォント大きさ
	static constexpr int FONT_SIZE = 48;

	//画像
	int* imgs_;

	//選択インデックス
	int index_;

	//フォント
	int font_;

	//背景画像
	int imgBack_;

	//回答
	RESPON respon_;

	//メッセージ
	std::vector<std::string> messages_;

};

