#include "../Utility/Utility.h"
#include "../Manager/System/KeyConfig.h"
#include "EditController.h"
#include "EditItemReady.h"

EditItemReady::EditItemReady(EditController& parent) : parent_(parent)
{
	ready_ = READY_PHASE::NOT_READY;
	hitSize_ = { HIT_WIDTH,HIT_HEIGHT };
	pos_ = { MARGIN,MARGIN };
	phaseChanges_.emplace(READY_PHASE::NOT_READY, std::bind(&EditItemReady::ChengePhaseNotRedy, this));
	phaseChanges_.emplace(READY_PHASE::CHECK, std::bind(&EditItemReady::ChengePhaseCheck, this));
	phaseChanges_.emplace(READY_PHASE::READY, std::bind(&EditItemReady::ChengePhaseReady, this));

}

EditItemReady::~EditItemReady()
{
}

void EditItemReady::Init()
{
	phaseChanges_[READY_PHASE::NOT_READY]();
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

void EditItemReady::ChengePhaseNotRedy(void)
{
	phaseUpdate_ = std::bind(&EditItemReady::UpdateNotReady, this);
	phaseDraw_ = std::bind(&EditItemReady::DrawNotReady, this);
}

void EditItemReady::ChengePhaseCheck(void)
{
	phaseUpdate_ = std::bind(&EditItemReady::UpdateCheck, this);
	phaseDraw_ = std::bind(&EditItemReady::DrawCheck, this);
}

void EditItemReady::ChengePhaseReady(void)
{
	phaseUpdate_ = std::bind(&EditItemReady::UpdateReady, this);
	phaseDraw_ = std::bind(&EditItemReady::DrawReady, this);
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
		ChangeReady(READY_PHASE::CHECK);
	}
}

void EditItemReady::UpdateCheck(void)
{
	KeyConfig& ins = KeyConfig::GetInstance();
	if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::CANCEL, parent_.GetPadNum(), KeyConfig::TYPE::PAD))
	{
		ChangeReady(READY_PHASE::NOT_READY);
		return;
	}
	if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, parent_.GetPadNum(), KeyConfig::TYPE::PAD))
	{
		ChangeReady(READY_PHASE::READY);
		return;
	}
}

void EditItemReady::UpdateReady(void)
{

}

void EditItemReady::DrawNotReady(void)
{
	DrawBox(MARGIN, MARGIN, MARGIN + hitSize_.x, MARGIN + hitSize_.y, 0x000000, true);
}

void EditItemReady::DrawCheck(void)
{
	auto screenSize = parent_.GetScreenSize();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128); //半透明にする
	//これでいいかの確認画面
	DrawBox(0, 0, screenSize.x, screenSize.y, 0x000000, true);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255); //不透明にする

	Utility::DrawStringPlace("OK Bボタン :CANCEL Aボタン", screenSize.x / 2, screenSize.y / 2, 0xffffff, Utility::STRING_PLACE::CENTER);
}

void EditItemReady::DrawReady(void)
{
	//準備完了時の描画処理
	auto screenSize = parent_.GetScreenSize();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128); //半透明にする
	//これでいいかの確認画面
	DrawBox(0, 0, screenSize.x, screenSize.y, 0x000000, true);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255); //不透明にする

}
