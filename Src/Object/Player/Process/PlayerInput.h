#pragma once
#include<DxLib.h>
#include<memory>
#include"../Manager/System/SceneManager.h"
#include"../Manager/System/InputManager.h"
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

    //パンチ
    static constexpr int PUNCH_KEY = KEY_INPUT_RETURN;
    static constexpr InputManager::JOYPAD_BTN PUNCH_BTN = InputManager::JOYPAD_BTN::LEFT;

    //ジャンプ
    static constexpr int JUMP_KEY = KEY_INPUT_E;
    static constexpr InputManager::JOYPAD_BTN JUMP_BTN = InputManager::JOYPAD_BTN::RIGHT;


    ////回避
    //static constexpr int DODGE_KEY = KEY_INPUT_N;
    //static constexpr InputManager::JOYPAD_BTN DODGE_BTN = InputManager::JOYPAD_BTN::LEFT;

    //アクションボタンの種類
    enum class ACT_CNTL
    {
        NONE    //何もしていないとき
        , MOVE   //移動
        , PUNCH
        , JUMP
    };

    enum class CNTL
    {
        NONE,
        KEYBOARD,
        PAD
    };

    //インスタンスの生成
    static void CreateInstance(void);
    static PlayerInput& GetInstance(void);

    
    void Update(InputManager::JOYPAD_NO _padNum,CNTL _cntl);




    //コントロール判定
    bool CheckAct(ACT_CNTL _actCntl) { return actCntl_ == _actCntl ? true : false; }

    //ゲッタ
    //----------------------------------------------------------------
    ACT_CNTL GetAct(void) { return actCntl_; }
    float GetStickDeg(void){ return stickDeg_; }
    float GetMoveDeg(void) { return moveDeg_; }

private:
    //メンバ関数
    void InputKeyBoard();
    void InputPad(InputManager::JOYPAD_NO _padNum);

    //メンバ変数
    //-----------------------------------------------------------------------
    //操作管理用
    ACT_CNTL actCntl_;

    //静的インスタンス生成
    static PlayerInput* playerInput_;

    float leftStickX_;
    float leftStickY_;
    float stickDeg_;            //パッドのスティックの角度
    float moveDeg_;             //移動方向

    //シングルトン化するために外部で生成されないようにする
    PlayerInput(void);
    PlayerInput(const PlayerInput& instance_) = default;
    ~PlayerInput(void) = default;
};

