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
#include "../../Manager/System/KeyConfig.h"
#include "../../Manager/System/ResourceManager.h"
#include "../../Manager/System/SoundManager.h"
#include "../../Manager/Game/ItemManager.h"
#include "../../Manager/Game/MapEditer.h"
#include "../System/YesNoResponder.h"
#include "../System/Select/SelectStage.h"

// JSONライブラリのエイリアス
using json = nlohmann::json;

namespace
{
	constexpr int MARGIN = 200;    	 //間
	constexpr int OFFSET_Y = 120; //座標調整用
    const std::string path_json = Application::PATH_JSON;   //パス
}

MapDataIO::MapDataIO(const Vector2& _padCursorPos):
    padCursorPos_(_padCursorPos)
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
	imgSystemMessages_ = nullptr;   
	imgEditMessages_ = nullptr;   

    //状態ごとの処理を登録
    RegisterState(STATE::WAIT, [&]() { UpdateWait(); }, [&]() { DrawWait(); });
    RegisterState(STATE::CHECK_EXPORT, [&]() { UpdateCheckExport(); }, [&]() { DrawCheckExport(); });
    RegisterState(STATE::CHECK_IMPORT, [&]() { UpdateCheckImport(); }, [&]() { DrawCheckImport(); });
    RegisterState(STATE::FINISH, [&]() { UpdateFinish(); }, [&]() { DrawFinish(); });

    //モード別に処理を登録
    RegisterGetFileName(SceneManager::SCENE_ID::FREE, [&]() { return GetFreeFileName(); });
    RegisterGetFileName(SceneManager::SCENE_ID::SOLO, [&]() {return GetSoloFileName(); });
    RegisterGetFileName(SceneManager::SCENE_ID::MULTI, [&]() {return GetMultiFileName(); });
}

MapDataIO::~MapDataIO()
{
    DeleteFontToHandle(exportFont_);
    DeleteFontToHandle(font_);
}

