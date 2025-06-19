#include "MapDataIO.h"
#include <iostream>
#include <fstream>
#include "../../Application.h"
#include "../../Utility/Utility.h"
#include "../../Lib/nlohmann/json.hpp"
#include "../../Common/IntVector3.h"
#include "../../Common/Quaternion.h"
#include "../../Common/FontRegistry.h"
#include "../../Manager/System/InputManager.h"
#include "../../Manager/System/DateBank.h"
#include "../../Manager/System/SceneManager.h"
#include "../../Manager/System/KeyConfig.h"
#include "../../Manager/System/ResourceManager.h"
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
    imgBack_ = -1;
    imgLoad_ = -1;
    imgBack_ = -1;
    font_ = -1;

    checkStep_ = 0;
    state_ = STATE::NONE;
    selectFile_ = "";

    //状態ごとの処理を登録
    RegisterState(STATE::WAIT, [&]() { UpdateWait(); }, [&]() { DrawWait(); });
    RegisterState(STATE::CHECK_EXPORT, [&]() { UpdateCheckExport(); }, [&]() { DrawCheckExport(); });
    RegisterState(STATE::CHECK_IMPORT, [&]() { UpdateCheckImport(); }, [&]() { DrawCheckImport(); });

    //メッセージを設定
    messages_[static_cast<int>(MESSAGE_TYPE::IMPORT)] = "今配置してるアイテムは消えますがよろしいですか？";
    messages_[static_cast<int>(MESSAGE_TYPE::EXPORT)] = "データを保存しますか？";
    messages_[static_cast<int>(MESSAGE_TYPE::YES)] = "はい";
    messages_[static_cast<int>(MESSAGE_TYPE::NO)] = "いいえ";
    messages_[static_cast<int>(MESSAGE_TYPE::REPORT)] = "ファイルに保存されました";

}

MapDataIO::~MapDataIO()
{
    DeleteFontToHandle(exportFont_);
    DeleteFontToHandle(font_);
}

void MapDataIO::Load()
{
    //ファイルパスの指定
    selectFile_ = GetFileName();
    ImportJsonFile();

    //画像
    ResourceManager& res = ResourceManager::GetInstance();
    imgLoad_ = res.Load(ResourceManager::SRC::LOAD_ICON).handleId_;
    imgSave_ = res.Load(ResourceManager::SRC::SAVE_ICON).handleId_;
    imgBack_ = res.Load(ResourceManager::SRC::EXPLAN_BACK).handleId_;

    //フォント生成
    font_ = CreateFontToHandle(FontRegistry::BOKUTATI.c_str(), FONT_SIZE, 0);
    exportFont_ = CreateFontToHandle(FontRegistry::BOKUTATI.c_str(), EXPORT_FONT_SIZE, 0);
}

