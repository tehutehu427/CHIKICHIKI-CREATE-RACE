#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include "../../Common/Vector2.h"
#include "../Item/ItemBase.h"

class ItemName;
class YesNoResponder;

class MapDataIO
{
public:

	//フォントサイズ
	static constexpr int FONT_SIZE = 48;
	static constexpr int EXPORT_FONT_SIZE = 32;	//出力用

	//アイコンサイズ
	static constexpr int ICON_SIZE_X = 128;
	static constexpr int ICON_SIZE_Y = 64;

	/// <summary>
	/// 状態
	/// </summary>
	enum class STATE
	{
		NONE,
		WAIT,
		CHECK_IMPORT,
		CHECK_EXPORT,
	};

	/// <summary>
	/// 確認
	/// </summary>
	enum class CHECK_LIST
	{
		YES,
		NO,
		MAX
	};

	/// <summary>
	/// メッセージタイプ
	/// </summary>
	enum class MESSAGE_TYPE
	{		
		YES,
		NO,
		IMPORT,
		EXPORT,
		SAVE,
		REPORT_EXPORT,
		REPORT_IMPORT,
		MAX,
	};
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	MapDataIO(const Vector2& _padCursolPos);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~MapDataIO();

	/// <summary>
	/// 読みこみ
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

private:	

	//保尊メッセージ表示時間
	static constexpr float MES_DISPLAY_TIME = 2.0f;

	//カーソル座標
	const Vector2& padCursolPos_;

	//画像関係 
	int imgSave_;	//セーブ
	int imgLoad_;	//読み込み
	int imgBack_;	//背景
	
	//フォント
	int font_;
	int exportFont_;

	//メッセージタイプ
	int messageType_;
	
	// 状態ごとの構造体（更新と描画を分けて保持）
	struct StateFuncs
	{
		std::function<void()> updateFunc;
		std::function<void()> drawFunc;
	};

	//入力データ
	struct ImportData
	{
		std::vector<VECTOR> positions;
		std::vector<VECTOR> rotations;
		std::vector<Quaternion> quaternions;
	};

	//入出力確認用ステップ
	int checkStep_;

	//保存メッセージ表示カウント
	float messageDisplayCnt_;

	//状態
	STATE state_;

	//選択したファイル
	std::string selectFile_;

	//メッセージ種類
	std::string messages_[static_cast<int>(MESSAGE_TYPE::MAX)];

	//回答を返す
	std::unique_ptr<YesNoResponder>responder_;

	//状態変更処理の管理
	std::unordered_map<STATE, StateFuncs> stateMap_;

	//状態の処理を登録
	void RegisterState(const STATE _state, std::function<void()> _update, std::function<void()> _draw);

	//状態変更処理
	void ChangeState(const STATE _state) { state_ = _state; }

	//状態別更新
	void UpdateWait();
	void UpdateCheckExport();
	void UpdateCheckImport();

	//状態別描画
	void DrawWait();
	void DrawCheckExport();
	void DrawCheckImport();

	//ファイルを読み込んでboolで返す
	bool ReadFileBool(std::string &_file);

	//JSONファイルで出力する
	void ExportJsonFile(const std::string _fileName);

	//JSONファイルを読み込む
	void ImportJsonFile();

	//出力を行うトリガーの条件を満たしたか
	bool IsTriggerExport() const;

	//入力を行うトリガーの条件を満たしたか
	bool IsTriggerImport() const;

	/// <summary>
	/// JSONからアイテム種類ごとに座標を読み込む
	/// </summary>
	/// <param name="filepath">ファイルネーム</param>
	/// <returns>読み込んだ種類別配置情報を返す</returns>
	std::unordered_map<ItemBase::ITEM_TYPE, ImportData> LoadItemsFromJson(const std::string& _filepath);

	//ファイルネームを取得
	std::string GetFileName();

};