void MapDataIO::Load()
{
    //ファイルパスの指定
    selectFile_ = Application::PATH_JSON + getFileNameMap_[SceneManager::GetInstance().GetSceneID()]();
    ImportJsonFile();

    //画像
    ResourceManager& res = ResourceManager::GetInstance();
    imgLoad_ = res.Load(ResourceManager::SRC::LOAD_ICON).handleId_;
    imgSave_ = res.Load(ResourceManager::SRC::SAVE_ICON).handleId_;
    imgBack_ = res.Load(ResourceManager::SRC::EXPLAN_BACK).handleId_;
	imgEditMessages_ = res.Load(ResourceManager::SRC::EDIT_MESSAGES).handleIds_;
	imgSystemMessages_ = res.Load(ResourceManager::SRC::SYSTEM_MESSAGES).handleIds_;

    //フォント生成
    font_ = CreateFontToHandle(FontRegistry::BOKUTATI.c_str(), FONT_SIZE, 0);
    exportFont_ = CreateFontToHandle(FontRegistry::BOKUTATI.c_str(), EXPORT_FONT_SIZE, 0);

    //回答を返す
    responder_ = std::make_unique<YesNoResponder>();
    responder_->Load();

    SoundManager & sndMng = SoundManager::GetInstance();
	sndMng.LoadResource(SoundManager::SRC::DECISION);               //決定音
	sndMng.LoadResource(SoundManager::SRC::CANCEL);                 //キャンセル音
	sndMng.LoadResource(SoundManager::SRC::CLICK_OBJECT_SE);        //クリック音
	sndMng.LoadResource(SoundManager::SRC::EDIT_SYSTEM_ICON_CLICK); //アイコンのクリック音
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

const bool MapDataIO::IsEdit() const
{
    return state_ == STATE::NONE || state_ == STATE::WAIT;
}

void MapDataIO::Reset()
{
    //ファイルパスの指定
    selectFile_ = Application::PATH_JSON + getFileNameMap_[SceneManager::GetInstance().GetSceneID()]();
    ImportJsonFile();
}

void MapDataIO::ExportJsonFile(const std::string _fileName)
{
    //メッセージカウントを設定
    messageDisplayCnt_ = MES_DISPLAY_TIME;

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
            nlohmann::json rotations = nlohmann::json::array();
            nlohmann::json quaternions = nlohmann::json::array();

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
                    }
                );

                //角度情報
                VECTOR rot = item->GetTransform().rot;
                rotations.push_back(
                    {
                        {"x", rot.x},
                        {"y", rot.y},
                        {"z", rot.z}
                    }
                );

                //クォータニオン情報
                Quaternion quaRot = item->GetTransform().quaRot;
                quaternions.push_back(
                    {
                        {"x", quaRot.x},
                        {"y", quaRot.y},
                        {"z", quaRot.z},
                        {"w", quaRot.w},
                    }
                );


            }

            //名前取得
            std::string typeName = DateBank::GetInstance().GetItemName(type);
            if (!typeName.empty()) // 空文字チェック
            {
                j[typeName] = 
                {
                    {"positions", positions},
                    {"rotations", rotations},
                    {"quaternions", quaternions}
                };
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

    //今あるオブジェクトを削除
    itemMng.AllDeleteItem();
    MapEditer::GetInstance().DeleteAllItem();
    //読み込み
    auto items = LoadItemsFromJson(selectFile_);

    // 読み込んだアイテムを確認
    for (const auto& [type, importData] : items)
    {
        for (int i = 0; i < items[type].positions.size(); i++)
        {
            //マップ座標に変換
            IntVector3 mapPos = MapEditer::GetInstance().WorldToMapPos(items[type].positions[i]);

            //格納
            itemMng.AddItem(mapPos, items[type].quaternions[i], type, Utility::Rad2DegF(items[type].rotations[i].y));
			MapEditer::GetInstance().AddItem(
				{ mapPos, items[type].quaternions[i], type},
                itemMng.GetItemSize(type),
                itemMng.GetItemHitSize(type), 
                Utility::Rad2DegF(items[type].rotations[i].y));
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
       ( Utility::IsPointInRect(ins.GetMousePos(), rightPos, leftDown) || Utility::IsPointInRect(padCursorPos_, rightPos, leftDown)))
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

    //特定のキーを押す、もしくはUIをクリックしたら処理を実行する
    if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::IMPORT_FILE, KeyConfig::JOYPAD_NO::PAD1) ||
        ins.IsTrgDown(KeyConfig::CONTROL_TYPE::IMPORT_FILE_CLICK, KeyConfig::JOYPAD_NO::PAD1) &&
        (Utility::IsPointInRect(ins.GetMousePos(), rightPos, leftDown) || Utility::IsPointInRect(padCursorPos_, rightPos, leftDown)))
    {
        return true;
    }

    return false;
}

std::unordered_map<ItemBase::ITEM_TYPE, MapDataIO::ImportData> MapDataIO::LoadItemsFromJson(const std::string& _filepath)
{
    std::unordered_map<ItemBase::ITEM_TYPE, MapDataIO::ImportData> items = {};

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
            // positions
            if (j[typeName].contains("positions"))
            {
                std::vector<VECTOR> positions;
                for (const auto& pos : j[typeName]["positions"])
                {
                    VECTOR position;
                    position.x = pos["x"].get<float>();
                    position.y = pos["y"].get<float>();
                    position.z = pos["z"].get<float>();
                    positions.push_back(position);
                }
                items[type].positions = positions;
            }

            // rotations
            if (j[typeName].contains("rotations"))
            {
                std::vector<VECTOR> rotations;
                for (const auto& rot : j[typeName]["rotations"])
                {
                    VECTOR rotation;
                    rotation.x = rot["x"].get<float>();
                    rotation.y = rot["y"].get<float>();
                    rotation.z = rot["z"].get<float>();
                    rotations.push_back(rotation);
                }
                items[type].rotations = rotations;
            }

            // quaternions
            if (j[typeName].contains("quaternions"))
            {
                std::vector<Quaternion> quaternions;
                for (const auto& qua : j[typeName]["quaternions"])
                {
                    Quaternion quaternion;
                    quaternion.x = qua["x"].get<float>();
                    quaternion.y = qua["y"].get<float>();
                    quaternion.z = qua["z"].get<float>();
                    quaternion.w = qua["w"].get<float>();
                    quaternions.push_back(quaternion);
                }
                items[type].quaternions = quaternions;
            }
        }
    }

    return items;
}

