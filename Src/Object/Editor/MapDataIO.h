#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "../Item/ItemBase.h"
#include "../../Manager/Game/MapEditer.h"

class ItemName;

class MapDataIO
{
public:

	enum class STATE
	{
		NONE,
		WAIT,
		CHECK_IMPORT,
		CHECK_EXPORT,
	};

	//コンストラクタ
	MapDataIO();

	//デストラクタ
	~MapDataIO();

	void Load();	//読み込み
	void Init();	//初期化
	void Update();	//更新



private:

	//JSONファイルで出力する
	void ExportJsonFile(const std::string _fileName);

	//JSONファイルを読み込む
	void ImportJsonFile();

	//エクスプローラーからファイルを開く
	std::string OpenFileDialog();

	/// <summary>
	/// JSONからアイテム種類ごとに座標を読み込む
	/// </summary>
	/// <param name="filepath"></param>ファイルネームを返す
	/// <returns></returns>読み込んだ種類別配置情報を返す
	std::unordered_map<ItemBase::ITEM_TYPE, std::vector<VECTOR>> LoadItemsFromJson(const std::string& filepath);

};

