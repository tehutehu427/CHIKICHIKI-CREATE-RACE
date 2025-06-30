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

	//ネームフォント
	static constexpr int NAME_FONT_SIZE = 18;	//サイズ
	static constexpr int NAME_FONT_THICK = 0.0f;	//太さ

	//パレットアイコン移動量
	static constexpr int ICONS_MOVE = 10;

	//描画数
	static constexpr int EXCLUSION = 3;//NONE,START,GOALはいらないため除外する
	static constexpr int ICON_NUM = ItemBase::ITEM_NUM_MAX - EXCLUSION;	//除外分減らす
	static constexpr int SCROLL_ICON_NUM = static_cast<int>(SCROLL::MAX);

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
	static constexpr int SCR_ICON_POS_Y[SCROLL_ICON_NUM] = {
		80,
		Application::SCREEN_SIZE_Y - 80 };

	//マスク位置(画像の左上位置)
	static constexpr int MASK_POS_X = Application::SCREEN_HALF_X - MASK_SIZE_X / 2;
	static constexpr int MASK_POS_Y = Application::SCREEN_HALF_Y - MASK_SIZE_Y / 2;

	//スクロールに制限をかける
	static constexpr int SCROLL_LIMIT_LINE = ICON_NUM / COL -1;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PaletteIcon();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~PaletteIcon();
	
	/// <summary>
	/// 読み込み
	/// </summary>
	virtual void Load();	

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init();	

	/// <summary>
	/// 更新
	/// </summary>
	void Update();	

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();	

	/// <summary>
	/// デバッグ描画
	/// </summary>
	virtual void DebagDraw();

	/// <summary>
	/// 状態変更
	/// </summary>
	/// <param name="_state"></param>変更先の状態
	void ChangeState(const STATE _state);

	/// <summary>
	/// 生成判定を返す
	/// </summary>
	/// <returns></returns>生成判定
	const bool IsCreate()const { return isCreate_; }

	/// <summary>
	/// 選択したアイテム種類を返す
	/// </summary>
	/// <param name="_playerIndex"></param>プレイヤーのインデックス(引数がなければ1P)
	/// <returns></returns>選択した種類
	const ItemBase::ITEM_TYPE GetSelectType(const int _playerIndex = 0)const { return selectTypes_[_playerIndex]; }

protected:

	//アイコン
	std::vector<EditorPaletteBase::ImgInfo> icons_;

	//スクロールアイコン
	EditorPaletteBase::ImgInfo scrIcon_[SCROLL_ICON_NUM];

	//選択してるアイテム
	std::vector<int> sleCnt_;
	std::vector<ItemBase::ITEM_TYPE> selectTypes_;	
	
	//生成判定
	bool isCreate_;

	//除外番号を除いたアイテム種類範囲
	std::vector<int> candidates_;

	//状態変更処理の管理
	std::map<STATE, std::function<void()>> stateChanges_;

	//更新処理管理
	std::function<void()> stateUpdate_;

	//状態
	STATE state_;

	//画像
	int imgScrIcon_;	//スクロール用アイコン
	int mskPal_;		//マスク画像
	int* imgIcons_;

	//スクロールの制限用ライン
	int scrLimitLine_;

	//フォント
	int fontHandle_;

	//座標のバックアップ
	Vector2 prePos_;

	//状態変更処理
	void ChangeStateNone();
	void ChangeStateScrollUp();
	void ChangeStateScrollDown();
	void ChangeStateSelect();

	//状態別更新処理
	void UpdateNone();
	void UpdateScrollUp();
	void UpdateScrollDown();
	virtual void UpdateSelect();

	//各アイコンの描画
	virtual void DrawItemIcon();		//アイテムアイコン
	void DrawScrollIcon();		//スクロールアイコン

	//除外番号を除いてアイテム配列を生成
	virtual void SetExcludingItemTypeArray();

	//マスクスクリーンの初期設定
	void InitMaskScreen();

	//クリック位置がスクロールアイコンか調べる
	bool CheckScrollIcon(const Vector2 _mPos);	
	
	//アイテムアイコンをクリックしたか調べる
	virtual bool CheckItemIcon( const Vector2 _mPosconst, int _playerIndex = 0);

};