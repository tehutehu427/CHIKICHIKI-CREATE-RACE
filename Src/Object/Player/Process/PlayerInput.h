#pragma once
#include<DxLib.h>
#include<memory>
#include"../Manager/System/DateBank.h"
#include"../Manager/System/SceneManager.h"
#include"../Manager/System/InputManager.h"

#define DEBUG_ON
class PlayerBase;
class PlayerInput
{
public:
    //*************************************************
//各アクション操作ボタン
//*************************************************
//移動
    static constexpr int MOVE_FRONT_KEY = KEY_INPUT_W;
    static constexpr int MOVE_LEFT_KEY = KEY_INPUT_A;
    static constexpr int MOVE_BACK_KEY = KEY_INPUT_S;
    static constexpr int MOVE_RIGHT_KEY = KEY_INPUT_D;

    //移動判定になるスティックの大きさ
    static constexpr int STICK_MOVE_SIZE_MIN = 200;

    //パンチ
    static constexpr int PUNCH_KEY = KEY_INPUT_RETURN;
    static constexpr KeyConfig::JOYPAD_BTN PUNCH_BTN = KeyConfig::JOYPAD_BTN::RIGHTBUTTON_LEFT;

    //ジャンプ
    static constexpr int JUMP_KEY = KEY_INPUT_SPACE;
    static constexpr KeyConfig::JOYPAD_BTN JUMP_BTN = KeyConfig::JOYPAD_BTN::RIGHTBUTTON_RIGHT;

    static constexpr float STICK_MARGIN_DEG = 90.0f;

    //角度関連
    static constexpr float FLONT_DEG = 0.0f;
    static constexpr float LEFT_DEG = 270.0f;
    static constexpr float BACK_DEG = 180.0f;
    static constexpr float RIGHT_DEG = 90.0f;

    //デルタタイム
    static constexpr float DELTA_TIME = 1.0f / 60.0f;

    //アクションボタンの種類
    enum class ACT_CNTL
    {
        NONE    //何もしていないとき
        , MOVE   //移動
        , DASHMOVE   //ダッシュ
        , PUNCH  //パンチ
        , JUMP   //ジャンプ
    };

    enum class CNTL
    {
        NONE,
        KEYBOARD,
        PAD
    };
    //シングルトン化するために外部で生成されないようにする
    PlayerInput(KeyConfig::JOYPAD_NO _padNum, KeyConfig::TYPE _cntl);
    ~PlayerInput(void) = default;

    //変更時の初期化
    void Init(void);

    void Update(void);

    //コントロール判定
    bool CheckAct(ACT_CNTL _actCntl) { return actCntl_ == _actCntl ? true : false; }
#ifdef _DEBUG
#endif // _DEBUG_ON

 

    //ゲッタ
    //----------------------------------------------------------------
    ACT_CNTL GetAct(void) { return actCntl_; }
    float GetStickDeg(void){ return stickDeg_; }
    float GetMoveDeg(void) { return moveDeg_; }
    VECTOR GetDir(void) { return moveDir_; }
private:
    //メンバ関数
    void InputKeyBoard(void);
    void InputAll(void);
    void InputPad(void);

    void KeyBoard(void);
    void Pad(void);

    //メンバ変数
    //-----------------------------------------------------------------------
    //操作管理用
    ACT_CNTL actCntl_;

    float leftStickX_;          //スティックの角度X
    float leftStickY_;          //スティックの角度Y

    //スティックの角度(0～1000を返す)
    Vector2 LStickAngleSize_;



    float stickDeg_;            //パッドのスティックの角度
    float moveDeg_;             //移動方向
    VECTOR moveDir_;            //移動方向

    KeyConfig::TYPE cntl_;                 //入力デバイス
    KeyConfig::JOYPAD_NO padNum_;                //パッド番号

    std::map<KeyConfig::TYPE, std::function<void(void)>>inputUpdates_;


};

