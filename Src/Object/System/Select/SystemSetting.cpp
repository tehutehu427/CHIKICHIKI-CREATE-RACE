#include "SystemSetting.h"
#include <DxLib.h>
#include "../../../Manager/System/ResourceManager.h"
#include "../../../Manager/System/SceneManager.h"
#include "../../../Manager/System/KeyConfig.h"
#include "../../../Manager/System/DateBank.h"
#include "../../../Manager/System/SoundManager.h"
#include "../../../Utility/Utility.h"
#include "../../../Common/IntVector3.h"
#include "../Scene/SelectScene.h"

namespace
{
	constexpr int START_POS_Y = 150; //項目の開始Y座標
	constexpr int HEIGHT_MARGIN = 80; //項目間の高さのマージン
	constexpr IntVector3 SELECT_ADD_COLOR = { 128,0,0 };	//選択色
}

SystemSetting::SystemSetting()
{
	//初期化
	stateIndex_ = -1;
	clearScore_ = -1;
	soundVolume_ = -1;
	isSkip_ = false;
	updateType_ = UPDATE_TYPE::SELECT;

	//画像の初期化
	imgTitle_ = -1;				//タイトル画像
	imgTriangle_ = -1;			//三角画像
	imgSettingFinish_ = -1;		//設定完了画像
	imgCursor_ = nullptr;		//カーソル画像
	imgSelectMess_ = nullptr;	//メッセージ
	imgSettings_ = nullptr;		//設定項目
	imgNumbers_ = nullptr;		//ナンバー画像
	imgOnOff_ = nullptr;		//オンオフ画像
	imgApplyMes_ = nullptr;		//適用時のメッセージ画像

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

	//描画関数の登録
	drawStateMap_ =
	{
		{STATE::CLEAR_SCORE,[this]()
		{
			DrawClearScore();
		}},
		{STATE::SKIP,[this]()
		{
			DrawSkip();
		}},
		{STATE::SOUND_VOLUME,[this]()
		{
			DrawSoundVolume();
		}}
	};

	//更新関数の登録
	updateMap_ =
	{
		{UPDATE_TYPE::SELECT,[this](SelectScene& _parent)
		{
			UpdateSelect(_parent);
		}},
		{UPDATE_TYPE::APPLY,[this](SelectScene& _parent)
		{
			UpdateApply(_parent);
		}}
	};
}

SystemSetting::~SystemSetting()
{
}

void SystemSetting::Load()
{
	ResourceManager& res = ResourceManager::GetInstance();
	imgTitle_ = res.Load(ResourceManager::SRC::SYSTEM_SETTING).handleId_; 
	imgCursor_ = res.Load(ResourceManager::SRC::CURSORS).handleIds_;
	imgSettingFinish_ = res.Load(ResourceManager::SRC::SETTING_FINISH).handleId_;
	imgTriangle_ = res.Load(ResourceManager::SRC::SCROLL_ARROW_ICON).handleId_;
	imgSelectMess_ = res.Load(ResourceManager::SRC::SELECT_MESSAGES).handleIds_;
	imgSettings_ = res.Load(ResourceManager::SRC::SETTING_ITEMS).handleIds_;
	imgNumbers_ = res.Load(ResourceManager::SRC::NUMBERS).handleIds_;
	imgOnOff_ = res.Load(ResourceManager::SRC::ONOFF).handleIds_;
	imgApplyMes_ = res.Load(ResourceManager::SRC::SYSTEM_MESSAGES).handleIds_;
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
	updateMap_[updateType_](_parent); //更新処理を呼び出す
}

