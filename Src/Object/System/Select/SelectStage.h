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
		EASY,			//初級
		NORMAL,			//中級
		ADVANCE,		//アドバンス
		HARD,			//上級
		OBLIVION,		//オヴィリオン
		ABYSS,			//アビス
		MAX
	};

	//ステージ最大数
	static constexpr int STAGE_TYPE_MAX = static_cast<int>(STAGE_TYPE::MAX);

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SelectStage(void);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SelectStage(void) = default;

	/// <summary>
	/// 読み込み
	/// </summary>
	void Load(void);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="_parent">親クラス</param>
	void Update(SelectScene& _parent);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(void);

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
	void DrawMessage(void);
};