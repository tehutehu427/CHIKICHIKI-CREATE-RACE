#include "MapDataIO.h"
#include "../../Lib/nlohmann/json.hpp"
#include "../../Manager/System/InputManager.h"

using json = nlohmann::json;

MapDataIO::MapDataIO()
{
}

MapDataIO::~MapDataIO()
{
}

void MapDataIO::Load()
{
}

void MapDataIO::Init()
{
}

void MapDataIO::Update()
{
	InputManager & ins = InputManager::GetInstance();

	//特定のキーを押す、もしくはUIをクリックしたら処理を実行する
	if (ins.IsTrgDown(KEY_INPUT_B))
	{
		//現在の配置データを出力する
		ExportJsonFile("3DStageDataFile");
	}
	
	else if (ins.IsTrgDown(KEY_INPUT_N))
	{
		//外部データを読み込む
		ImportJsonFile();
	}
}

void MapDataIO::ExportJsonFile(const std::string _fileName)
{
	//json j;
	//MapEditer& map = MapEditer::GetInstance();

	////各アイテムの配置情報を格納
	//for (const auto& [type, positions] : grouped) {
	//	for (const auto& pos : positions) {
	//		output[type].push_back({ {"x", pos.x}, {"y", pos.y} });
	//	}
	//}

	//std::ofstream file(filename + ".json");
	//if (file.is_open()) {
	//	file << j.dump(4); // インデント付きで出力
	//	file.close();
	//}
	//else {
	//	MessageBoxA(NULL, "JSONファイルの保存に失敗しました", "エラー", MB_OK);
	//}
}

void MapDataIO::ImportJsonFile()
{
}


