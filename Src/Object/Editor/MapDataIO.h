#pragma once
#include <string>
#include "../../Manager/Game/MapEditer.h"

class MapDataIO
{
public:

	MapDataIO();
	~MapDataIO();

	void Load();
	void Init();
	void Update();

private:

	//JSONファイルで出力する
	void ExportJsonFile(const std::string _fileName);

	//JSONファイルを読み込む
	void ImportJsonFile();

};

