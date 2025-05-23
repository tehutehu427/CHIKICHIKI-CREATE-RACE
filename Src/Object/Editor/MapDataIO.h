#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include "../Item/ItemBase.h"

class ItemName;

class MapDataIO
{
public:

	//状態
	enum class STATE
	{
		NONE,
		WAIT,
		CHECK_IMPORT,
		CHECK_EXPORT,
	};

	//確認
	enum class CHECK_LIST
	{
		YES,
		NO,
		MAX
	};
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	MapDataIO();

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
	
	// 状態ごとの構造体（更新と描画を分けて保持）
	struct StateFuncs
	{
		std::function<void()> updateFunc;
		std::function<void()> drawFunc;
	};

	//入出力確認用ステップ
	int checkStep_;

	//状態
	STATE state_;

	//選択したファイル
	std::string selectFile_;

	//状態変更処理の管理
	std::unordered_map<STATE, StateFuncs> stateMap_;

	//状態の処理を登録
	void RegisterState(const STATE _state, std::function<void()> _update, std::function<void()> _draw);

	//状態変更処理
	inline void ChangeState(const STATE _state) { state_ = _state; }

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

	/// <summary>
	/// JSONからアイテム種類ごとに座標を読み込む
	/// </summary>
	/// <param name="filepath"></param>ファイルネームを返す
	/// <returns></returns>読み込んだ種類別配置情報を返す
	std::unordered_map<ItemBase::ITEM_TYPE, std::vector<VECTOR>> LoadItemsFromJson(const std::string& _filepath);

	//確認画面の背景
	void DrawCheckBackBox();

	//確認画面のコマンドの描画
	void DrawCheckCommand();

};