std::string MapDataIO::GetFreeFileName()
{
    return "DefaultStage.json";
}

std::string MapDataIO::GetSoloFileName()
{
    //配列を生成
    const std::string stages[static_cast<int>(SelectStage::STAGE_TYPE::MAX)] =
    {
        "BeginnerStage.json",
        "IntermediateStage.json",
        "AdvancedStage.json",
        "AbyssStage.json"
    };

    //選択したステージを取得
    int selectNum = DateBank::GetInstance().GetStageNo();
 
    //値を返す
    return stages[selectNum];
}

std::string MapDataIO::GetMultiFileName()
{
    //配列を生成
    const std::string stages[MULTI_STAGE_TYPES] =
    {
        "MultiStage1.json",
        "MultiStage2.json",
        "MultiStage3.json",
        "MultiStage4.json"
    };

    //ランダム値を取得
    int randNum = GetRand(MULTI_STAGE_TYPES - 1);
    
    //値を返す
    return stages[randNum];
}

void MapDataIO::RegisterGetFileName(const SceneManager::SCENE_ID _sceneId, std::function<std::string()> _func)
{
    getFileNameMap_[_sceneId] = _func;
}

void MapDataIO::RegisterState(const STATE _state, std::function<void()> _update, std::function<void()> _draw)
{
    stateMap_[_state] = StateFuncs{ _update, _draw };
}

void MapDataIO::UpdateWait()
{
    KeyConfig& ins = KeyConfig::GetInstance();
	SoundManager& sndMng = SoundManager::GetInstance();

    //特定のキーを押す、もしくはUIをクリックしたら処理を実行する
    if (IsTriggerExport())
    {
		sndMng.Play(SoundManager::SRC::EDIT_SYSTEM_ICON_CLICK, SoundManager::PLAYTYPE::BACK);
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
        sndMng.Play(SoundManager::SRC::EDIT_SYSTEM_ICON_CLICK, SoundManager::PLAYTYPE::BACK);
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

        //画像用インデックス設定
        systemMessageIndex_ = IMG_EXPORT_INDEX;

        //状態遷移
        ChangeState(STATE::FINISH);
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

        //画像用インデックス設定
        systemMessageIndex_ = IMG_IMPORT_INDEX;

        //状態遷移
        ChangeState(STATE::FINISH);
        return;
    }
    else
    {
        //状態遷移
        ChangeState(STATE::WAIT);
        return;
    }
}

void MapDataIO::UpdateFinish()
{
	KeyConfig& ins = KeyConfig::GetInstance();

    //特定のキーを押す、もしくはUIをクリックしたら処理を実行する
    if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
    {
		SoundManager::GetInstance().Play(SoundManager::SRC::DECISION, SoundManager::PLAYTYPE::BACK);
        ChangeState(STATE::WAIT);
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
}

void MapDataIO::DrawCheckExport()
{
    //確認画面の描画
    responder_->Draw();

    //メッセージの描画
    DrawRotaGraph(
        Application::SCREEN_HALF_X,
        Application::SCREEN_HALF_Y - OFFSET_Y,
        0.7f,
        0.0f,
        imgEditMessages_[IMG_EXPORT_INDEX],
        true);
}

void MapDataIO::DrawCheckImport()
{
    //確認画面の描画
    responder_->Draw();

    //メッセージの描画
    DrawRotaGraph(
        Application::SCREEN_HALF_X,
        Application::SCREEN_HALF_Y - OFFSET_Y,
        0.7f,
        0.0f,
        imgEditMessages_[IMG_IMPORT_INDEX],
        true);
}

void MapDataIO::DrawFinish()
{
    //メッセージの描画
    DrawRotaGraph(
        Application::SCREEN_HALF_X,
        Application::SCREEN_HALF_Y,
        2.0f,
        0.0f,
        imgSystemMessages_[systemMessageIndex_],
        true);
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