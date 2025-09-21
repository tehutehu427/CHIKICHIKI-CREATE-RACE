#include "SystemSetting.h"
#include <DxLib.h>
#include "../../../Manager/System/ResourceManager.h"
#include "../../../Manager/System/SceneManager.h"
#include "../../../Manager/System/KeyConfig.h"
#include "../../../Manager/System/DateBank.h"
#include "../../../Utility/Utility.h"
#include "../../../Common/IntVector3.h"
#include "../Scene/SelectScene.h"

namespace
{
	constexpr int START_POS_Y = 150; //項目の開始Y座標
	constexpr int HEIGHT_MARGIN = 80; //項目間の高さのマージン
	constexpr IntVector3 SELECT_ADD_COLOR = { 128,0,0 };	//選択色
}

SystemSetting::SystemSetting(void)
{
	//初期化
	stateIndex_ = -1;
	clearScore_ = -1;
	roundCnt_ = -1;
	for (int i = 0; i < SoundManager::TYPE_MAX; i++) { soundVolume_[i] = -1; }
	isSkip_ = false;
	isFullScreen_ = false;
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
			clearScore_ = (clearScore_ - CLEAR_SCORE_MIN + 1) % CLEAR_SCORE_RANGE + CLEAR_SCORE_MIN;
		}},
		{STATE::SKIP,[this]()
		{
			isSkip_ = !isSkip_;
		}},
		{STATE::BGM_VOLUME,[this]()
		{
			// 右キーで音量を一段階上げる（最大を超えたら最小に戻る）
			soundVolume_[static_cast<int>(SoundManager::TYPE::BGM)] += SOUND_VOLUME_STEP;
			if (soundVolume_[static_cast<int>(SoundManager::TYPE::BGM)] > SOUND_VOLUME_MAX)
			{
				soundVolume_[static_cast<int>(SoundManager::TYPE::BGM)] = 0;
			}
		}},
		{STATE::SE_VOLUME,[this]()
		{
			// 右キーで音量を一段階上げる（最大を超えたら最小に戻る）
			soundVolume_[static_cast<int>(SoundManager::TYPE::SE)] += SOUND_VOLUME_STEP;
			if (soundVolume_[static_cast<int>(SoundManager::TYPE::SE)] > SOUND_VOLUME_MAX)
			{
				soundVolume_[static_cast<int>(SoundManager::TYPE::SE)] = 0;
			}
		}},
		{STATE::FULL_SCREEN,[this]()
		{
			isFullScreen_ = !isFullScreen_;
		}},
		{STATE::ROUND,[this]()
		{
			//右キーで選択をひとつ進める（範囲内でループ）
			roundCnt_ = (roundCnt_ - ROUND_MIN + 1) % ROUND_RANGE + ROUND_MIN;
		}},
	};

	//ルール項目別左の処理の登録
	leftStateMap_ =
	{
		{STATE::CLEAR_SCORE,[this]()
		{
			//左キーで選択をひとつ戻す（範囲内でループ）
			clearScore_ = (clearScore_ - CLEAR_SCORE_MIN - 1 + CLEAR_SCORE_RANGE) % CLEAR_SCORE_RANGE + CLEAR_SCORE_MIN;
		}},
		{STATE::SKIP,[this]()
		{
			isSkip_ = !isSkip_;
		}},
		{STATE::BGM_VOLUME,[this]()
		{
			// 左キーで音量を一段階下げる（最小未満になったら最大に戻る）
			soundVolume_[static_cast<int>(SoundManager::TYPE::BGM)] -= SOUND_VOLUME_STEP;
			if (soundVolume_[static_cast<int>(SoundManager::TYPE::BGM)] < 0)
			{
				soundVolume_[static_cast<int>(SoundManager::TYPE::BGM)] = SOUND_VOLUME_MAX;
			}
		}},
		{STATE::SE_VOLUME,[this]()
		{
			// 左キーで音量を一段階下げる（最小未満になったら最大に戻る）
			soundVolume_[static_cast<int>(SoundManager::TYPE::SE)] -= SOUND_VOLUME_STEP;
			if (soundVolume_[static_cast<int>(SoundManager::TYPE::SE)] < 0)
			{
				soundVolume_[static_cast<int>(SoundManager::TYPE::SE)] = SOUND_VOLUME_MAX;
			}
		}},
		{STATE::FULL_SCREEN,[this]()
		{
			isFullScreen_ = !isFullScreen_;
		}},
		{STATE::ROUND,[this]()
		{
			//左キーで選択をひとつ戻す（範囲内でループ）
			roundCnt_ = (roundCnt_ - ROUND_MIN - 1 + ROUND_RANGE) % ROUND_RANGE + ROUND_MIN;
		}}
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
			DrawSwitch(STATE::SKIP, isSkip_);
		}},
		{STATE::BGM_VOLUME,[this]()
		{
			DrawSoundVolume(STATE::BGM_VOLUME, static_cast<int>(SoundManager::TYPE::BGM));
		}},
		{STATE::SE_VOLUME,[this]()
		{
			DrawSoundVolume(STATE::SE_VOLUME, static_cast<int>(SoundManager::TYPE::SE));
		}},
		{STATE::FULL_SCREEN,[this]()
		{
			DrawSwitch(STATE::FULL_SCREEN, isFullScreen_);
		}},
		{STATE::ROUND,[this]()
		{
			DrawRound();
		}},
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

