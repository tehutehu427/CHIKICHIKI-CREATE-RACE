#include "MultiCheckPlayer.h"
#include "../../../Application.h"
#include "../../../Manager/System/ResourceManager.h"
#include "../../../Manager/Game/PlayerManager.h"
#include "../../../Utility/Utility.h"
#include "../../Common/AnimationController.h"
#include "../../Common/Animation2DController.h"
#include "../../Common/ToonStyle.h"
#include "../../Player/Player.h"

MultiCheckPlayer::MultiCheckPlayer()
{
	RegisterStateUpdate(STATE::SMOKE, [this]() { UpdateSmoke(); });
	RegisterStateUpdate(STATE::ANIMATION, [this]() { UpdateAnimation(); });
}

MultiCheckPlayer::~MultiCheckPlayer()
{
}

void MultiCheckPlayer::Create(const int _playerIndex, const int _playerNum)
{
	//プレイヤー番号
	playerIndex_ = _playerIndex;

	//プレイヤー人数
	playerNum_ = _playerNum;	
	
	//リソースの読み込み
	trans_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::CHICKEN));	

	//トゥーン設定
	toon_ = std::make_unique<ToonStyle>();
	toon_->Load(trans_.modelId, ToonStyle::MESH_TYPE::CHICKEN);

	//プレイヤー番号別の設定
	SetParamByPlayerIndex();

	//モデルの基本情報を設定
	trans_.pos = ConvScreenPosToWorldPos(VGet((float)screenPos_.x, (float)screenPos_.y, 0.75f));
	trans_.quaRot = Quaternion();
	trans_.scl = Utility::VECTOR_ONE;
	trans_.quaRotLocal =
		Quaternion::Euler({ 0.0f, Utility::Deg2RadF(0.0f), 0.0f });
	trans_.localPos = { 0.0f,-Player::RADIUS,0.0f };
	trans_.Update();

	//リソースの読み込みなど
	animController_ = std::make_unique<AnimationController>(trans_.modelId);
	animController_->Add(static_cast<int>(Player::ANIM_TYPE::IDLE), Player::DEFAULT_ANIM_SPD);
	animController_->Add(static_cast<int>(Player::ANIM_TYPE::HAND_WAVE), Player::DEFAULT_ANIM_SPD);
	animController_->Add(static_cast<int>(Player::ANIM_TYPE::PUNCH), Player::DEFAULT_ANIM_SPD);
	animController_->Play(static_cast<int>(Player::ANIM_TYPE::IDLE));

	//状態
	ChangeState(STATE::SMOKE);

	//エフェクト
	imgSmokes_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::SMOKE_EFE_SPR).handleIds_;

	//エフェクトコントローラー
	effectController_ = std::make_unique<Animation2DController>();
	effectController_->SetAnimParam(0, ResourceManager::SMOKE_EFE_DIV_X, EFFECT_SPEED);
	effectController_->PlayAnimation();
}

void MultiCheckPlayer::Update()
{
	stateMap_[state_]();
}

void MultiCheckPlayer::Draw()
{
	//プレイヤーの描画
	toon_->Draw();

	//エフェクトの描画
	DrawRotaGraph(
		screenPos_.x,
		screenPos_.y,
		2.5f,
		0.0f,
		imgSmokes_[effectController_->GetAnimationIndex()],
		true
	);

}

void MultiCheckPlayer::Reset()
{
	ChangeState(STATE::SMOKE);
	effectController_->PlayAnimation();
}

void MultiCheckPlayer::SetGameStartAnimation()
{
	animController_->Play(static_cast<int>(Player::ANIM_TYPE::PUNCH), false, 0.0f, ANIM_STOP_STEP);
}

bool MultiCheckPlayer::IsFinishGameStartAnimation()
{
	if (animController_->IsEnd())
	{ 
		return true;
	}
	return false;
}

void MultiCheckPlayer::RegisterStateUpdate(const STATE _state, std::function<void()> _update)
{
	stateMap_[_state] = _update;
}

void MultiCheckPlayer::UpdateSmoke()
{
	effectController_->Update();

	if (effectController_->GetAnimationIndex() == ResourceManager::SMOKE_EFE_DIV_X -1)
	{
		ChangeState(STATE::ANIMATION);
		return;
	}
}

void MultiCheckPlayer::UpdateAnimation()
{
	trans_.Update();

	animController_->Update();
}

void MultiCheckPlayer::SetParamByPlayerIndex()
{
	//スクリーン座標
	screenPos_ = { Application::SCREEN_SIZE_X / (playerNum_ + 1) * (playerIndex_ + 1), Application::SCREEN_HALF_Y };

	//カラー
	toon_->SetModelColor(
		PlayerManager::PLAYER_COLOR[playerIndex_].r,
		PlayerManager::PLAYER_COLOR[playerIndex_].g,
		PlayerManager::PLAYER_COLOR[playerIndex_].b,
		PlayerManager::PLAYER_COLOR[playerIndex_].a);
}
