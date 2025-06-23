#include "YesNoResponder.h"
#include "../../Application.h"
#include "../../Common/FontRegistry.h"
#include "../../Manager/System/ResourceManager.h"
#include "../../Manager/System/KeyConfig.h"
#include "../../Utility/Utility.h"

namespace
{
    constexpr int MARGIN = 200;             //間   
    const Vector2 OFFSET_POS = { 150, 40 }; //座標調整用
    const Vector2 YES_POS = {
            Application::SCREEN_HALF_X - OFFSET_POS.x,
            Application::SCREEN_HALF_Y + OFFSET_POS.y,
    };
}

YesNoResponder::YesNoResponder()
{
	index_ = 0;
    font_ = 0;
    imgBack_ = 0;
    respon_ = RESPON::NONE;
    messages_ =
    {
        "はい",
        "いいえ"
    };
}

YesNoResponder::~YesNoResponder()
{
    DeleteFontToHandle(font_);
}

void YesNoResponder::Load()
{
    font_ = CreateFontToHandle(FontRegistry::BOKUTATI.c_str(), FONT_SIZE, 0);
}

void YesNoResponder::Init()
{
    Reset();
}

void YesNoResponder::Update()
{
	KeyConfig& key = KeyConfig::GetInstance();

    //コマンドの選択
    if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_LEFT, KeyConfig::JOYPAD_NO::PAD1) ||
        key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_RIGHT, KeyConfig::JOYPAD_NO::PAD1))
    {
        //増減方向
        int dir = key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_RIGHT, KeyConfig::JOYPAD_NO::PAD1) ? 1 : -1;

        //ステップ更新
        index_ += dir;

        //範囲の巻き戻し
        if (index_ < 0)
        {
            index_ = static_cast<int>(RESPON::MAX) - 1;
        }
        else if (index_ >= static_cast<int>(RESPON::MAX))
        {
            index_ = 0;
        }
        return;
    }

    //決定
    else if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::DECISION_KEY_AND_PAD, KeyConfig::JOYPAD_NO::PAD1))
    {
        //選択した内容の処理を行う
        if (index_ == static_cast<int>(RESPON::YES))
        {
            respon_ = RESPON::YES;
            return;
        }
        else
        {
            respon_ = RESPON::NO;
            return;
        }
    }

    //クリックでも選べる
    for (int i = 0; i < static_cast<int>(RESPON::MAX); i++)
    {
        //座標設定
        Vector2 leftTop = { YES_POS.x + MARGIN * i,YES_POS.y };
        Vector2 rightBottom = { static_cast<int>(leftTop.x + messages_[i].length() * FONT_SIZE),
            YES_POS.y + FONT_SIZE };

        //マウスが範囲内か調べる
        if (Utility::IsPointInRect(key.GetMousePos(), leftTop, rightBottom))
        {
            //ステップを設定
            index_ = i;

            //クリックしたか調べる
            if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::DECISION_MOUSE, KeyConfig::JOYPAD_NO::PAD1))
            {
                //選択した内容の処理を行う
                if (index_ == static_cast<int>(RESPON::YES))
                {
                    respon_ = RESPON::YES;
                    return;
                }
                else
                {
                    respon_ = RESPON::NO;
                    return;
                }
            }
            //処理終了
            return;
        }
    }
}

void YesNoResponder::Draw()
{
    constexpr int ALPHA = 128;    
    const Vector2 OFFSET_POS = { 150, 40 };
    constexpr int MARGIN = 200;
    
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, ALPHA);
    DrawExtendGraph(0, 0,
        Application::SCREEN_SIZE_X,
        Application::SCREEN_SIZE_Y,
        imgBack_,
        true);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    //選択コマンドの描画
    std::string commandMes[static_cast<int>(RESPON::MAX)] = 
    {
        messages_[static_cast<int>(RESPON::YES)],
        messages_[static_cast<int>(RESPON::NO)]
    };

    for (int i = 0; i < static_cast<int>(RESPON::MAX); i++)
    {
        //選択しているのを色を変える
        int fontColor = Utility::BLUE;  //初期カラー 
        if (i == index_) { fontColor = Utility::RED; }

        //コマンドメッセージを描画
        DrawFormatStringToHandle(
            Application::SCREEN_HALF_X - OFFSET_POS.x + i * MARGIN,
            Application::SCREEN_HALF_Y + OFFSET_POS.y,
            fontColor,
            font_,
            commandMes[i].c_str()
        );
    }
}

void YesNoResponder::Reset()
{
    index_ = 0;
    respon_ = RESPON::NONE;
}
