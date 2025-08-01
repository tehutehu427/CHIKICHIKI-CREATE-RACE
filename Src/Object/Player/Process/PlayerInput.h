#pragma once
#include<DxLib.h>
#include<memory>
#include"../Manager/System/DateBank.h"
#include"../Manager/System/SceneManager.h"
#include"../Manager/System/InputManager.h"

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

    //変更時の初期化
    void Init(void);

    //更新
    void Update(void);

    //コントロール判定
    bool CheckAct(ACT_CNTL _actCntl) { return actCntl_ == _actCntl ? true : false; }
#ifdef _DEBUG
#endif // _DEBUG_ON

 

    //ゲッタ
    //----------------------------------------------------------------
    //方向角度
    float GetMoveDeg(void) { return moveDeg_; }
    //方向(どの方向に向いてるか)
    VECTOR GetDir(void) { return moveDir_; }
private:
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

    //角度関連
    //前
    static constexpr float FLONT_DEG = 0.0f;
    //右斜め前
    static constexpr float FLONTRIGHT_DEG = 45.0f;
    //右
    static constexpr float RIGHT_DEG = 90.0f;
    //右斜め後ろ
    static constexpr float BACKRIGHT_DEG = 135.0f;
    //後ろ
    static constexpr float BACK_DEG = 180.0f;
    //左斜め後ろ
    static constexpr float BACKLEFT_DEG = 225.0f;
    //左
    static constexpr float LEFT_DEG = 270.0f;
    //左斜め前
    static constexpr float FLONTLEFT_DEG = 315.0f;
    //デルタタイム
    static constexpr float DELTA_TIME = 1.0f / 60.0f;

    //メンバ関数
    //キーボードのみ
    void InputKeyBoard(void);
    //パッドとキーボードのｎ入力を受け付ける
    void InputAll(void);
    //パッドのみ
    void InputPad(void);
    
    //キーボード入力関連
    //移動
    void KeyMove(void);
    //ダッシュ
    void KeyDash(void);
    //パンチ
    void KeyPanch(void);
    //ジャンプ
    void KeyJump(void);


    //移動入力にDash入力があったかどうかを返す
    const bool IsDashInputDuringMove(void);

    //Lスティックが入力閾値以上に傾いているか判定する
    const bool IsStickMoved(void);

    //メンバ変数
    //-----------------------------------------------------------------------
    //操作管理用
    ACT_CNTL actCntl_;

    float leftStickX_;          //スティックの角度X
    float leftStickY_;          //スティックの角度Y


    Vector2 LStickAngleSize_;   //スティックの大きさ


    float stickDeg_;            //パッドのスティックの角度
    float moveDeg_;             //移動方向
    VECTOR moveDir_;            //移動方向

    KeyConfig::TYPE cntl_;      //入力デバイス
    KeyConfig::JOYPAD_NO padNum_;//パッド番号

    //状態更新
    std::map<KeyConfig::TYPE, std::function<void(void)>>inputUpdates_;


};

