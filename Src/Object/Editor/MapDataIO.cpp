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
#include "../System/YesNoResponder.h"

using json = nlohmann::json;

namespace
{
    constexpr int MARGIN = 200;             //間   
    const Vector2 OFFSET_POS = { 150, 40 }; //座標調整用
    const Vector2 YES_POS = {               
            Application::SCREEN_HALF_X - OFFSET_POS.x,
            Application::SCREEN_HALF_Y + OFFSET_POS.y,
    };
}

MapDataIO::MapDataIO(const Vector2& _padCursolPos):
    padCursolPos_(_padCursolPos)
{
    imgBack_ = -1;
    imgLoad_ = -1;
    imgBack_ = -1;
    messageType_ = -1;
    font_ = -1;

    checkStep_ = 0;
    messageDisplayCnt_ = 0.0f;
    state_ = STATE::NONE;
    selectFile_ = "";
    responder_ = nullptr;

    //状態ごとの処理を登録
    RegisterState(STATE::WAIT, [&]() { UpdateWait(); }, [&]() { DrawWait(); });
    RegisterState(STATE::CHECK_EXPORT, [&]() { UpdateCheckExport(); }, [&]() { DrawCheckExport(); });
    RegisterState(STATE::CHECK_IMPORT, [&]() { UpdateCheckImport(); }, [&]() { DrawCheckImport(); });

    //メッセージを設定
    messages_[static_cast<int>(MESSAGE_TYPE::IMPORT)] = "今配置してるアイテムは消えますがよろしいですか？";
    messages_[static_cast<int>(MESSAGE_TYPE::EXPORT)] = "データを保存しますか？";
    messages_[static_cast<int>(MESSAGE_TYPE::YES)] = "はい";
    messages_[static_cast<int>(MESSAGE_TYPE::NO)] = "いいえ";
    messages_[static_cast<int>(MESSAGE_TYPE::REPORT_EXPORT)] = "ファイルに保存されました";
    messages_[static_cast<int>(MESSAGE_TYPE::REPORT_IMPORT)] = "ファイルをインポートしました";

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

    //回答を返す
    responder_ = std::make_unique<YesNoResponder>();
    responder_->Load();
}

void MapDataIO::Init()
{
    //メッセージ表示カウントを初期化
    messageDisplayCnt_ = 0.0f;

    //初期化
    responder_->Init();

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
    //メッセージカウントを設定
    messageDisplayCnt_ = MES_DISPLAY_TIME;

    //表示メッセージを設定
    messageType_ = static_cast<int>(MESSAGE_TYPE::REPORT_EXPORT);

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

    //メッセージカウントを設定
    messageDisplayCnt_ = MES_DISPLAY_TIME;

    //表示メッセージを設定
    messageType_ = static_cast<int>(MESSAGE_TYPE::REPORT_IMPORT);

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
    const Vector2 rightPos = { ICON_SIZE_X * 2, 0 };
    const Vector2 leftDown = { ICON_SIZE_X * 3, ICON_SIZE_Y };

    //特定のキーを押す、もしくはUIをクリックしたら処理を実行する
    if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::EXPORT_FILE, KeyConfig::JOYPAD_NO::PAD1) || 
        ins.IsTrgDown(KeyConfig::CONTROL_TYPE::EXPORT_FILE_CLICK, KeyConfig::JOYPAD_NO::PAD1) && 
       ( Utility::IsPointInRect(ins.GetMousePos(), rightPos, leftDown) || Utility::IsPointInRect(padCursolPos_, rightPos, leftDown)))
    {
        return true;
    }

    return false;
}

