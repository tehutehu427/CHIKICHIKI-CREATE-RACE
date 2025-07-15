#pragma once

class SelectScene;

class SelectStage
{
public:

	/// <summary>
	/// ステージ種類
	/// </summary>
	enum class STAGE_TYPE
	{
		BEGINNER,		//初級
		INTERMEDIATE,	//中級
		ADVANCED,		//上級
		ABYSS,			//アビス
		MAX
	};

	//ステージ最大数
	static constexpr int STAGE_TYPE_MAX = static_cast<int>(STAGE_TYPE::MAX);

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
	
	//矢印の数
	static constexpr int ARROW_NUM = 2;

	//メッセージ画像
	int* imgMessages_;

	//レベル選択用画像
	int* imgLevels_;

	//矢印のアイコン
	int imgArrow_;

	//選択用アイコン
	int selectIcon_;

	//選択してるステージインデックス
	int stageIndex_;

	//メッセージの描画
	void DrawMessage();

};