void SystemSetting::Load(void)
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

void SystemSetting::Init(void)
{	
	DateBank& data = DateBank::GetInstance();
	SoundManager& sound = SoundManager::GetInstance();

	//初期状態
	stateIndex_ = 0;
	clearScore_ = data.GetMultiClearScore();
	isSkip_ = data.IsItemSetSkip(); 
	isFullScreen_ = data.IsFullScreen();
	roundCnt_ = data.GetRoundLimit();
	for (int i = 0; i < SoundManager::TYPE_MAX; i++) { soundVolume_[i] = sound.GetSoundTypeVolume(i); }
}

void SystemSetting::Update(SelectScene& _parent)
{
	updateMap_[updateType_](_parent); //更新処理を呼び出す
}

void SystemSetting::Draw(void)
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

void SystemSetting::DrawTitle(void)
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

void SystemSetting::DrawSettingFinish(void)
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
	SoundManager& sound = SoundManager::GetInstance();

	//決定
	if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::DECISION_KEY_AND_PAD, KeyConfig::JOYPAD_NO::PAD1) && stateIndex_ == static_cast<int>(STATE::APPLY))
	{
		updateType_ = UPDATE_TYPE::APPLY; //更新タイプを適用に変更
		sound.Play(SoundManager::SRC::DECISION, SoundManager::PLAYTYPE::BACK); //決定音を再生
		return;
	}
	//上へ
	else if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_UP, KeyConfig::JOYPAD_NO::PAD1))
	{
		//下キーで選択をひとつ戻す（範囲内でループ）
		stateIndex_ = (stateIndex_ - 1 + STATE_MAX) % STATE_MAX;
		sound.Play(SoundManager::SRC::CLICK_OBJECT_SE, SoundManager::PLAYTYPE::BACK); //クリック音を再生
		return;
	}
	//下へ
	else if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_DOWN, KeyConfig::JOYPAD_NO::PAD1))
	{
		//上キーで選択をひとつ進める（範囲内でループ）
		stateIndex_ = (stateIndex_ + 1) % STATE_MAX;
		sound.Play(SoundManager::SRC::CLICK_OBJECT_SE, SoundManager::PLAYTYPE::BACK); //クリック音を再生
		return;
	}
	//右
	else if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_RIGHT, KeyConfig::JOYPAD_NO::PAD1) && stateIndex_ != static_cast<int>(STATE::APPLY))
	{
		rightStateMap_[static_cast<STATE>(stateIndex_)](); //右の処理を実行
		sound.Play(SoundManager::SRC::CLICK_OBJECT_SE, SoundManager::PLAYTYPE::BACK); //クリック音を再生
		return;
	}
	//左
	else if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_LEFT, KeyConfig::JOYPAD_NO::PAD1) && stateIndex_ != static_cast<int>(STATE::APPLY))
	{
		leftStateMap_[static_cast<STATE>(stateIndex_)](); //左の処理を実行
		sound.Play(SoundManager::SRC::CLICK_OBJECT_SE, SoundManager::PLAYTYPE::BACK); //クリック音を再生
		return;
	}
	//メニューに戻る
	else if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::CANCEL, KeyConfig::JOYPAD_NO::PAD1))
	{
		_parent.ChangeState(SelectScene::STATE::SELECT_MENU); //メニューに戻る
		sound.Play(SoundManager::SRC::CANCEL, SoundManager::PLAYTYPE::BACK); //キャンセル音を再生
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

		SoundManager::GetInstance().Play(SoundManager::SRC::DECISION, SoundManager::PLAYTYPE::BACK); //決定音を再生

		//更新タイプを選択に戻す
		updateType_ = UPDATE_TYPE::SELECT;
		return;
	}
}

