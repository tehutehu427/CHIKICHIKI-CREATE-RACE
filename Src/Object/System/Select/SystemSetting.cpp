#include "SystemSetting.h"
#include <DxLib.h>
#include "../../../Manager/System/ResourceManager.h"
#include "../../../Manager/System/SceneManager.h"
#include "../../../Manager/System/KeyConfig.h"
#include "../../../Manager/System/DateBank.h"
#include "../../../Manager/System/SoundManager.h"
#include "../../../Utility/Utility.h"
#include "../Scene/SelectScene.h"

namespace
{
	int startPosY = 100; //項目の開始Y座標
	int heightMargin = 80; //項目間の高さのマージン
}

SystemSetting::SystemSetting()
{
	//初期化
	stateIndex_ = -1;
	clearScore_ = -1;
	soundVolume_ = -1;
	isSkip_ = false;

	//画像の初期化
	imgTriangle_ = -1;
	imgCursor_ = nullptr;		//カーソル画像
	imgSelectMess_ = nullptr;	//メッセージ
	imgSettings_ = nullptr;		//設定項目
	imgNumbers_ = nullptr;		//ナンバー画像

	//ルール項目別右の処理の登録
	rightStateMap_ =
	{
		{STATE::CLEAR_SCORE,[this]()
		{
			//右キーで選択をひとつ進める（範囲内でループ）
			clearScore_ = (clearScore_ + 1) % CLEAR_SCORE_MAX;
		}},
		{STATE::SKIP,[this]()
		{
			isSkip_ = !isSkip_;
		}},
		{STATE::SOUND_VOLUME,[this]()
		{
			//右キーで選択をひとつ進める（範囲内でループ）
			soundVolume_ = (soundVolume_ + SOUND_VOLUME_STEP) % SOUND_VOLUME_MAX;
		}}
	};

	//ルール項目別左の処理の登録
	leftStateMap_ =
	{
		{STATE::CLEAR_SCORE,[this]()
		{
			//左キーで選択をひとつ戻す（範囲内でループ）
			clearScore_ = (clearScore_ - 1 + CLEAR_SCORE_MAX) % CLEAR_SCORE_MAX;
		}},
		{STATE::SKIP,[this]()
		{
			isSkip_ = !isSkip_;
		}},
		{STATE::SOUND_VOLUME,[this]()
		{
			//左キーで選択をひとつ戻す（範囲内でループ）
			soundVolume_ = (soundVolume_ - SOUND_VOLUME_STEP + SOUND_VOLUME_MAX) % SOUND_VOLUME_MAX;
}		}
	};
}

SystemSetting::~SystemSetting()
{
}

void SystemSetting::Load()
{
	ResourceManager& res = ResourceManager::GetInstance();
	imgCursor_ = res.Load(ResourceManager::SRC::CURSORS).handleIds_;
	imgTriangle_ = res.Load(ResourceManager::SRC::SCROLL_ARROW_ICON).handleId_;
	imgSelectMess_ = res.Load(ResourceManager::SRC::SELECT_MESSAGES).handleIds_;
	//imgSettings = res.Load(ResourceManager::SRC::SYSTEM_SETTINGS).handleIds_;
	imgNumbers_ = res.Load(ResourceManager::SRC::NUMBERS).handleIds_;
}

void SystemSetting::Init()
{	
	//初期状態
	stateIndex_ = 0;
	clearScore_ = DateBank::DEFAULT_SCORE;
	soundVolume_ = SoundManager::DEFAULT_VOLUME;
}

void SystemSetting::Update(SelectScene& _parent)
{
	KeyConfig& key = KeyConfig::GetInstance();

	//決定
	if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::DECISION_KEY_AND_PAD, KeyConfig::JOYPAD_NO::PAD1))
	{
		//データの反映
		ApplyData();
		return;
	}
	//上へ
	else if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_UP, KeyConfig::JOYPAD_NO::PAD1))
	{
		//上キーで選択をひとつ進める（範囲内でループ）
		stateIndex_ = (stateIndex_ + 1) % STATE_MAX;
		return;
	}
	//下へ
	else if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_DOWN, KeyConfig::JOYPAD_NO::PAD1))
	{
		//下キーで選択をひとつ戻す（範囲内でループ）
		stateIndex_ = (stateIndex_ - 1 + STATE_MAX) % STATE_MAX;
		return;
	}
	//右
	else if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_RIGHT, KeyConfig::JOYPAD_NO::PAD1))
	{
		rightStateMap_[static_cast<STATE>(stateIndex_)](); //右の処理を実行
		return;
	}
	//左
	else if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_LEFT, KeyConfig::JOYPAD_NO::PAD1))
	{
		leftStateMap_[static_cast<STATE>(stateIndex_)](); //左の処理を実行
		return;
	}
	//メニューに戻る
	else if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::CANCEL, KeyConfig::JOYPAD_NO::PAD1))
	{
		_parent.ChangeState(SelectScene::STATE::SELECT_MENU); //メニューに戻る
		return;
	}
}