void SystemSetting::Draw()
{
	//タイトルの描画
	DrawTitle();

	//設定完了のメッセージの描画
	DrawSettingFinish();

	//矢印の描画
	DrawCursor();

	//三角の描画
	DrawSelectTriangle();

	//項目名の描画
	for (int i = 0; i < STATE_MAX - 1; ++i)
	{
		if (i == stateIndex_)
		{
			SetDrawAddColor(SELECT_ADD_COLOR.x, SELECT_ADD_COLOR.y, SELECT_ADD_COLOR.z);
		}
		//項目名の描画
		DrawSelectStateName(i);					
		
		//色のリセット
		SetDrawAddColor(0, 0, 0); //色のリセット				
		
		//項目別の描画処理を呼び出す
		drawStateMap_[static_cast<STATE>(i)]();		
		
	}

	//適用完了時のメッセージの描画
	DrawApplyMessage();

}

void SystemSetting::DrawTitle()
{
	//タイトルの描画
	constexpr int POS_Y = 50; //Y座標
	DrawRotaGraph(
		Application::SCREEN_HALF_X,
		POS_Y,
		1.0f,
		0.0f,
		imgTitle_,
		true,
		false
	);
}

void SystemSetting::DrawSettingFinish()
{
	constexpr int POS_X = Application::SCREEN_SIZE_X - 106; //X座標
	constexpr int POS_Y = Application::SCREEN_SIZE_Y - 32;	//Y座標

	if (static_cast<int>(STATE::APPLY) == stateIndex_)
	{
		SetDrawAddColor(SELECT_ADD_COLOR.x, SELECT_ADD_COLOR.y, SELECT_ADD_COLOR.z);
	}

	DrawRotaGraph(
		POS_X,
		POS_Y,
		1.0f,
		0.0f,
		imgSettingFinish_,
		true,
		false
	);
	//色のリセット
	SetDrawAddColor(0, 0, 0); //色のリセット
}

void SystemSetting::UpdateSelect(SelectScene& _parent)
{
	KeyConfig& key = KeyConfig::GetInstance();

	//決定
	if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::DECISION_KEY_AND_PAD, KeyConfig::JOYPAD_NO::PAD1) && stateIndex_ == static_cast<int>(STATE::APPLY))
	{
		updateType_ = UPDATE_TYPE::APPLY; //更新タイプを適用に変更
		return;
	}
	//上へ
	else if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_UP, KeyConfig::JOYPAD_NO::PAD1))
	{
		//下キーで選択をひとつ戻す（範囲内でループ）
		stateIndex_ = (stateIndex_ - 1 + STATE_MAX) % STATE_MAX;
		return;
	}
	//下へ
	else if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_DOWN, KeyConfig::JOYPAD_NO::PAD1))
	{
		//上キーで選択をひとつ進める（範囲内でループ）
		stateIndex_ = (stateIndex_ + 1) % STATE_MAX;
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

void SystemSetting::UpdateApply(SelectScene& _parent)
{
	KeyConfig& key = KeyConfig::GetInstance();
	//決定
	if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::DECISION_KEY_AND_PAD, KeyConfig::JOYPAD_NO::PAD1))
	{
		//データの反映
		ApplyData();

		//更新タイプを選択に戻す
		updateType_ = UPDATE_TYPE::SELECT;
		return;
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
	int posX = POS_X; //カーソルのX座標（初期値）
	int posY = START_POS_Y + stateIndex_ * HEIGHT_MARGIN; //カーソルのY座標（項目の開始Y座標 + 選択項目のインデックス * 項目間隔）

	if (stateIndex_ == static_cast<int>(STATE::APPLY))
	{
		posX = Application::SCREEN_SIZE_X - 250;	//Y座標
		posY = Application::SCREEN_SIZE_Y - 32;	//X座標
	}

	DrawRotaGraph(
		posX,
		posY,
		1.0f,
		Utility::Deg2RadF(90.0f),
		imgCursor_[0],
		true
	);
}