void SystemSetting::ApplyData(void)
{
	DateBank& data = DateBank::GetInstance();
	SoundManager& sound = SoundManager::GetInstance();

	//クリアスコアの設定
	data.SetMultiClearScore(clearScore_);

	//スキップ設定の反映
	data.SetItemSetSkip(isSkip_);

	//サウンドボリュームの設定
	for (int i = 0; i < SoundManager::TYPE_MAX; i++) { sound.SetSystemVolume(soundVolume_[i], i); }

	//スクリーン設定
	data.SetFullScreen(isFullScreen_);

	//ラウンド数設定
	data.SetRoundLimit(roundCnt_);
}

void SystemSetting::DrawCursor(void)
{
	constexpr int POS_X = 50;		//カーソルのX座標
	constexpr float ANGLE = 90.0f;	//角度
	constexpr int INDEX = 0;		//画像インデックス
	int posX = POS_X;				//カーソルのX座標（初期値）
	int posY = START_POS_Y + stateIndex_ * HEIGHT_MARGIN; //カーソルのY座標（項目の開始Y座標 + 選択項目のインデックス * 項目間隔）

	if (stateIndex_ == static_cast<int>(STATE::APPLY))
	{
		constexpr int APPLY_POS_X = Application::SCREEN_SIZE_X - 250;
		constexpr int APPLY_POS_Y = Application::SCREEN_SIZE_Y - 32;

		//座標の設定
		posX = APPLY_POS_X;	//X座標
		posY = APPLY_POS_Y;	//Y座標
	}

	DrawRotaGraph(
		posX,
		posY,
		1.0f,
		Utility::Deg2RadF(ANGLE),
		imgCursor_[INDEX],
		true
	);
}

