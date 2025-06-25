#include "ManualTab.h"
#include <DxLib.h>
#include "../../Manager/System/ResourceManager.h"
#include "../../Manager/System/KeyConfig.h"
#include "../../Utility/Utility.h"



ManualTab::ManualTab(const Vector2& _padCursolPos) :
	key_(KeyConfig::GetInstance()),
	padCursolPos_(_padCursolPos)
{
	imgIcon_ = -1;
	imgManual_ = -1;
	state_ = STATE::WAIT;
	uiPos_ = {};

	//処理の登録
	RegisterStateFunction(STATE::WAIT, SceneBase::ProcessFunction{ [&]() { UpdateWait(); },  [&]() { DrawWait(); } });
	RegisterStateFunction(STATE::DISPLAY, SceneBase::ProcessFunction{ [&]() { UpdateDisplay(); },  [&]() { DrawDisplay(); } });
}

ManualTab::~ManualTab()
{
}

void ManualTab::Load()
{
	//リソースの読み込み
	ResourceManager& res = ResourceManager::GetInstance();
	imgIcon_ = res.Load(ResourceManager::SRC::MANUAL_ICON).handleId_;
}

void ManualTab::Init()
{
	uiPos_ = {
		Application::SCREEN_SIZE_X - ICON_SIZE / 2,
		ICON_SIZE / 2,
	};
}

void ManualTab::Update()
{
	stateFunc_[state_].updataFunc_();
}

void ManualTab::Draw()
{
	stateFunc_[state_].drawFunc_();
}

void ManualTab::RegisterStateFunction(const STATE _state, SceneBase::ProcessFunction _func)
{
	stateFunc_[_state] = _func;
}

void ManualTab::UpdateWait()
{
	if(key_.IsTrgDown(KeyConfig::CONTROL_TYPE::MANUAL, KeyConfig::JOYPAD_NO::PAD1) ||
		key_.IsTrgDown(KeyConfig::CONTROL_TYPE::MANUAL_ICON_CLICK, KeyConfig::JOYPAD_NO::PAD1) &&
		(Utility::IsPointInRectCircle(key_.GetMousePos(), uiPos_, RADIUS) || 
			Utility::IsPointInRectCircle(padCursolPos_, uiPos_, RADIUS)))
	{
		ChangeState(STATE::DISPLAY);
	}
}

void ManualTab::UpdateDisplay()
{
	if (key_.IsTrgDown(KeyConfig::CONTROL_TYPE::MANUAL, KeyConfig::JOYPAD_NO::PAD1) ||
		key_.IsTrgDown(KeyConfig::CONTROL_TYPE::MANUAL_ICON_CLICK, KeyConfig::JOYPAD_NO::PAD1)) 
	{
		ChangeState(STATE::WAIT);
	}
}

void ManualTab::DrawWait()
{
	//アイコンを描画
	DrawRotaGraph(
		uiPos_.x,
		uiPos_.y,
		1.0f,
		0.0f,
		imgIcon_,
		true,
		false
	);
}

void ManualTab::DrawDisplay()
{
	//マニュアルを描画
	DrawBox(
		100, 100,
		Application::SCREEN_SIZE_X - 100,
		Application::SCREEN_SIZE_Y - 100,
		Utility::BLUE,
		true
	);
}
