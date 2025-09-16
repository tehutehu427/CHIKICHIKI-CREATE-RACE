#pragma once
#include "../EditorPaletteBase.h"

class ItemName;

class PaletteIcon
{

public:

	/// <summary>
	/// 状態
	/// </summary>
	enum class STATE
	{
		NONE,
		SCR_UP,
		SCR_DOWN,
		SELCT,
	};

	/// <summary>
	/// スクロール
	/// </summary>
	enum class SCROLL
	{
		UP,
		DOWWN,
		MAX,
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PaletteIcon(void);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~PaletteIcon(void);
	
	/// <summary>
	/// 読み込み
	/// </summary>
	virtual void Load(void);

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init(void);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(void);

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw(void);

	/// <summary>
	/// デバッグ描画
	/// </summary>
	virtual void DebagDraw(void);

	/// <summary>
	/// 状態変更
	/// </summary>
	/// <param name="_state"></param>変更先の状態
	void ChangeState(const STATE _state);

	/// <summary>
	/// 生成判定を返す
	/// </summary>
	/// <returns></returns>生成判定
	const bool IsCreate(void)const { return isCreate_; }

	/// <summary>
	/// 選択したアイテム種類を返す
	/// </summary>
	/// <param name="_playerIndex"></param>プレイヤーのインデックス(引数がなければ1P)
	/// <returns></returns>選択した種類
	const ItemBase::ITEM_TYPE GetSelectType(const int _playerIndex = 0)const { return selectTypes_[_playerIndex]; }

protected:	
	
	//ネームフォント
	static constexpr int NAME_FONT_SIZE = 18;	//サイズ
	static constexpr int NAME_FONT_THICK = 0;	//太さ

	//パレットアイコン移動量
	static constexpr int ICONS_MOVE = 10;

	//描画数
	static constexpr int EXCLUSION = 3;										//NONE,START,GOALはいらないため除外する
	static constexpr int ICON_NUM = ItemBase::ITEM_NUM_MAX - EXCLUSION;		//除外分減らす
	static constexpr int SCROLL_ICON_NUM = static_cast<int>(SCROLL::MAX);	//スクロールアイコン数

	//スクロールアイコン下の回転角度
	static constexpr float SCR_ICON_DEG = 180.0f;

	//拡大率
	static constexpr float ICON_RATE = 1.0f;
	static constexpr float SCR_ICON_RATE = 1.0f;

	//大きさ
	static constexpr int ICON_SIZE = 128;
	static constexpr int SCR_ICON_SIZE = 128;

	//マスクサイズ
	static constexpr int MASK_SIZE_X = 720;
	static constexpr int MASK_SIZE_Y = 510;

	//アイコン位置
	static constexpr int COL = 4;	//列
	static constexpr int ICON_POS_X = Application::SCREEN_HALF_X - static_cast<int>(ICON_SIZE * ICON_RATE * 2);
	static constexpr int ICON_POS_Y = Application::SCREEN_HALF_Y - static_cast<int>(ICON_SIZE * ICON_RATE * 1.5f) + 60;

	//アイコン描画用間隔
	static constexpr int INTERVAL_X = static_cast<int>(ICON_SIZE * 1.3f);
	static constexpr int INTERVAL_Y = static_cast<int>(ICON_SIZE * 1.5f);

	//スクロールアイコン位置
	static constexpr int SCR_ICON_POS_X = Application::SCREEN_HALF_X;
	static constexpr int SCR_ICON_POS_Y[SCROLL_ICON_NUM] =
	{
		80,
		Application::SCREEN_SIZE_Y - 80
	};

	//マスク位置(画像の左上位置)
	static constexpr int MASK_POS_X = Application::SCREEN_HALF_X - MASK_SIZE_X / 2;
	static constexpr int MASK_POS_Y = Application::SCREEN_HALF_Y - MASK_SIZE_Y / 2;

	//スクロールに制限をかける
	static constexpr int SCROLL_LIMIT_LINE = ICON_NUM / COL - 1;

	//画像
	int imgScrIcon_;	//スクロール用アイコン
	int mskPal_;		//マスク画像
	int* imgIcons_;	
	
	//スクロールの制限用ライン
	int scrLimitLine_;

	//フォント
	int fontHandle_;	
	
	//生成判定
	bool isCreate_;	
	
	//状態
	STATE state_;

	//座標のバックアップ
	Vector2 prePos_;

	//スクロールアイコン
	EditorPaletteBase::ImgInfo scrIcon_[SCROLL_ICON_NUM];

	//選択してるアイテム
	std::vector<int> sleCnt_;						//選択しているアイコンのインデックス
	std::vector<ItemBase::ITEM_TYPE> selectTypes_;	//選択しているアイテム種類

	//除外番号を除いたアイテム種類範囲
	std::vector<int> candidates_;	
	
	//アイコン
	std::vector<EditorPaletteBase::ImgInfo> icons_;	
	
	//更新処理管理
	std::function<void(void)> stateUpdate_;

	//状態変更処理の管理
	std::map<STATE, std::function<void(void)>> stateChanges_;

	//状態変更処理
	void ChangeStateNone(void);
	void ChangeStateScrollUp(void);
	void ChangeStateScrollDown(void);
	void ChangeStateSelect(void);

	//状態別更新処理
	void UpdateNone(void) {};
	void UpdateScrollUp(void);
	void UpdateScrollDown(void);
	virtual void UpdateSelect(void);

	//各アイコンの描画
	virtual void DrawItemIcon(void);		//アイテムアイコン
	void DrawScrollIcon(void);				//スクロールアイコン

	//除外番号を除いてアイテム配列を生成
	virtual void SetExcludingItemTypeArray(void);

	//マスクスクリーンの初期設定
	void InitMaskScreen(void);

	//クリック位置がスクロールアイコンか調べる
	bool CheckScrollIcon(const Vector2 _mPos);	
	
	//アイテムアイコンをクリックしたか調べる
	virtual bool CheckItemIcon( const Vector2 _mPosconst, int _playerIndex = 0);

};