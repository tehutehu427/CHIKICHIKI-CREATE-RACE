#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "MapDataIO.h"
#include "../../Application.h"
#include "../../Utility/Utility.h"
#include "../../Lib/nlohmann/json.hpp"
#include "../../Common/IntVector3.h"
#include "../../Common/Quaternion.h"
#include "../../Manager/System/InputManager.h"
#include "../../Manager/System/DateBank.h"
#include "../../Manager/Game/ItemManager.h"
#include "../../Manager/Game/MapEditer.h"

using json = nlohmann::json;

namespace
{
    constexpr int appear_interval = 30;	//出現までのフレーム
    constexpr int menu_line_height = 40;//メニュー1つ当たりの高さ
    constexpr int margin_size = 20;		//ポーズメニュー枠の太さ
}

MapDataIO::MapDataIO()
{
    checkStep_ = 0;
    state_ = STATE::NONE;
}

MapDataIO::~MapDataIO()
{
}

void MapDataIO::Load()
{
}

void MapDataIO::Init()
{
    //状態ごとの処理を登録
    RegisterState(STATE::WAIT, [&]() { UpdateWait(); }, [&]() { DrawWait(); });
    RegisterState(STATE::CHECK_EXPORT, [&]() { UpdateCheckExport(); }, [&]() { DrawCheckExport(); });
    RegisterState(STATE::CHECK_IMPORT, [&]() { UpdateCheckImport(); }, [&]() { DrawCheckImport(); });
    
    //最初の状態を設定
    ChangeState(STATE::WAIT); 
}

void MapDataIO::Update()
{
    if (stateMap_.count(state_)) 
    {
        if (stateMap_[state_].updateFunc)
        {
            stateMap_[state_].updateFunc();
        }
    }
}

void MapDataIO::Draw()
{
    if (stateMap_.count(state_))
    {
        if (stateMap_[state_].drawFunc)
        {
            stateMap_[state_].drawFunc();
        }
    }
}

