#include "CheckChangePhase.h"
#include <DxLib.h>
#include "../../Application.h"
#include "../../Manager/System/KeyConfig.h"
#include "../../Manager/System/InputManager.h"
#include "../../Manager/System/ResourceManager.h"
//#include "../../Scene/Game/GameScene.h"

CheckChangePhase::CheckChangePhase()
{
	int i = -1;
	imgIcons_ = &i;
	iconIndex_ = -1;
	nextPhase_ = GameScene::PHASE::EDIT_PHASE;
}

CheckChangePhase::~CheckChangePhase()
{
}

void CheckChangePhase::Load()
{
	ResourceManager& res = ResourceManager::GetInstance();
	imgIcons_ = res.Load(ResourceManager::SRC::CHANGE_PHASE_ICONS).handleIds_;
}

void CheckChangePhase::Init()
{
	//座標
	pos_ = {
		ICON_SIZE_X / 2,
		Application::SCREEN_SIZE_Y - ICON_SIZE_Y / 2,
	};
	
	iconIndex_ = 0;
}

void CheckChangePhase::Update(GameScene& _parent)
{	
	//特定のキーを押す、もしくはアイコンをクリックした場合
	if (IsCheckChangePhase())
	{
		//フェーズを遷移
		_parent.ChangePhase(nextPhase_);
	}
}

void CheckChangePhase::Draw()
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

bool CheckChangePhase::IsCheckChangePhase()
{
	KeyConfig& key = KeyConfig::GetInstance();
	//key.IsTrgDown(KeyConfig::CONTROL_TYPE::MOUSE)

	return false;
}