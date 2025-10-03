#pragma once
#include<DxLib.h>
#include<memory>
#include"../Manager/System/DateBank.h"      //これ消したい
#include"../Manager/System/KeyConfig.h"
#include"../Manager/System/SceneManager.h"  //これ消したい
#include"../Manager/System/InputManager.h"  //これ消したい

//#define DEBUG_ON

class PlayerBase;

class PlayerInput
{

public:
 
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
    //コンストラクタ
    PlayerInput(KeyConfig::JOYPAD_NO _padNum, KeyConfig::TYPE _cntl);
    ~PlayerInput(void) = default;

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name=""></param>
    void Init(void);

    /// <summary>
    /// 更新
    /// </summary>
    /// <param name=""></param>
    void Update(void);

    /// <summary>
    /// コントロール判定
    /// </summary>
    /// <param name="_actCntl">どの入力を知りたいか</param>
    /// <returns>その入力がされたか</returns>
    bool CheckAct(ACT_CNTL _actCntl) { return actCntl_ == _actCntl ? true : false; }

    /// <summary>
    /// 方向角度
    /// </summary>
    /// <param name=""></param>
    /// <returns>方向角度</returns>
    float GetMoveDeg(void) { return moveDeg_; }
    /// <summary>
    /// 方向(どの方向に向いてるか)
    /// </summary>
    /// <param name=""></param>
    /// <returns>方向(どの方向に向いてるか)</returns>
    VECTOR GetDir(void) { return moveDir_; }

private:

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

    //角度関連
    static constexpr float FLONT_DEG = 0.0f;            //前
    static constexpr float FLONTRIGHT_DEG = 45.0f;      //右斜め前
    static constexpr float RIGHT_DEG = 90.0f;           //右
    static constexpr float BACKRIGHT_DEG = 135.0f;      //右斜め後ろ
    static constexpr float BACK_DEG = 180.0f;           //後ろ
    static constexpr float BACKLEFT_DEG = 225.0f;       //左斜め後ろ
    static constexpr float LEFT_DEG = 270.0f;           //左
    static constexpr float FLONTLEFT_DEG = 315.0f;      //左斜め前
    static constexpr float DELTA_TIME = 1.0f / 60.0f;    //デルタタイム

    //状態更新
    std::map<KeyConfig::TYPE, std::function<void(void)>>inputUpdates_;

    //パッドとキーボードの入力を受け付ける
    void InputAll(void);
    //パッドのみ
    void InputPad(void);
 
    //移動入力関連
    void KeyMove(void);    //キーボード
    void PadMove(void);    //パッド

    //移動入力にDash入力があったかどうかを返す
    const bool IsDashInputDuringMove(void);
    //Lスティックが入力閾値以上に傾いているか判定する
    const bool IsStickMoved(void);

    //操作管理用
    ACT_CNTL actCntl_;

    //スティックの角度
    float leftStickX_;          //スティックの角度X
    float leftStickY_;          //スティックの角度Y

    //スティックの大きさ
    Vector2 LStickAngleSize_;   
    //パッドのスティックの角度
    float stickDeg_;            
    float moveDeg_;             //移動方向
    VECTOR moveDir_;            //移動方向

    KeyConfig::TYPE cntl_;      //入力デバイス
    KeyConfig::JOYPAD_NO padNum_;//パッド番号




};

