#include "../Utility/Utility.h"
#include "../Manager/System/KeyConfig.h"
#include "../Manager/System/ResourceManager.h"
#include "../Manager/System/SoundManager.h"
#include "EditController.h"
#include "EditItemReady.h"

EditItemReady::EditItemReady(EditController& parent) : parent_(parent)
{
	ready_ = READY_PHASE::NOT_READY;
	hitSize_ = { HIT_WIDTH,HIT_HEIGHT };
	pos_ = { MARGIN,MARGIN };
	readyImg_ = -1;
	phaseChanges_.emplace(READY_PHASE::NOT_READY, std::bind(&EditItemReady::ChangePhaseNotRedy, this));
	phaseChanges_.emplace(READY_PHASE::CHECK, std::bind(&EditItemReady::ChangePhaseCheck, this));
	phaseChanges_.emplace(READY_PHASE::READY, std::bind(&EditItemReady::ChangePhaseReady, this));
	readyImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::READY_IMG).handleId_;
}

EditItemReady::~EditItemReady()
{
}

void EditItemReady::Init()
{
	ChangeReady(READY_PHASE::NOT_READY);
}

void EditItemReady::Update()
{
	phaseUpdate_();
}

void EditItemReady::Draw()
{
	phaseDraw_();
}

void EditItemReady::ChangeReady(const READY_PHASE _ready)
{
	ready_ = _ready;
	phaseChanges_[ready_]();

}

void EditItemReady::ChangePhaseNotRedy(void)
{
	phaseUpdate_ = std::bind(&EditItemReady::UpdateNotReady, this);
	phaseDraw_ = std::bind(&EditItemReady::DrawNotReady, this);
}

void EditItemReady::ChangePhaseCheck(void)
{
	phaseUpdate_ = std::bind(&EditItemReady::UpdateCheck, this);
	phaseDraw_ = std::bind(&EditItemReady::DrawCheck, this);
}

void EditItemReady::ChangePhaseReady(void)
{
	phaseUpdate_ = std::bind(&EditItemReady::UpdateReady, this);
	phaseDraw_ = std::bind(&EditItemReady::DrawReady, this);
	parent_.SetReady(); //EditControllerに準備完了を通知
}

void EditItemReady::UpdateNotReady(void)
{
	KeyConfig& ins = KeyConfig::GetInstance();
	if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, parent_.GetPadNum(), KeyConfig::TYPE::PAD))
	{
		auto cursorPos = parent_.GetCursorPos();
		if (cursorPos.x < MARGIN || cursorPos.x > MARGIN + hitSize_.x ||
			cursorPos.y < MARGIN || cursorPos.y > MARGIN + hitSize_.y)
		{
			return; //カーソルが当たり判定の外にある場合は何もしない
		}
		int errorType = parent_.IsError();
		if (errorType < 0)
		{
			//エラーがあった場合はエラー音を鳴らしてエラー状態にする
			SoundManager::GetInstance().Play(SoundManager::SRC::ERROR_SE, SoundManager::PLAYTYPE::BACK);
			parent_.SetError(errorType);
			return;
		}
		SoundManager::GetInstance().Play(SoundManager::SRC::OK, SoundManager::PLAYTYPE::BACK);
		ChangeReady(READY_PHASE::CHECK);
	}
}

void EditItemReady::UpdateCheck(void)
{
	int errorType = parent_.IsError();
	if (errorType < 0)
	{
		//エラーがあった場合はエラー音を鳴らしてエラー状態にする
		SoundManager::GetInstance().Play(SoundManager::SRC::ERROR_SE, SoundManager::PLAYTYPE::BACK);
		parent_.SetError(errorType);
		ChangeReady(READY_PHASE::NOT_READY);
		return;
	}
	KeyConfig& ins = KeyConfig::GetInstance();
	if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::CANCEL, parent_.GetPadNum(), KeyConfig::TYPE::PAD))
	{
		//キャンセルを押されたら戻る
		SoundManager::GetInstance().Play(SoundManager::SRC::CANCEL, SoundManager::PLAYTYPE::BACK);
		ChangeReady(READY_PHASE::NOT_READY);
		return;
	}
	if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, parent_.GetPadNum(), KeyConfig::TYPE::PAD))
	{
		//OKを押されたら準備完了
		SoundManager::GetInstance().Play(SoundManager::SRC::OK, SoundManager::PLAYTYPE::BACK);
		ChangeReady(READY_PHASE::READY);
		return;
	}
}

void EditItemReady::UpdateReady(void)
{
	KeyConfig& ins = KeyConfig::GetInstance();
	if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, parent_.GetPadNum(), KeyConfig::TYPE::PAD))
	{
		SoundManager::SRC type = GetRand(CHIKEN_VOICE_RATE) == 0 ? SoundManager::SRC::CHICKEN_SE : SoundManager::SRC::CLICK_OBJECT_SE;
		SoundManager::GetInstance().Play(type, SoundManager::PLAYTYPE::BACK);
	}

}

void EditItemReady::DrawNotReady(void) const
{
	//DrawBox(MARGIN, MARGIN, MARGIN + hitSize_.x, MARGIN + hitSize_.y, 0x000000, true);
	DrawModiGraph(MARGIN, MARGIN + hitSize_.y, MARGIN, MARGIN, MARGIN + hitSize_.x, MARGIN, MARGIN + hitSize_.x, MARGIN + hitSize_.y, readyImg_, true);
}

void EditItemReady::DrawCheck(void)
{
	auto screenSize = parent_.GetScreenSize();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, ALPHA_RATE); //半透明にする
	//これでいいかの確認画面
	DrawBox(0, 0, screenSize.x, screenSize.y, Utility::BLACK, true);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, ALPHA_RATE_MAX); //不透明にする

	Utility::DrawStringPlace("OK Bボタン :CANCEL Aボタン", screenSize.x / 2, screenSize.y / 2, Utility::WHITE, Utility::STRING_PLACE::CENTER);
}

void EditItemReady::DrawReady(void)
{
	//準備完了時の描画処理
	auto screenSize = parent_.GetScreenSize();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, ALPHA_RATE); //半透明にする
	//これでいいかの確認画面
	DrawBox(0, 0, screenSize.x, screenSize.y, Utility::BLACK, true);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, ALPHA_RATE_MAX); //不透明にする
	DrawRotaGraph(screenSize.x / 2, screenSize.y / 2, 1.0f, 0.0f, ResourceManager::GetInstance().Load(ResourceManager::SRC::OK).handleId_, true); //中央に準備完了の画像を表示

}
