#include "../../Utility/Utility.h"
#include "Player.h"

Player::Player(void)
{
	animationController_ = nullptr;
	state_ = STATE::NONE;
	movedPos_ = Utility::VECTOR_ZERO;

	//オブジェクト生成
	//操作関連
	pMove_ = std::make_shared<PMove>();
	pJump_ = std::make_shared<PJump>();

	//カメラ(後でゲームシーンに書く)
	camera_ = std::make_shared<Camera>();

	//状態管理
	stateChanges_.emplace(STATE::NONE, std::bind(&Player::ChangeStateNone, this));
	stateChanges_.emplace(STATE::PLAY, std::bind(&Player::ChangeStatePlay, this));
}


void Player::Load(void)
{
	//モデルの基本設定
	//transform_.SetModel(resMng_.LoadModelDuplicate(
	//	ResourceManager::SRC::PLAYER));

}

void Player::Init(void)
{
	transform_.scl = Utility::VECTOR_ONE;
	transform_.pos = { 0.0f, 0.0f, 0.0f };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, Utility::Deg2RadF(180.0f), 0.0f });
	transform_.Update();

	// 初期状態
	ChangeState(STATE::PLAY);

	//操作関連
	pMove_->Init();
	pJump_->Init();
}

void Player::Update(void)
{
	// 更新ステップ
	stateUpdate_();

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
	DrawSphere3D(transform_.pos, 10.0f, 10, 0xff0000, 0xff0000, true);
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
	pMove_->Update(camera_,isJump);
	pJump_->Update(transform_.GetUp(),transform_.GetDown(),IsEndLanding());

	// 衝突判定
	Collision();

	Quaternion playerRotY = pMove_->GetPlayerRotY();
	transform_.quaRot = transform_.quaRot.Mult(playerRotY);
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
