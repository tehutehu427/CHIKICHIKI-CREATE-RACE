#include "../../Utility/Utility.h"
#include "../../Manager/Game/GravityManager.h"
#include "../../Manager/System/ResourceManager.h"
#include "../../Object/Common/Capsule.h"
#include "./Process/PlayerInput.h"
#include "Player.h"

Player::Player(int _playerNum,Transform _trans,PlayerInput::CNTL _cntl):playerNum_(_playerNum), cntl_(_cntl)
{
	animationController_ = nullptr;
	state_ = STATE::NONE;
	movedPos_ = Utility::VECTOR_ZERO;
	padNum_ = static_cast<InputManager::JOYPAD_NO>(playerNum_);
	transform_ = _trans;
	//オブジェクト生成
	//操作関連
	pMove_ = std::make_shared<PMove>();
	pJump_ = std::make_shared<PJump>();

	//カメラ
	camera_ = std::make_shared<Camera>();

	isCol_ = false;


	capsule_ = std::make_shared<Capsule>(transform_);
	capsule_->SetLocalPosTop({ 0.0f, 110.0f, 0.0f });
	capsule_->SetLocalPosDown({ 0.0f, 0.0f, 0.0f });
	capsule_->SetRadius(20.0f);

	//状態管理
	stateChanges_.emplace(STATE::NONE, std::bind(&Player::ChangeStateNone, this));
	stateChanges_.emplace(STATE::PLAY, std::bind(&Player::ChangeStatePlay, this));
}

void Player::Load(void)
{
}

void Player::Init(void)
{
	//transform_.modelId = transform_.SetModel(resMng_.GetInstance().LoadModelDuplicate(ResourceManager::SRC::)
	//transform_.scl = Utility::VECTOR_ONE;
	//transform_.pos = { 0.0f, 0.0f, 0.0f };
	//transform_.quaRot = Quaternion();
	//transform_.quaRotLocal =
	//	Quaternion::Euler({ 0.0f, Utility::Deg2RadF(180.0f), 0.0f });
	transform_.Update();

	// 初期状態
	ChangeState(STATE::PLAY);

	//入力
	PlayerInput::CreateInstance();

	//操作関連
	pMove_->Init();
	pJump_->Init();
}

void Player::Update(void)
{
	// 更新ステップ
	stateUpdate_();

	//入力更新
	PlayerInput::GetInstance().Update(padNum_,cntl_);

	transform_.Update();
}

void Player::Draw(void)
{
#ifdef DEBUG_ON
	DrawDebug();
#endif // DEBUG_ON

}

#ifdef DEBUG_ON
void Player::DrawDebug(void)
{
	unsigned int color = 0xffffff;
	if (isCol_) { color = 0xff0000; }
	DrawSphere3D(transform_.pos, 10.0f, 10, color, color, true);
	capsule_->Draw();
	pJump_->DrawDebug();
}
#endif // DEBUG_ON



void Player::ChangeState(STATE _state)
{
	//同じ状態だったら処理しない
	if (state_ == _state)return;

	//状態変更
	state_ = _state;

	//各状態への初期処理
	stateChanges_[state_]();
}

void Player::ChangeStateNone(void)
{
	stateUpdate_ = std::bind(&Player::UpdateNone,this);
}

void Player::ChangeStatePlay(void)
{
	stateUpdate_ = std::bind(&Player::UpdatePlay, this);
}

void Player::UpdateNone(void)
{

}

void Player::UpdatePlay(void)
{
	bool isJump = pJump_->GetIsJump();
	//移動関連
	pMove_->Update(camera_,isJump,transform_);
	pJump_->Update(transform_.GetUp(),transform_.GetDown(),IsEndLanding());

	// 衝突判定
	Collision();

	//Quaternion playerRotY = pMove_->GetPlayerRotY();
	//transform_.quaRot = transform_.quaRot.Mult(playerRotY);
}

void Player::CalcGravityPow(void)
{
}

void Player::Collision(void)
{
	VECTOR movePow = pMove_->GetMovePow();
	VECTOR jumpPow = pJump_->GetJumpPow();

	// 現在座標を起点に移動後座標を決める
	movedPos_ = VAdd(transform_.pos, movePow);
	movedPos_ = VAdd(movedPos_, jumpPow);
#ifdef DEBUG_ON
	if (movedPos_.y < 0.0f)
	{
		movedPos_.y = 0.0f;
		pJump_->SetIsJump(false);
		pJump_->SetStepJump(0.0f);
	}
#endif // DEBUG_ON
	//移動前の座標を格納する
	//moveDiff_ = VSub(movedPos_, transform_.pos);
	// 移動
	transform_.pos = movedPos_;
}

bool Player::IsEndLanding(void)
{
	return true;
}
