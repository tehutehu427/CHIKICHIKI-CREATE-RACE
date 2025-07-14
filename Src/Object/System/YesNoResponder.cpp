#include "YesNoResponder.h"
#include "../../Application.h"
#include "../../Common/FontRegistry.h"
#include "../../Common/IntVector3.h"
#include "../../Manager/System/SoundManager.h"
#include "../../Manager/System/ResourceManager.h"
#include "../../Manager/System/KeyConfig.h"
#include "../../Utility/Utility.h"

namespace
{
    constexpr int MARGIN = 200;                                    //間   
	constexpr int IMG_SIZE_X = ResourceManager::YES_NO_SIZE_X;     //画像のサイズX
	constexpr int IMG_SIZE_Y = ResourceManager::YES_NO_SIZE_Y;     //画像のサイズY
    const Vector2 OFFSET_POS = { 150, 40 }; //座標調整用
    const Vector2 YES_POS = 
    {
            Application::SCREEN_HALF_X - OFFSET_POS.x,
            Application::SCREEN_HALF_Y + OFFSET_POS.y,
    };
}

YesNoResponder::YesNoResponder()
{
	index_ = 0;
    imgs_ = nullptr;
    font_ = 0;
    imgBack_ = 0;
    respon_ = RESPON::NONE;
}

YesNoResponder::~YesNoResponder()
{
    DeleteFontToHandle(font_);
}

void YesNoResponder::Load()
{
    ResourceManager& res = ResourceManager::GetInstance();
    imgBack_ = res.Load(ResourceManager::SRC::EXPLAN_BACK).handleId_;
	imgs_ = res.Load(ResourceManager::SRC::YES_NO).handleIds_;
    font_ = CreateFontToHandle(FontRegistry::BOKUTATI.c_str(), FONT_SIZE, 0);
}

void YesNoResponder::Init()
{
    Reset();
}

void YesNoResponder::Update()
{
	KeyConfig& key = KeyConfig::GetInstance();
	SoundManager& sndMng = SoundManager::GetInstance();

    //コマンドの選択
    if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_LEFT, KeyConfig::JOYPAD_NO::PAD1) ||
        key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_RIGHT, KeyConfig::JOYPAD_NO::PAD1))
    {
        //増減方向
        int dir = key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_RIGHT, KeyConfig::JOYPAD_NO::PAD1) ? 1 : -1;

        //ステップ更新
        index_ += dir;

		sndMng.GetInstance().Play(SoundManager::SRC::CLICK_OBJECT_SE, SoundManager::PLAYTYPE::BACK);

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
            sndMng.GetInstance().Play(SoundManager::SRC::DECISION, SoundManager::PLAYTYPE::BACK);
            return;
        }
        else
        {
            respon_ = RESPON::NO;
            sndMng.GetInstance().Play(SoundManager::SRC::CANCEL, SoundManager::PLAYTYPE::BACK);
            return;
        }
    }

    //クリックでも選べる
    for (int i = 0; i < static_cast<int>(RESPON::MAX); i++)
    {

        
        //座標設定
		int rev = i == 0 ? -1 : 1; //YESは左側、NOは右側
        Vector2 leftTop = { Application::SCREEN_HALF_X + OFFSET_POS.x * rev - IMG_SIZE_X / 2, Application::SCREEN_HALF_Y + OFFSET_POS.y - IMG_SIZE_Y / 2 };
        Vector2 rightBottom = { Application::SCREEN_HALF_X + OFFSET_POS.x * rev + IMG_SIZE_X / 2, Application::SCREEN_HALF_Y + OFFSET_POS.y + IMG_SIZE_Y / 2 };

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
                    sndMng.GetInstance().Play(SoundManager::SRC::DECISION, SoundManager::PLAYTYPE::BACK);
                    return;
                }
                else
                {
                    respon_ = RESPON::NO;
                    sndMng.GetInstance().Play(SoundManager::SRC::CANCEL, SoundManager::PLAYTYPE::BACK);
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
	constexpr IntVector3 SELECT_COLOR = { 128, 0, 0 }; //選択時の色
    
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, ALPHA);
    DrawExtendGraph(0, 0,
        Application::SCREEN_SIZE_X,
        Application::SCREEN_SIZE_Y,
        imgBack_,
        true);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    for (int i = 0; i < static_cast<int>(RESPON::MAX); i++)
    {
        //選択しているのを色を変える
        int fontColor = Utility::BLUE;  //初期カラー 
        int rev = i == 0 ? -1 : 1;
        if (i == index_) 
        {
            SetDrawAddColor(SELECT_COLOR.x, SELECT_COLOR.y, SELECT_COLOR.z);
        }

        //コマンドメッセージを描画
        DrawRotaGraph(
            Application::SCREEN_HALF_X + OFFSET_POS.x * rev,
            Application::SCREEN_HALF_Y + OFFSET_POS.y,
            1.0f,
            0.0f,
			imgs_[i],
            true
        );

        SetDrawAddColor(0, 0, 0);
    }
}

void YesNoResponder::Reset()
{
    index_ = 0;
    respon_ = RESPON::NONE;
}