void SystemSetting::Draw()
{
	//矢印の描画
	DrawCursor();

	//三角の描画
	DrawSelectTriangle();

	//項目名の描画
	for (int i = 0; i < STATE_MAX; ++i)
	{
		int alpha = Utility::ALPHA_MAX / 2; //半透明
		if (i == stateIndex_)
		{
			alpha = Utility::ALPHA_MAX; //選択中は最大
		}

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);		//透過させる
		DrawSelectStateName(i);								//項目名の描画
		drawStateMap_[static_cast<STATE>(i)]();				//項目別の描画処理を呼び出す
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);			//透過を解除
		
	}

}

void SystemSetting::ApplyData()
{
	DateBank& data = DateBank::GetInstance();
	SoundManager& sound = SoundManager::GetInstance();

	//クリアスコアの設定
	data.SetMultiClearScore(clearScore_);

	//スキップ設定の反映
	//data.SetSkip(isSkip_);

	//サウンドボリュームの設定
	sound.SetVolume(soundVolume_);
}

void SystemSetting::DrawCursor()
{
	constexpr int POS_X = 50;	//カーソルのX座標
	const int posY = startPosY + stateIndex_ * heightMargin; //カーソルのY座標（項目の開始Y座標 + 選択項目のインデックス * 項目間隔）

	DrawRotaGraph(
		POS_X,
		posY,
		1.0f,
		0.0f,
		imgCursor_[0],
		true
	);
}

void SystemSetting::DrawSelectTriangle()
{	
	constexpr int DRAW_NUM = 2;	//描画する数
	constexpr int POS_X = 650;	//カーソルのX座標
	const int posY = startPosY + stateIndex_ * heightMargin; //カーソルのY座標（項目の開始Y座標 + 選択項目のインデックス * 項目間隔）

	for (int i = 0; i < DRAW_NUM; i++)
	{
		//矢印の描画
		DrawRotaGraph(
			POS_X,
			posY,
			1.0f,
			0.0f,
			imgTriangle_,
			true);
	}
}

void SystemSetting::DrawClearScore()
{
	constexpr int POS_X = 200; //X座標
	constexpr int MARGIN_X = 50; //数字の間隔	
	constexpr int OFFSET_X = 50; //数字の位置を調整
	const int posY = startPosY + static_cast<int>(STATE::CLEAR_SCORE) * heightMargin; //Y座標（項目の開始Y座標 + 選択項目のインデックス * 項目間隔）
	const int numberDigit = Utility::GetDigitCount(clearScore_); //数字の桁数

	//数字の描画
	for (int i = 0; i < numberDigit; i++)
	{
		int index = Utility::GetDigit(clearScore_, i);

		DrawRotaGraph(
			POS_X + i * MARGIN_X - OFFSET_X,
			posY,
			1.0f,
			0.0f,
			imgNumbers_[index],
			true,
			false
		);
	};
}

void SystemSetting::DrawSkip()
{
	constexpr int POS_X = 200; //X座標
	const int posY = startPosY + static_cast<int>(STATE::SKIP) * heightMargin; //Y座標（項目の開始Y座標 + 選択項目のインデックス * 項目間隔）

	//スキップ設定の描画
	DrawRotaGraph(
		POS_X,
		posY,
		1.0f,
		0.0f,
		imgSelectMess_[isSkip_ ? 0 : 1],
		true,
		false
	);
}

void SystemSetting::DrawSoundVolume()
{
	constexpr int POS_X = 200; //X座標
	constexpr int MARGIN_X = 50; //数字の間隔	
	constexpr int OFFSET_X = 90; //数字の位置を調整
	const int posY = startPosY + static_cast<int>(STATE::SOUND_VOLUME) * heightMargin; //Y座標（項目の開始Y座標 + 選択項目のインデックス * 項目間隔）
	const int numberDigit = Utility::GetDigitCount(clearScore_); //数字の桁数

	//数字の描画
	for (int i = 0; i < numberDigit; i++)
	{
		int index = Utility::GetDigit(clearScore_, i);

		DrawRotaGraph(
			POS_X + i * MARGIN_X - OFFSET_X,
			posY,
			1.0f,
			0.0f,
			imgNumbers_[index],
			true,
			false
		);
	};
}

void SystemSetting::DrawSelectStateName(const int _index)
{
	constexpr int POS_X = 150; //X座標
	const int posY = startPosY + _index * heightMargin; //Y座標（項目の開始Y座標 + 選択項目のインデックス * 項目間隔）

	//設定項目の描画
	DrawGraph(
		POS_X,
		posY,
		imgSettings_[_index],
		true
	);
}