void MapDataIO::Init()
{
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

        //中身がカラじゃない場合
        if (items && !items->empty())
        {
            //座標格納用の配列を用意
            nlohmann::json positions = nlohmann::json::array();

            //アイテム分回す
            for (const auto& item : *items)
            {
                //ない場合次へ
                if (!item) continue; // nullチェックを追加

                //座標取得
                VECTOR pos = item->GetTransform().pos;
                positions.push_back(
                    {   //格納
                    {"x", pos.x},
                    {"y", pos.y},
                    {"z", pos.z}
                    });
            }

            //名前取得
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

    //JSONファイルを出力
    std::ofstream outFile(_fileName);
    if (outFile.is_open())
    {
        try 
        {
            outFile << j.dump(INDENT);
            std::cout << "アイテムを " << _fileName << " に出力しました。\n";
        }
        catch (const std::exception& e)
        {
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

    //今あるオブジェクトを削除
    itemMng.AllDeleteItem();

    //読み込み
    auto items = LoadItemsFromJson(selectFile_);

    // 読み込んだアイテムを確認
    for (const auto& [type, positions] : items)
    {
        for (const auto& pos : positions) 
        {
            //マップ座標に変換
            IntVector3 mapPos = MapEditer::GetInstance().WorldToMapPos(pos);

            //格納
            itemMng.AddItem(mapPos, Quaternion(), type,0.0f);
			MapEditer::GetInstance().AddItem(
				{ mapPos, Quaternion(), type },itemMng.GetItemSize(type),
                itemMng.GetItemHitSize(type),0.0f);
        }
    }
}

bool MapDataIO::IsTriggerExport() const
{
    KeyConfig& ins = KeyConfig::GetInstance();

    //アイコンクリック用
    const Vector2 rightPos = { ICON_SIZE_X, 0 };
    const Vector2 leftDown = { ICON_SIZE_X * 2, ICON_SIZE_Y };

    //特定のキーを押す、もしくはUIをクリックしたら処理を実行する
    if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::EXPORT_FILE, KeyConfig::JOYPAD_NO::PAD1) || 
        ins.IsTrgDown(KeyConfig::CONTROL_TYPE::EXPORT_FILE_CLICK, KeyConfig::JOYPAD_NO::PAD1) && 
        Utility::IsPointInRect(ins.GetMousePos(), rightPos, leftDown))
    {
        return true;
    }

    return false;
}

bool MapDataIO::IsTriggerImport() const
{
    KeyConfig& ins = KeyConfig::GetInstance();

    //アイコンクリック用
    const Vector2 rightPos = { 0, 0 };
    const Vector2 leftDown = { ICON_SIZE_X, ICON_SIZE_Y };

    //特定のキーを押す、もしくはUIをクリックしたら処理を実行する
    if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::IMPORT_FILE, KeyConfig::JOYPAD_NO::PAD1) ||
        ins.IsTrgDown(KeyConfig::CONTROL_TYPE::IMPORT_FILE_CLICK, KeyConfig::JOYPAD_NO::PAD1) &&
        Utility::IsPointInRect(ins.GetMousePos(), rightPos, leftDown))
    {
        return true;
    }

    return false;
}

