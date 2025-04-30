#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "MapDataIO.h"
#include "../../Lib/nlohmann/json.hpp"
#include "../../Common/IntVector3.h"
#include "../../Common/Quaternion.h"
#include "../../Manager/System/InputManager.h"
#include "../../Manager/System/DateBank.h"
#include "../../Manager/Game/ItemManager.h"
#include "../../Manager/Game/MapEditer.h"

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
		ExportJsonFile("3DStageDataFile.json");
	}
	
	else if (ins.IsTrgDown(KEY_INPUT_N))
	{
		//外部データを読み込む
		ImportJsonFile();
	}
}

void MapDataIO::ExportJsonFile(const std::string _fileName)
{
    nlohmann::json j;

    for (int i = 0; i < ItemBase::ITEM_NUM_MAX; i++)
    {
        ItemBase::ITEM_TYPE type = static_cast<ItemBase::ITEM_TYPE>(i);
        const auto& items = ItemManager::GetInstance().GetItems(type);

        if (items && !items->empty())
        {
            nlohmann::json positions = nlohmann::json::array();

            for (const auto& item : *items)
            {
                if (!item) continue; // nullチェックを追加

                VECTOR pos = item->GetTransform().pos;
                positions.push_back({
                    {"x", pos.x},
                    {"y", pos.y},
                    {"z", pos.z}
                    });
            }

            std::string typeName = DateBank::GetInstance().GetItemName(type);
            if (!typeName.empty()) // 空文字チェック
            {
                j[typeName] = positions;
            }
            else
            {
                std::cerr << "警告: 空のアイテム名を持つタイプがありました（type=" << i << "）\n";
            }
        }
    }

    std::ofstream outFile(_fileName);
    if (outFile.is_open())
    {
        try {
            outFile << j.dump(4);
            std::cout << "アイテムを " << _fileName << " に出力しました。\n";
        }
        catch (const std::exception& e) {
            std::cerr << "JSON出力中に例外発生: " << e.what() << "\n";
        }
        outFile.close();
    }
    else
    {
        std::cerr << "ファイルを開けませんでした: " << _fileName << "\n";
    }
}

void MapDataIO::ImportJsonFile()
{
    ItemManager& itemMng = ItemManager::GetInstance();

    std::string filepath = OpenFileDialog();
    if (filepath.empty()) 
    {
        std::cout << "ファイルが選択されませんでした。\n";
        return;
    }

    //今あるオブジェクトを削除
    itemMng.AllDeleteItem();

    //読み込み
    auto items = LoadItemsFromJson(filepath);

    // 読み込んだアイテムを確認
    for (const auto& [type, positions] : items)
    {
        for (const auto& pos : positions) 
        {
            //マップ座標に変換
            IntVector3 mapPos = MapEditer::GetInstance().WorldToMapPos(pos);

            //格納
            itemMng.AddItem(mapPos, Quaternion(), type);
        }
    }
}

std::string MapDataIO::OpenFileDialog()
{
    char filename[MAX_PATH] = "";

    OPENFILENAMEA ofn = {};
    ofn.lStructSize = sizeof(OPENFILENAMEA);
    ofn.lpstrFile = filename;
    ofn.nMaxFile = sizeof(filename);
    ofn.lpstrFilter = "JSON Files\0*.json\0All Files\0*.*\0";
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    if (GetOpenFileNameA(&ofn))
    {
        return std::string(filename);
    }
    return ""; // キャンセルされた場合
}

std::unordered_map<ItemBase::ITEM_TYPE, std::vector<VECTOR>> MapDataIO::LoadItemsFromJson(const std::string& filepath)
{
    std::unordered_map<ItemBase::ITEM_TYPE, std::vector<VECTOR>> items = {};

    std::ifstream inFile(filepath);
    if (!inFile.is_open()) {
        std::cerr << "ファイルを開けませんでした: " << filepath << "\n";
        return items;
    }

    json j;
    inFile >> j;

    for (int i = 0; i < ItemBase::ITEM_NUM_MAX; i++)
    {
        //アイテムの種類
        ItemBase::ITEM_TYPE type = static_cast<ItemBase::ITEM_TYPE>(i);
        //アイテムの名前を取得
        std::string typeName = DateBank::GetInstance().GetItemName(type);

        if (j.contains(typeName))
        {
            std::vector<VECTOR> positions;
            for (auto& pos : j[typeName])
            {
                VECTOR position;
                position.x = pos["x"].get<float>();
                position.y = pos["y"].get<float>();
                position.z = pos["z"].get<float>();  
                positions.push_back(position);
            }
            items[type] = positions;
        }
    }

    return items;
}
