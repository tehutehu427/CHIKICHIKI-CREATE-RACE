#include"../Utility/Utility.h"
#include "PlayerAction.h"

PlayerAction::PlayerAction(Player& _player)
{
}

PlayerAction::~PlayerAction(void)
{
}

void PlayerAction::Init(void)
{
	//操作関連
//----------------------------------------------------
	changeAction_.emplace(ATK_ACT::NONE, [this]() {ChangeNone(); });
	changeAction_.emplace(ATK_ACT::MOVE, [this]() {ChangeMove(); });
	changeAction_.emplace(ATK_ACT::INPUT, [this]() {ChangeInput(); });
	changeAction_.emplace(ATK_ACT::JUMP, [this]() {ChangeJump(); });
	changeAction_.emplace(ATK_ACT::PUNCH, [this]() {ChangePunch(); });
	changeAction_.emplace(ATK_ACT::KNOCKBACK, [this]() {ChangeKnockBack(); });

	//ジャンプ関係
	isJump_ = false;
	stepJump_ = 0.0f;
	jumpPow_ = Utility::VECTOR_ZERO;
	jumpDeceralation_ = POW_JUMP;

	//パンチ関係の初期化
	punchCnt_ = 0.0f;
	punchCoolCnt_ = 0.0f;
	punchPos_ = Utility::VECTOR_ZERO;
	punchedCnt_ = PUNCHED_TIME;


}

void PlayerAction::Update(void)
{
}

void PlayerAction::NoneUpdate(void)
{
}

void PlayerAction::ActionInputUpdate(void)
{
}

void PlayerAction::ChangeInput(void)
{
}

void PlayerAction::ChangeNone(void)
{
}

void PlayerAction::MoveUpdate(void)
{
}

void PlayerAction::MoveDirFronInput(void)
{
}

void PlayerAction::ChangeMove(void)
{
}

void PlayerAction::UpdateMoveDirAndPow(void)
{
}

void PlayerAction::JumpUpdate(void)
{
}

void PlayerAction::Jump(void)
{
}

void PlayerAction::ChangeJump(void)
{
}

void PlayerAction::Punch(void)
{
}

void PlayerAction::ChangePunch(void)
{
}

void PlayerAction::KnockBack(void)
{
}

void PlayerAction::ChangeKnockBack(void)
{
}

void PlayerAction::Rotate(void)
{
}

void PlayerAction::SetGoalRotate(double _deg)
{
}
