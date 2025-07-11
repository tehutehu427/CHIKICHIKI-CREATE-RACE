#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include "../../Common/Vector2.h"
#include "../Item/ItemBase.h"
#include "../Manager/System/SceneManager.h"

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
		FINISH,
	};
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	MapDataIO(const Vector2& _padCursorPos);

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

	/// <summary>
	/// エディットに制限を掛ける
	/// </summary>
	/// <returns>制限中はtrue,　それ以外はfalse</returns>
	const bool IsEdit() const;

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

private:	

	//保尊メッセージ表示時間
	static constexpr float MES_DISPLAY_TIME = 2.0f;

	//マルチステージ種類
	static constexpr float MULTI_STAGE_TYPES = 4;

	//カーソル座標
	const Vector2& padCursorPos_;

	//メッセージ画像インデックス
	static constexpr int IMG_EXPORT_INDEX = 0;	//出力
	static constexpr int IMG_IMPORT_INDEX = 1;	//入力

	//画像関係 
	int imgSave_;	//セーブ
	int imgLoad_;	//読み込み
	int imgBack_;	//背景
	int* imgEditMessages_;		//エディットメッセージ
	int* imgSystemMessages_;	//システムメッセージ
	
	//フォント
	int font_;
	int exportFont_;

	//メッセージタイプ
	int messageType_;

	//システムメッセージ画像用インデックス
	int systemMessageIndex_;
	
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

	//回答を返す
	std::unique_ptr<YesNoResponder>responder_;

	//状態変更処理の管理
	std::unordered_map<STATE, StateFuncs> stateMap_;

	//モード別にファイルネームを取得
	std::unordered_map<SceneManager::SCENE_ID, std::function<std::string()>> getFileNameMap_;

	//状態の処理を登録
	void RegisterGetFileName(const SceneManager::SCENE_ID _sceneId, std::function<std::string()> _func);
	void RegisterState(const STATE _state, std::function<void()> _update, std::function<void()> _draw);

	//状態変更処理
	void ChangeState(const STATE _state) { state_ = _state; }

	//状態別更新
	void UpdateWait();
	void UpdateCheckExport();
	void UpdateCheckImport();
	void UpdateFinish();

	//状態別描画
	void DrawWait();
	void DrawCheckExport();
	void DrawCheckImport();
	void DrawFinish();

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

	//JSONからアイテム種類ごとに座標を読み込む
	std::unordered_map<ItemBase::ITEM_TYPE, ImportData> LoadItemsFromJson(const std::string& _filepath);

	//ファイルネームを取得
	std::string GetFreeFileName();
	std::string GetSoloFileName();
	std::string GetMultiFileName();


};