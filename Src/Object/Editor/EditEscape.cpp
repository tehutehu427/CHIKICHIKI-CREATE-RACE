#include "EditEscape.h"
#include <string>
#include "../../Manager/System/SceneManager.h"
#include "../../Manager/System/ResourceManager.h"
#include "../../Manager/System/SoundManager.h"
#include "../../Manager/System/KeyConfig.h"
#include "../../Common/FontRegistry.h"
#include "../../Utility/Utility.h"
#include "../System/YesNoResponder.h"

namespace 
{
	Vector2 iconHalfSize =
	{
		EditEscape::ICON_SIZE_X / 2,
		EditEscape::ICON_SIZE_Y / 2
	};
}

EditEscape::EditEscape(const Vector2& _padCursorPos):
	padCursorPos_(_padCursorPos),
	key_(KeyConfig::GetInstance())
{
	//初期化
	pos_ = {};
	imgIcon_ = -1;
	imgSystemMessages_ = nullptr;
	font_ = 0;

	//処理の登録
	RegisterStateFunction(STATE::WAIT, SceneBase::ProcessFunction{ [&]() { UpdateWait(); },  [&]() { DrawWait(); } });
	RegisterStateFunction(STATE::CHECK, SceneBase::ProcessFunction{ [&]() { UpdateCheck(); },  [&]() { DrawCheck(); } });
}

EditEscape::~EditEscape()
{
	DeleteFontToHandle(font_);
}

void EditEscape::Load()
{
	ResourceManager& res = ResourceManager::GetInstance();
	imgIcon_ = res.Load(ResourceManager::SRC::CANCEL_ICON).handleId_;
	imgSystemMessages_ = res.Load(ResourceManager::SRC::EDIT_MESSAGES).handleIds_;

	font_ = CreateFontToHandle(FontRegistry::BOKUTATI.c_str(), FONT_SIZE, 0);

	responder_ = std::make_unique<YesNoResponder>();
	responder_->Load();

	SoundManager::GetInstance().LoadResource(SoundManager::SRC::EDIT_SYSTEM_ICON_CLICK);
}

void EditEscape::Init()
{
	//座標設定
	pos_ = iconHalfSize;

	responder_->Init();
}

void EditEscape::Update()
{
	//更新
	stateFunc_[state_].updataFunc_();
}

void EditEscape::Draw()
{
	//描画
	stateFunc_[state_].drawFunc_();
}

const bool EditEscape::IsEdit() const
{
	return state_ == STATE::WAIT;
}

void EditEscape::RegisterStateFunction(const STATE _state, SceneBase::ProcessFunction _func)
{
	//処理の登録
	stateFunc_[_state] = _func;
}

void EditEscape::UpdateWait()
{
	//座標設定
	Vector2 leftTop = Vector2::SubVector2(pos_, iconHalfSize);
	Vector2 rightBottom = Vector2::AddVector2(pos_, iconHalfSize);

	if(key_.IsTrgDown(KeyConfig::CONTROL_TYPE::EDIT_ESCAPE, KeyConfig::JOYPAD_NO::PAD1) ||
		(key_.IsTrgDown(KeyConfig::CONTROL_TYPE::EDIT_ESCAPE_CLICK, KeyConfig::JOYPAD_NO::PAD1) &&
		(Utility::IsPointInRect(key_.GetMousePos(), leftTop, rightBottom) || 
			Utility::IsPointInRect(padCursorPos_, leftTop, rightBottom))))
	{
		SoundManager::GetInstance().Play(SoundManager::SRC::EDIT_SYSTEM_ICON_CLICK, SoundManager::PLAYTYPE::BACK);

		//リセット
		responder_->Reset();

		//状態遷移
		ChangeState(STATE::CHECK);
		return;
	}
}

void EditEscape::UpdateCheck()
{
	YesNoResponder::RESPON res = responder_->GetRespon();

	if (res == YesNoResponder::RESPON::NONE)
	{
		responder_->Update();
		return;
	}
	else if (res == YesNoResponder::RESPON::YES)
	{
		//シーンをセレクトシーンへ遷移
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::SELECT);
		ChangeState(STATE::WAIT);
		return;
	}
	else
	{
		//状態を戻す
		ChangeState(STATE::WAIT);
		return;
	}

}

void EditEscape::DrawWait()
{
	DrawRotaGraph(
		pos_.x,
		pos_.y,
		1.0f,
		0.0f,
		imgIcon_,
		true,
		false
	);
}

void EditEscape::DrawCheck()
{
	//背景含め回答ボックスの描画
	responder_->Draw();

	//メッセージの描画
	constexpr int OFFSET_Y = 120;

	DrawRotaGraph(
		Application::SCREEN_HALF_X,
		Application::SCREEN_HALF_Y - OFFSET_Y,
		0.7f,
		0.0f,
		imgSystemMessages_[SYS_MES_INDEX],
		true,
		false
	);
}