std::unordered_map<ItemBase::ITEM_TYPE, std::vector<VECTOR>> MapDataIO::LoadItemsFromJson(const std::string& _filepath)
{
    std::unordered_map<ItemBase::ITEM_TYPE, std::vector<VECTOR>> items = {};

    std::ifstream inFile(_filepath);
    if (!inFile.is_open()) 
    {
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
    constexpr int ALPHA = 128;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, ALPHA);
    DrawExtendGraph(0, 0,
        Application::SCREEN_SIZE_X,
        Application::SCREEN_SIZE_Y,
        imgBack_,
        true);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void MapDataIO::DrawCheckCommand()
{
    //選択コマンドの描画
    std::string commandMes[static_cast<int>(CHECK_LIST::MAX)] = { 
        messages_[static_cast<int>(MESSAGE_TYPE::YES)],
        messages_[static_cast<int>(MESSAGE_TYPE::NO)]
    };

    const Vector2 OFFSET_POS = { 150, 40 };
    constexpr int MARGIN = 200;

    for (int i = 0; i < static_cast<int>(CHECK_LIST::MAX); i++)
    {
        //選択しているのを色を変える
        int commandColor = Utility::BLUE;  //初期カラー 
        if (i == checkStep_) { commandColor = Utility::RED; }

        //コマンドメッセージを描画
        DrawFormatStringToHandle(
            Application::SCREEN_HALF_X - OFFSET_POS.x + i * MARGIN,
            Application::SCREEN_HALF_Y + OFFSET_POS.y,
            commandColor,
            font_,
            commandMes[i].c_str()
        );
    }
}

std::string MapDataIO::GetFileName()
{
    //パス指定
    std::string path = Application::PATH_JSON;

    //シーンごとに呼び出すファイルを変える
    switch (SceneManager::GetInstance().GetSceneID())
    {
    case SceneManager::SCENE_ID::MULTI:
    case SceneManager::SCENE_ID::FREE:
        return path + "DefaultStage.json";
        break;
   
    case SceneManager::SCENE_ID::SOLO:
        return path + "ChallengeStage.json";
        break;

    default:
        break;
    }
}

void MapDataIO::RegisterState(const STATE _state, std::function<void()> _update, std::function<void()> _draw)
{
    stateMap_[_state] = StateFuncs{ _update, _draw };
}

void MapDataIO::UpdateWait()
{
    KeyConfig& ins = KeyConfig::GetInstance();

    //特定のキーを押す、もしくはUIをクリックしたら処理を実行する
    if (IsTriggerExport())
    {
        //確認へ移る
        ChangeState(STATE::CHECK_EXPORT);
        return;
    }

    else if (IsTriggerImport())
    {
        //ファイルを読み込む
        if (!ReadFileBool(selectFile_))
        {
            //読み込まない場合処理を終える
            return;
        }
        //確認へ移る
        ChangeState(STATE::CHECK_IMPORT);
    }
}

void MapDataIO::UpdateCheckExport()
{  
    KeyConfig& ins = KeyConfig::GetInstance();

    //コマンドの選択
    if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_LEFT, KeyConfig::JOYPAD_NO::PAD1) || ins.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_RIGHT, KeyConfig::JOYPAD_NO::PAD1))
    {
        //増減方向
        int dir = ins.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_RIGHT, KeyConfig::JOYPAD_NO::PAD1) ? 1 : -1;

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
    else if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::DECISION_KEY_AND_PAD,KeyConfig::JOYPAD_NO::PAD1))
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
    KeyConfig& ins = KeyConfig::GetInstance();

    //コマンドの選択
    if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_LEFT, KeyConfig::JOYPAD_NO::PAD1) || 
        ins.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_RIGHT, KeyConfig::JOYPAD_NO::PAD1))
    {
        //増減方向
        int dir = ins.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_RIGHT, KeyConfig::JOYPAD_NO::PAD1) ? 1 : -1;

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
    else if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::DECISION_KEY_AND_PAD, KeyConfig::JOYPAD_NO::PAD1))
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
    DrawRotaGraph(
        ICON_SIZE_X / 2,
        ICON_SIZE_Y / 2,
        1.0f,
        0.0f,
        imgLoad_,
        true,
        false
     );

    DrawRotaGraph(
        ICON_SIZE_X + ICON_SIZE_X / 2,
        ICON_SIZE_Y / 2,
        1.0f,
        0.0f,
        imgSave_,
        true,
        false
    );
}

void MapDataIO::DrawCheckExport()
{
    //背景の描画
    DrawCheckBackBox();

    //メッセージ描画位置調整
    std::string mes = messages_[static_cast<int>(MESSAGE_TYPE::EXPORT)];
    const Vector2 OFFSET_POS = { 
        static_cast<int>(mes.length() * FONT_SIZE / 4),
        -50
    };

    //メッセージの描画
    DrawFormatStringToHandle(
        Application::SCREEN_HALF_X - OFFSET_POS.x,
        Application::SCREEN_HALF_Y + OFFSET_POS.y,
        Utility::WHITE,
        font_,
        mes.c_str());

    //確認コマンドの描画
    DrawCheckCommand();
}

void MapDataIO::DrawCheckImport()
{
    //背景描画
    DrawCheckBackBox();

    //メッセージ描画位置調整
    std::string mes = messages_[static_cast<int>(MESSAGE_TYPE::IMPORT)];
    const Vector2 OFFSET_POS = {
        static_cast<int>(mes.length() * EXPORT_FONT_SIZE / 4),
        -50
    };

    //メッセージの描画
    DrawFormatStringToHandle(
        Application::SCREEN_HALF_X - OFFSET_POS.x,
        Application::SCREEN_HALF_Y + OFFSET_POS.y,
        Utility::WHITE,
        exportFont_,
        mes.c_str());

    //確認コマンドの描画
    DrawCheckCommand();
}

bool MapDataIO::ReadFileBool(std::string &_file)
{
    //エクスプローラーからファイルを読み込む
    _file = Utility::OpenFileDialog();

    //ファイルが空の場合
    if (_file.empty())
    {
        std::cout << "ファイルが選択されませんでした。\n";
        return false;   //失敗判定を返す
    }
    return true;
}