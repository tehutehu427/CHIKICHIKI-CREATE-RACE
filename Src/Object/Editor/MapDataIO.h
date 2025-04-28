#pragma once
#include <string>
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

	//アイテムの名前


	//JSONファイルで出力する
	void ExportJsonFile(const std::string _fileName);

	//JSONファイルを読み込む
	void ImportJsonFile();

};