void SystemSetting::DrawSelectTriangle(void)
{	
	if (stateIndex_ == static_cast<int>(STATE::APPLY))
	{
		return; //適用状態では描画しない
	}

	constexpr int DRAW_NUM = 2;			//描画する数
	constexpr int POS_X = 720;			//カーソルのX座標
	constexpr int LEFT_OFFSET_X = 250;	//左の座標調整
	constexpr float RATE = 0.7f;		//拡大率
	constexpr float RIGHT_ROT = 270.0f;	//右の回転
	constexpr float LEFT_ROT = 90.0f;	//左の回転
	const int POS_Y = START_POS_Y + stateIndex_ * HEIGHT_MARGIN; //カーソルのY座標

	for (int i = 0; i < DRAW_NUM; i++)
	{
		int posX = POS_X;
		float angle = RIGHT_ROT;
		if (i == 1)
		{
			posX += LEFT_OFFSET_X; //左側の位置調整
			angle = LEFT_ROT; //左側の矢印は90度回す
		}


		//矢印の描画
		DrawRotaGraph(
			posX,
			POS_Y,
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
	const int POS_Y = START_POS_Y + static_cast<int>(STATE::CLEAR_SCORE) * HEIGHT_MARGIN; //Y座標（項目の開始Y座標 + 選択項目のインデックス * 項目間隔）
	const int NUMBER_DIGIT = Utility::GetDigitCount(clearScore_); //数字の桁数

	//数字の描画
	for (int i = 0; i < NUMBER_DIGIT; i++)
	{
		int index = Utility::GetDigit(clearScore_, NUMBER_DIGIT - 1 - i);

		DrawRotaGraph(
			POS_X + i * MARGIN_X - OFFSET_X,
			POS_Y,
			0.7f,
			0.0f,
			imgNumbers_[index],
			true,
			false
		);
	};
}

void SystemSetting::DrawSwitch(const STATE _state, const bool _flag)
{
	constexpr int POS_X = 850; //X座標
	const int POS_Y = START_POS_Y + static_cast<int>(_state) * HEIGHT_MARGIN; //Y座標

	//スキップ設定の描画
	DrawRotaGraph(
		POS_X,
		POS_Y,
		1.0f,
		0.0f,
		imgOnOff_[_flag ? 0 : 1],
		true,
		false
	);
}

void SystemSetting::DrawSoundVolume(const STATE _state, const int _type)
{
	constexpr int POS_X = 900;		//X座標
	constexpr int MARGIN_X = 64;	//数字の間隔	
	constexpr int OFFSET_X = 90;	//数字の位置を調整
	constexpr float RATE = 0.7f;	//拡大率
	const int POS_Y = START_POS_Y + static_cast<int>(_state) * HEIGHT_MARGIN; //Y座標（項目の開始Y座標 + 選択項目のインデックス * 項目間隔）
	const int NUM_DIGIT = Utility::GetDigitCount(soundVolume_[_type]); //数字の桁数

	//数字の描画
	for (int i = 0; i < NUM_DIGIT; i++)
	{
		int index = Utility::GetDigit(soundVolume_[_type], NUM_DIGIT - 1 - i);

		DrawRotaGraph(
			POS_X + i * MARGIN_X - OFFSET_X,
			POS_Y,
			RATE,
			0.0f,
			imgNumbers_[index],
			true,
			false
		);
	};
}

void SystemSetting::DrawRound(void)
{
	constexpr int ONOFF_POS_X = 850;	//スイッチのX座標
	constexpr int NUM_POS_X = 900;		//数字のX座標
	constexpr int MARGIN_X = 64;		//数字の間隔	
	constexpr int OFFSET_X = 90;		//数字の位置を調整
	constexpr float NUM_RATE = 0.7f;	//数字の拡大率
	const int POS_Y = START_POS_Y + static_cast<int>(STATE::ROUND) * HEIGHT_MARGIN; //Y座標
	const int NUM_DIGIT = Utility::GetDigitCount(roundCnt_); //数字の桁数

	if (roundCnt_ == 0)
	{
		DrawRotaGraph(
			ONOFF_POS_X,
			POS_Y,
			1.0f,
			0.0f,
			imgOnOff_[1],
			true,
			false
		);
	}
	else
	{
		//数字の描画
		for (int i = 0; i < NUM_DIGIT; i++)
		{
			int index = Utility::GetDigit(roundCnt_, NUM_DIGIT - 1 - i);

			DrawRotaGraph(
				NUM_POS_X + i * MARGIN_X - OFFSET_X,
				POS_Y,
				NUM_RATE,
				0.0f,
				imgNumbers_[index],
				true,
				false
			);
		};
	}
}

void SystemSetting::DrawApplyMessage(void)
{
	if (updateType_ == UPDATE_TYPE::SELECT)
	{
		return;
	}

	constexpr float RATE = 3.0f;
	constexpr int INDEX = 0;

	//適用メッセージの描画
	DrawRotaGraph(
		Application::SCREEN_HALF_X,
		Application::SCREEN_HALF_Y,
		RATE,
		0.0f,
		imgApplyMes_[INDEX],
		true,
		false
	);
}

void SystemSetting::DrawSelectStateName(const int _index)
{
	constexpr int POS_X = 100; //X座標
	const int posY = START_POS_Y + _index * HEIGHT_MARGIN - 16; //Y座標（項目の開始Y座標 + 選択項目のインデックス * 項目間隔）

	//設定項目の描画
	DrawGraph(
		POS_X,
		posY,
		imgSettings_[_index],
		true
	);
}