void SystemSetting::DrawSelectTriangle()
{	
	if (stateIndex_ == static_cast<int>(STATE::APPLY))
	{
		return; //適用状態では描画しない
	}

	constexpr int DRAW_NUM = 2;	//描画する数
	constexpr int POS_X = 720;	//カーソルのX座標
	constexpr float RATE = 0.7f;
	const int posY = START_POS_Y + stateIndex_ * HEIGHT_MARGIN; //カーソルのY座標（項目の開始Y座標 + 選択項目のインデックス * 項目間隔）

	for (int i = 0; i < DRAW_NUM; i++)
	{
		int posX = POS_X;
		float angle = 270.0f;
		if (i == 1)
		{
			posX += 250; //左側の位置調整
			angle = 90.0f; //左側の矢印は90度回
		}


		//矢印の描画
		DrawRotaGraph(
			posX,
			posY,
			RATE,
			Utility::Deg2RadF(angle),
			imgTriangle_,
			true);
	}
}

void SystemSetting::DrawClearScore()
{
	constexpr int POS_X = 900; //X座標
	constexpr int MARGIN_X = 64 ; //数字の間隔	
	constexpr int OFFSET_X = 90; //数字の位置を調整
	const int posY = START_POS_Y + static_cast<int>(STATE::CLEAR_SCORE) * HEIGHT_MARGIN; //Y座標（項目の開始Y座標 + 選択項目のインデックス * 項目間隔）
	const int numberDigit = Utility::GetDigitCount(clearScore_); //数字の桁数

	//数字の描画
	for (int i = 0; i < numberDigit; i++)
	{
		int index = Utility::GetDigit(clearScore_, numberDigit - 1 - i);

		DrawRotaGraph(
			POS_X + i * MARGIN_X - OFFSET_X,
			posY,
			0.7f,
			0.0f,
			imgNumbers_[index],
			true,
			false
		);
	};
}

void SystemSetting::DrawSkip()
{
	constexpr int POS_X = 850; //X座標
	const int posY = START_POS_Y + static_cast<int>(STATE::SKIP) * HEIGHT_MARGIN; //Y座標（項目の開始Y座標 + 選択項目のインデックス * 項目間隔）

	//スキップ設定の描画
	DrawRotaGraph(
		POS_X,
		posY,
		1.0f,
		0.0f,
		imgOnOff_[isSkip_ ? 0 : 1],
		true,
		false
	);
}

void SystemSetting::DrawSoundVolume()
{
	constexpr int POS_X = 900; //X座標
	constexpr int MARGIN_X = 64; //数字の間隔	
	constexpr int OFFSET_X = 90; //数字の位置を調整
	const int posY = START_POS_Y + static_cast<int>(STATE::SOUND_VOLUME) * HEIGHT_MARGIN; //Y座標（項目の開始Y座標 + 選択項目のインデックス * 項目間隔）
	const int numberDigit = Utility::GetDigitCount(soundVolume_); //数字の桁数

	//数字の描画
	for (int i = 0; i < numberDigit; i++)
	{
		int index = Utility::GetDigit(soundVolume_, numberDigit - 1 - i);

		DrawRotaGraph(
			POS_X + i * MARGIN_X - OFFSET_X,
			posY,
			0.7f,
			0.0f,
			imgNumbers_[index],
			true,
			false
		);
	};
}

void SystemSetting::DrawApplyMessage()
{
	if (updateType_ == UPDATE_TYPE::SELECT)
	{
		return;
	}

	//適用メッセージの描画
	DrawRotaGraph(
		Application::SCREEN_HALF_X,
		Application::SCREEN_HALF_Y,
		3.0f,
		0.0f,
		imgApplyMes_[0],
		true,
		false
	);
}

void SystemSetting::DrawSelectStateName(const int _index)
{
	constexpr int POS_X = 100; //X座標
	const int posY = START_POS_Y + _index * HEIGHT_MARGIN - 32; //Y座標（項目の開始Y座標 + 選択項目のインデックス * 項目間隔）

	//設定項目の描画
	DrawGraph(
		POS_X,
		posY,
		imgSettings_[_index],
		true
	);
}
