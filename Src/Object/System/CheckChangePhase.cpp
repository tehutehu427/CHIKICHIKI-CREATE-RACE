#include "CheckChangePhase.h"
#include <DxLib.h>
#include "../../Application.h"
#include "../../Manager/System/KeyConfig.h"
#include "../../Manager/System/InputManager.h"
#include "../../Manager/System/ResourceManager.h"
#include "../../Manager/System/SoundManager.h"
#include "../../Utility/Utility.h"

CheckChangePhase::CheckChangePhase(const Vector2& _padCursorPos) :
	padCursolPos_(_padCursorPos)
{
	int i = -1;
	imgIcons_ = &i;
	iconIndex_ = -1;
	nextPhase_ = GameScene::PHASE::EDIT_PHASE;
}

void CheckChangePhase::Load(void)
{
	ResourceManager& res = ResourceManager::GetInstance();
	imgIcons_ = res.Load(ResourceManager::SRC::CHANGE_PHASE_ICONS).handleIds_;

	SoundManager::GetInstance().LoadResource(SoundManager::SRC::CHANGE_PHASE_ICON_CLICK);
}

void CheckChangePhase::Init(void)
{
	//座標
	pos_ =
	{
		ICON_SIZE_X / 2,
		Application::SCREEN_SIZE_Y - ICON_SIZE_Y / 2,
	};

	iconIndex_ = 0;
}

void CheckChangePhase::Update(GameScene& _parent)
{
	//特定のキーを押す、もしくはアイコンをクリックした場合
	if (IsCheckChangePhase(_parent))
	{
		//フェーズを遷移
		SoundManager::GetInstance().Play(SoundManager::SRC::CHANGE_PHASE_ICON_CLICK, SoundManager::PLAYTYPE::BACK);
		_parent.ChangePhase(nextPhase_);
	}
}

void CheckChangePhase::Draw(void)
{
	DrawRotaGraph(
		pos_.x,
		pos_.y,
		1.0f,
		0.0f,
		imgIcons_[iconIndex_],
		true,
		false);
}

void CheckChangePhase::SetNextPhase(const GameScene::PHASE _nextPhase)
{
	nextPhase_ = _nextPhase;
	switch (nextPhase_)
	{
	case GameScene::PHASE::ACTION_PHASE:
		iconIndex_ = static_cast<int>(ICON_TYPE::PLAY);
		break;

	case GameScene::PHASE::EDIT_PHASE:
		iconIndex_ = static_cast<int>(ICON_TYPE::EDIT);
		break;

	default:
		break;
	}
}

bool CheckChangePhase::IsCheckChangePhase(GameScene& _parent)
{
	KeyConfig& key = KeyConfig::GetInstance();
	Vector2 rightTop = 
	{
		pos_.x - ICON_SIZE_X / 2,
		pos_.y - ICON_SIZE_Y / 2,
	};

	Vector2 leftBottom =
	{
		pos_.x + ICON_SIZE_X / 2,
		pos_.y + ICON_SIZE_Y / 2,
	};

	//特定のキーを押す
	if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::PHASE_CHANGE, KeyConfig::JOYPAD_NO::PAD1))
	{
		return true;
	}
	//エディット時以外は以下の処理をしない
	if(_parent.GetPhase() != GameScene::PHASE::EDIT_PHASE)
	{
		return false;
	}
	//アイコンをクリックする(Editのみ)
	if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::PHASE_CHANGE_CHECK, KeyConfig::JOYPAD_NO::PAD1) &&
		(Utility::IsPointInRect(key.GetMousePos(), rightTop, leftBottom) ||
		Utility::IsPointInRect(padCursolPos_, rightTop, leftBottom)))
	{
		return true;
	}

	return false;
}