inline bool MapDataIO::IsTriggerImport() const
{
    KeyConfig& ins = KeyConfig::GetInstance();

    //アイコンクリック用
    const Vector2 rightPos = { ICON_SIZE_X, 0 };
    const Vector2 leftDown = { ICON_SIZE_X * 2, ICON_SIZE_Y };

    if (Utility::IsPointInRect(padCursolPos_, rightPos, leftDown))
    {
        int x = 0;
    }

    //特定のキーを押す、もしくはUIをクリックしたら処理を実行する
    if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::IMPORT_FILE, KeyConfig::JOYPAD_NO::PAD1) ||
        ins.IsTrgDown(KeyConfig::CONTROL_TYPE::IMPORT_FILE_CLICK, KeyConfig::JOYPAD_NO::PAD1) &&
        (Utility::IsPointInRect(ins.GetMousePos(), rightPos, leftDown) || Utility::IsPointInRect(padCursolPos_, rightPos, leftDown)))
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
        SetMouseDispFlag(true);
        selectFile_ = Utility::ShowSaveJsonDialog();
        if (selectFile_.empty())
        {
            // キャンセルされた
            SetMouseDispFlag(false);
            return;
        }
        SetMouseDispFlag(false);

        //リセット
        responder_->Reset();

        //確認へ移る
        ChangeState(STATE::CHECK_EXPORT);
        return;
    }

    else if (IsTriggerImport())
    {
        SetMouseDispFlag(true);
        //ファイルを読み込む
        if (!ReadFileBool(selectFile_))
        {
            SetMouseDispFlag(false);
            //読み込まない場合処理を終える
            return;
        }

        //リセット
        responder_->Reset();

        //確認へ移る
        ChangeState(STATE::CHECK_IMPORT);

        SetMouseDispFlag(false);
        return;
    }

    if (messageDisplayCnt_ <= 0.0f) { return; }
    messageDisplayCnt_ -= SceneManager::GetInstance().GetDeltaTime();
}

void MapDataIO::UpdateCheckExport()
{  
    YesNoResponder::RESPON res = responder_->GetRespon();
    if (res == YesNoResponder::RESPON::NONE)
    {
        //選択処理
        responder_->Update();
        return;
    }
    else if (res == YesNoResponder::RESPON::YES)
    {
        //現在の配置データを出力する
        ExportJsonFile(selectFile_);

        //状態遷移
        ChangeState(STATE::WAIT);
        return;
    }
    else
    {
        //状態遷移
        ChangeState(STATE::WAIT);
        return;
    }
}

void MapDataIO::UpdateCheckImport()
{
    YesNoResponder::RESPON res = responder_->GetRespon();
    if (res == YesNoResponder::RESPON::NONE)
    {
        //選択処理
        responder_->Update();
        return;
    }
    else if (res == YesNoResponder::RESPON::YES)
    {
   
        //外部データを読み込む    
        ImportJsonFile();

        //状態遷移
        ChangeState(STATE::WAIT);
        return;
    }
    else
    {
        //状態遷移
        ChangeState(STATE::WAIT);
        return;
    }
}

void MapDataIO::DrawWait()
{
    DrawRotaGraph(
        ICON_SIZE_X + ICON_SIZE_X / 2,
        ICON_SIZE_Y / 2,
        1.0f,
        0.0f,
        imgLoad_,
        true,
        false
     );

    DrawRotaGraph(
        ICON_SIZE_X * 2 + ICON_SIZE_X / 2,
        ICON_SIZE_Y / 2,
        1.0f,
        0.0f,
        imgSave_,
        true,
        false
    );
    if (messageDisplayCnt_ <= 0.0f) { return; }
    constexpr int MESSAGE_POS_X = 10;
    constexpr int MESSAGE_POS_Y = Application::SCREEN_SIZE_Y - 145;
    DrawFormatStringToHandle(
        MESSAGE_POS_X,
        MESSAGE_POS_Y,
        Utility::CYAN,
        font_,
        messages_[messageType_].c_str());
}

void MapDataIO::DrawCheckExport()
{
    //確認画面の描画
    responder_->Draw();

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
        Utility::BLUE,
        font_,
        mes.c_str());
}

void MapDataIO::DrawCheckImport()
{
    //確認画面の描画
    responder_->Draw();

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
        Utility::BLUE,
        exportFont_,
        mes.c_str());
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