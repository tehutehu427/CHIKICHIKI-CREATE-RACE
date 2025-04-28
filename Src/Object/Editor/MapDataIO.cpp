#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "MapDataIO.h"
#include "../../Lib/nlohmann/json.hpp"
#include "../../Manager/System/InputManager.h"
#include "../../Manager/Game/ItemManager.h"
#include "../../Manager/System/DateBank.h"

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
    json j;

    for (int i = 0; ItemBase::ITEM_NUM_MAX; i++) {
        //アイテムの種類
        ItemBase::ITEM_TYPE type = static_cast<ItemBase::ITEM_TYPE>(i);

        //指定されたアイテムの配列
        const auto& items = ItemManager::GetInstance().GetItems(type);

        //配列の中身がカラじゃない場合
        if (items && !items->empty()) {
            nlohmann::json positions = nlohmann::json::array();

            //配置情報の格納
            for (const auto& item : *items) {
                VECTOR pos = item->GetTransform().pos;
                positions.push_back({
                    {"x", pos.x},
                    {"y", pos.y},
                    {"z", pos.z}
                });
            }
            std::string typeName = DateBank::GetInstance().GetItemName(type);
            j[typeName] = positions;
        }
    }

    std::ofstream outFile(_fileName);
    if (outFile.is_open()) {
        outFile << j.dump(4); // インデント付きで出力
        outFile.close();
        std::cout << "アイテムを " << _fileName << " に出力しました。\n";
    }
    else {
        std::cerr << "ファイルを開けませんでした: " << _fileName << "\n";
    }
}

void MapDataIO::ImportJsonFile()
{
}