void MapDataIO::ExportJsonFile(const std::string _fileName)
{
    nlohmann::json j;
    constexpr int INDENT = 4;

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
        try 
        {
            outFile << j.dump(INDENT);
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

std::unordered_map<ItemBase::ITEM_TYPE, std::vector<VECTOR>> MapDataIO::LoadItemsFromJson(const std::string& _filepath)
{
    std::unordered_map<ItemBase::ITEM_TYPE, std::vector<VECTOR>> items = {};

    std::ifstream inFile(_filepath);
    if (!inFile.is_open()) {
        std::cerr << "ファイルを開けませんでした: " << _filepath << "\n";
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

void MapDataIO::DrawCheckBackBox()
{
    constexpr int alpha = 128;
    constexpr float lineThickness = 3.0f;

    //白っぽいセロファン
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
    DrawBoxAA(
        margin_size,
        margin_size,
        Application::SCREEN_SIZE_X - margin_size,
        Application::SCREEN_SIZE_Y - margin_size,
        Utility::WHITE,
        true);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    //白枠
    DrawBoxAA(
        margin_size,
        margin_size,
        Application::SCREEN_SIZE_X - margin_size,
        Application::SCREEN_SIZE_Y - margin_size,
        Utility::WHITE,
        false,
        lineThickness);
}

void MapDataIO::DrawCheckCommand()
{
    //選択コマンドの描画
    std::string commandMes[static_cast<int>(CHECK_LIST::MAX)] = { "はい","いいえ" };

    for (int i = 0; i < static_cast<int>(CHECK_LIST::MAX); i++)
    {
        //選択しているのを色を変える
        int commandColor = Utility::BLUE;  //初期カラー 
        if (i == checkStep_) { commandColor = Utility::RED; }

        //コマンドメッセージを描画
        DrawFormatString(
            Application::SCREEN_HALF_X - 20 + i * 50,
            Application::SCREEN_HALF_Y + 40,
            commandColor,
            commandMes[i].c_str()
        );
    }
}

void MapDataIO::RegisterState(const STATE _state, std::function<void()> _update, std::function<void()> _draw)
{
    stateMap_[_state] = StateFuncs{ _update, _draw };
}

void MapDataIO::UpdateWait()
{
    InputManager& ins = InputManager::GetInstance();

    //特定のキーを押す、もしくはUIをクリックしたら処理を実行する
    if (ins.IsTrgDown(KEY_INPUT_B))
    {
        ChangeState(STATE::CHECK_EXPORT);
    }

    else if (ins.IsTrgDown(KEY_INPUT_N))
    {
        ChangeState(STATE::CHECK_IMPORT);
    }
}

void MapDataIO::UpdateCheckExport()
{  
    InputManager& ins = InputManager::GetInstance();

    //コマンドの選択
    if (ins.IsTrgDown(KEY_INPUT_RIGHT) || ins.IsTrgDown(KEY_INPUT_LEFT))
    {
        //増減方向
        int dir = ins.IsTrgDown(KEY_INPUT_RIGHT) ? 1 : -1;

        //ステップ更新
        checkStep_ += dir;

        //範囲の巻き戻し
        if (checkStep_ < 0)
        {
            checkStep_ = static_cast<int>(CHECK_LIST::MAX) - 1;
        }
        else if (checkStep_ >= static_cast<int>(CHECK_LIST::MAX))
        {
            checkStep_ = 0;
        }
    }

    //決定
    else if (ins.IsTrgDown(KEY_INPUT_RETURN))
    {
        //選択した内容の処理を行う
        if (checkStep_ == static_cast<int>(CHECK_LIST::YES))
        {
            //現在の配置データを出力する
            ExportJsonFile("3DStageDataFile.json");

            //状態遷移
            ChangeState(STATE::WAIT);
        }
        else
        {
            //状態遷移
            ChangeState(STATE::WAIT);
        }
    }

}

void MapDataIO::UpdateCheckImport()
{
    InputManager& ins = InputManager::GetInstance();

    //コマンドの選択
    if (ins.IsTrgDown(KEY_INPUT_RIGHT) || ins.IsTrgDown(KEY_INPUT_LEFT))
    {
        //増減方向
        int dir = ins.IsTrgDown(KEY_INPUT_RIGHT) ? 1 : -1;

        //ステップ更新
        checkStep_ += dir;

        //範囲の巻き戻し
        if (checkStep_ < 0)
        {
            checkStep_ = static_cast<int>(CHECK_LIST::MAX) - 1;
        }
        else if (checkStep_ >= static_cast<int>(CHECK_LIST::MAX))
        {
            checkStep_ = 0;
        }
    }

    //決定
    else if (ins.IsTrgDown(KEY_INPUT_RETURN))
    {
        //選択した内容の処理を行う
        if (checkStep_ == static_cast<int>(CHECK_LIST::YES))
        {
            //外部データを読み込む    
            ImportJsonFile();

            //状態遷移
            ChangeState(STATE::WAIT);
        }
        else
        {
            //状態遷移
            ChangeState(STATE::WAIT);
        }
    }
}

void MapDataIO::DrawWait()
{
    //特別描画することはなし
}

void MapDataIO::DrawCheckExport()
{
    //背景の描画
    DrawCheckBackBox();

    //メッセージの描画
    DrawFormatString(
        Application::SCREEN_HALF_X,
        Application::SCREEN_HALF_Y,
        Utility::BLUE,
        "今の配置で保存しますか？"
    );

    //確認コマンドの描画
    DrawCheckCommand();
}

void MapDataIO::DrawCheckImport()
{
    //背景描画
    DrawCheckBackBox();

    //メッセージの描画
    DrawFormatString(
        Application::SCREEN_HALF_X,
        Application::SCREEN_HALF_Y,
        Utility::BLUE,
        "今の配置しているアイテムは消えますがよろしいですか？"
    );

    //確認コマンドの描画
    DrawCheckCommand();
}