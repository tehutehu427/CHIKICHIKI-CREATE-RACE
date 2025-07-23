#include "../../Utility/Utility.h"
#include "../Application.h"

#include "../../Manager/Game/GravityManager.h"
#include "../../Manager/Game/MapEditer.h"
#include "../../Manager/Game/PlayerManager.h"
#include "../../Manager/System/ResourceManager.h"
#include "../../Manager/System/SoundManager.h"
#include "../../Manager/System/SceneManager.h"
#include"../../Manager/System/DateBank.h"

#include "../../Manager/System/Camera.h"

#include "../../Renderer/ModelMaterial.h"
#include "../../Renderer/ModelRenderer.h"

#include "../../Object/Common/Geometry/Sphere.h"
#include "../../Object/Common/Geometry/Line.h"
#include"../../Object/Common/Geometry/Model.h"
#include"../../Object/Common/EffectController.h"

#include "../../Object/Common/AnimationController.h"
#include"../Item/Installation/MoveHoriFloor.h"
#include"../Item/Installation/MoveVerFloor.h"

#include "../../Object/Editor/EditController.h"

#include "../../Manager/Game/ItemManager.h"
#include"./PlayerAction.h"
#include"./PlayerOnHit.h"
#include "./Process/PlayerInput.h"
#include "./Shadow.h"
#include<algorithm>


#include "Player.h"
Player::Player(int _playerNum, KeyConfig::TYPE _cntl, const Collider::TAG _tag)
	:playerNum_(_playerNum)
	, cntl_(_cntl)
	, tag_(_tag)
{
	trans_ = Transform();
	material_ = nullptr;
	renderer_ = nullptr;


	//初めのJOYPADがkey_padなのでパッドの番号に合わせる
	//パッド番号を設定
	padNum_ = static_cast<KeyConfig::JOYPAD_NO>(playerNum_ + 1);


	//プレイヤー状態
	changeStates_.emplace(PLAYER_STATE::ALIVE, [this]() {ChangeAlive();});
	changeStates_.emplace(PLAYER_STATE::DEATH, [this]() {ChangeDeath(); });
	changeStates_.emplace(PLAYER_STATE::GOAL, [this]() {ChangeGoal(); });

	coinNum_ = 0;

	//////コライダ作成
	////*****************************************************
	////接地しているときのライン(床上にとどまっているとき)
	////Lineを引くための上と下の座標をとる
	//std::unique_ptr<Line>lineGeo = std::make_unique<Line>(trans_.pos,trans_.quaRot, LOCAL_DOWN_POS, LOCAL_UP_POS);
	//MakeCollider({ tag_ }, std::move(lineGeo));

	////プレイヤーの体
	//std::unique_ptr<Sphere>bodySphereGeo = std::make_unique<Sphere>(trans_.pos, RADIUS);
	//MakeCollider({ tag_ }, std::move(bodySphereGeo));

	////現在の座標と移動後座標を結んだ線のコライダ(落下時の当たり判定)
	//std::unique_ptr<Line>moveLineGeo = std::make_unique<Line>(trans_.pos,trans_.quaRot, Utility::VECTOR_ZERO,Utility::VECTOR_ZERO);
	//MakeCollider({ tag_ }, std::move(moveLineGeo));

	////階段の当たり判定のためのプレイヤーの目線からのライン
	//	//現在の座標と移動後座標を結んだ線のコライダ(落下時の当たり判定)
	//std::unique_ptr<Line>eyeLine = std::make_unique<Line>(trans_.pos, trans_.quaRot, EYE_HEIGHT, EYE_RANGE);
	//MakeCollider({ tag_ }, std::move(eyeLine));

	//*****************************************************
}

Player::~Player(void)
{
}

void Player::Load(void)
{
	//アニメーションでmodelIdを使うので先にモデルセットする
	trans_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::CHICKEN));

	//リソースの読み込みなど
	animationController_ = std::make_unique<AnimationController>(trans_.modelId);
	animationController_->Add(static_cast<int>(ANIM_TYPE::IDLE), DEFAULT_ANIM_SPD);
	animationController_->Add(static_cast<int>(ANIM_TYPE::WALK), DEFAULT_ANIM_SPD);
	animationController_->Add(static_cast<int>(ANIM_TYPE::FALL), DEFAULT_ANIM_SPD);
	animationController_->Add(static_cast<int>(ANIM_TYPE::JUMP), DEFAULT_ANIM_SPD);
	animationController_->Add(static_cast<int>(ANIM_TYPE::LAND), DEFAULT_ANIM_SPD);
	animationController_->Add(static_cast<int>(ANIM_TYPE::GOAL), DEFAULT_ANIM_SPD);
	animationController_->Add(static_cast<int>(ANIM_TYPE::PUNCH), DEFAULT_ANIM_SPD / PlayerAction::PUNCH_TIME_MAX);

	//アクション
	action_ = std::make_unique<PlayerAction>(*this, scnMng_, *animationController_);
	action_->Load();


	//影
	shadow_ = std::make_unique<Shadow>(trans_);
	shadow_->Load();

	//当たり判定
	onHitCol_ = std::make_unique<PlayerOnHit>(*action_, colParam_, trans_, tag_);
	onHitCol_->Load();

	//トゥーンにする
	material_ = std::make_unique<ModelMaterial>("ChickenOutlineVS.cso", 1, "OutlinePS.cso", 1);
	renderer_ = std::make_unique<ModelRenderer>(trans_.modelId, *material_);

}

void Player::Init(void)
{
	//コライダがある場合、削除する
	for (auto& col : colParam_)
	{
		col.collider_->Kill();
	}
	colParam_.clear();

	//本来コライダを作りたい場所
	//コライダ作成
	//*****************************************************
	//接地しているときのライン(床上にとどまっているとき)
	//Lineを引くための上と下の座標をとる
	std::unique_ptr<Line>lineGeo = std::make_unique<Line>(trans_.pos,trans_.quaRot, LOCAL_DOWN_POS, LOCAL_UP_POS);
	MakeCollider({ tag_ }, std::move(lineGeo));

	//プレイヤーの体
	std::unique_ptr<Sphere>bodySphereGeo = std::make_unique<Sphere>(trans_.pos, RADIUS);
	MakeCollider({ tag_ }, std::move(bodySphereGeo));

	//現在の座標と移動後座標を結んだ線のコライダ(落下時の当たり判定)
	std::unique_ptr<Line>moveLineGeo = std::make_unique<Line>(trans_.pos,trans_.quaRot, Utility::VECTOR_ZERO,Utility::VECTOR_ZERO);
	MakeCollider({ tag_ }, std::move(moveLineGeo));

	//階段の当たり判定のためのプレイヤーの目線からのライン
		//現在の座標と移動後座標を結んだ線のコライダ(落下時の当たり判定)
	std::unique_ptr<Line>eyeLine = std::make_unique<Line>(trans_.pos, trans_.quaRot, EYE_HEIGHT, EYE_RANGE);
	MakeCollider({ tag_ }, std::move(eyeLine));

	//*****************************************************


	//Transformの設定
	trans_.quaRot = Quaternion();
	trans_.scl = MODEL_SCL;
	trans_.quaRotLocal = 
		Quaternion::Euler({ 0.0f, Utility::Deg2RadF(MODEL_LOCAL_DEG), 0.0f });

	float posX = PLAYER_ONE_POS_X + DISTANCE_POS * playerNum_;
	trans_.pos={ posX,0.0f,0.0f };
	trans_.localPos = { 0.0f,-Player::RADIUS,0.0f };


	//生存状態
	ChangeState(PLAYER_STATE::ALIVE);

	time_ = 0.0f;

	action_->Init();

	goalTime_ = 0.0f;

	finishDelay_ = 0.0f;

	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE), true);

	//バッファー設定
	material_->AddConstBufVS(FLOAT4{ 3.0f,0.0f,0.0f ,0.0f });	//輪郭線太さ
	material_->AddConstBufPS(FLOAT4{ 0.0f,0.0f,0.0f ,1.0f });	//輪郭線カラー

	//当たり判定
	onHitCol_ = std::make_unique<PlayerOnHit>(*action_, colParam_, trans_,tag_);
	onHitCol_->Init();

	//更新
	trans_.Update();

	shadow_->Init();
}

void Player::Update(void)
{
	animationController_->Update();
	shadow_->Update();
#ifdef DEBUG_ON
	//CubeMove();
#endif // DEBUG_ON

	//プレイヤー状態更新
	stateUpdate_();

	//回転の同期
	trans_.quaRot = action_->GetPlayerRotY();
	
	trans_.Update();
	shadow_->Update();
}

void Player::Draw(void)
{
	if (IsDeath())return;
	//モデル描画のZBufferを無効にする
	MV1SetWriteZBuffer(trans_.modelId, false);

	//アウトライン描画
	renderer_->Draw();

	//モデル描画のZBufferを戻す
	MV1SetWriteZBuffer(trans_.modelId, true);

	//通常描画
	MV1DrawModel(trans_.modelId);
	
	//影の描画
	shadow_->Draw();

#ifdef DEBUG_ON
	DrawDebug();
#endif // DEBUG_ON
}

void Player::OnHit(const std::weak_ptr<Collider> _hitCol)
{
	onHitCol_->ColUpdate(_hitCol);
}

#ifdef DEBUG_ON
void Player::DrawDebug(void)
{
	unsigned int color = 0xffffff;
	const int HIGH = 10;
	const int WIDTH = 200;
	//onHitCol_->DrawDebug();


	VECTOR pow = action_->GetMovePow();
	VECTOR jumpPow = action_->GetJumpPow();
	VECTOR movedPos = onHitCol_->GetMovedPos();
	DrawFormatString(0, 16*(playerNum_*9), 0x000000
		, "角度(%.2f,%.2f,%.2f)\njumpDecel(%f)\nstepJump_(%f)\njumpPow(%f,%f,%f)\nisJump(%d)\nisLand(%d)"
		, trans_.rot.x, trans_.rot.y, trans_.rot.z
		,action_->GetJumpDecel()
		,action_->GetStepJump()
		, jumpPow.x, jumpPow.y, jumpPow.z
		,action_->GetIsJump()
		,onHitCol_->GetIsLandHit()
	);

	action_->DrawDebug();

	if (IsDeath())
	{
		static int OFFSET = 32;
		//リトライするかEditシーンに戻るか選べるようにする。
		DrawFormatString(Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y+ OFFSET *playerNum_, 0xff0000, "(%d)GameOver", playerNum_);
	}
}

#endif // DEBUG_ON

void Player::ChangeState(PLAYER_STATE _state)
{
	state_ = _state;
	changeStates_[state_]();
}
void Player::ChangeAlive(void)
{
	stateUpdate_ = std::bind(&Player::AliveUpdate, this);
}
void Player::AliveUpdate(void)
{
	//奈落に落ちたら死に状態へ遷移
	if (trans_.pos.y <= DEATH_POS_Y || onHitCol_->GetIsDeath())
	{
		ChangeState(PLAYER_STATE::DEATH);
		return;
	}
	else if (onHitCol_->GetIsGoal())
	{
		ChangeState(PLAYER_STATE::GOAL);
		return;
	}
	//アクション関係更新
	Action();

	//時間計測
	TimeUpdate();

	//移動後座標の更新
	onHitCol_->Update();
}
void Player::ChangeDeath(void)
{
	goalTime_ = -1;
	KillPunchCol();
	for (auto& col : colParam_)
	{
		col.collider_->Kill();
	}
	colParam_.clear();
	action_->StopResource();

	//死んだらコインを落とす
	onHitCol_->SetCoinNum(0);
	//パッド振動
	KeyConfig::GetInstance().PadVibration(padNum_, 300, 300);

	stateUpdate_ = std::bind(&Player::DeathUpdate, this);
}
void Player::DeathUpdate(void)
{
	//死んだ時の処理
	//終了からの遅延時間を計測
	finishDelay_ += scnMng_.GetInstance().GetDeltaTime();
	//落ちているアニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::FALL), true);

	KeyConfig::GetInstance().PadVibration(padNum_, DEATH_PAD_VIBRATION_TIME, DEATH_PAD_VIBRATION_POW);
	//アニメーションループ
	if (animationController_->GetAnimStep() >= FALL_ANIM_START)
	{
		animationController_->SetEndLoop(FALL_ANIM_START, FALL_ANIM_END, DEFAULT_ANIM_SPD);
	}
}
void Player::ChangeGoal(void)
{
	goalTime_ = time_;
	KillPunchCol();
	for (auto& col : colParam_)
	{
		col.collider_->Kill();
	}
	colParam_.clear();
	action_->StopResource();
	stateUpdate_ = std::bind(&Player::GoalUpdate, this);

}
void Player::GoalUpdate(void)
{
	//終了からの遅延時間を計測
	finishDelay_ += scnMng_.GetInstance().GetDeltaTime();
	//落ちているアニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::GOAL), true);
}

void Player::Action(void)
{
	//アクション関係の更新
	action_->Update();
	
	//死んだら何もしないようにする
	if (IsDeath())
	{
		//何もできないようにする
		action_->ChangeAction(PlayerAction::ATK_ACT::NONE);
	}
}

void Player::TimeUpdate(void)
{
	float deltaTime = scnMng_.GetInstance().GetDeltaTime();
	time_ += deltaTime;
}

const bool Player::GetIsLandHit(void)
{
	return onHitCol_->GetIsLandHit();
}

const bool Player::GetIsSlimeFloor(void)
{
	return onHitCol_->GetIsSlimeHit();
}

const bool Player::IsGoal(void) const
{
	return state_ == PLAYER_STATE::GOAL&&finishDelay_>=GOAL_DELAY;
}
bool Player::IsDeath(void)
{
	return state_ == PLAYER_STATE::DEATH && finishDelay_ >= DEATH_DELAY;
}

void Player::SetPos(const VECTOR _worldPos)
{
	trans_.pos = _worldPos;
	trans_.Update();
}

void Player::ChangeModelColor(const COLOR_F _colorScale)
{
	MV1SetEmiColorScale(trans_.modelId, _colorScale);
}

void Player::MakePunchCol(void)
{
	//事前に配列のサイズを取得する
	auto ParamSize = colParam_.size();

	for (int i = 0; i < ParamSize; i++)
	{
		auto tags = colParam_[i].collider_->GetTags();
		auto tagSize = colParam_[i].collider_->GetTags().size();
		for (int j = 0; j < tagSize; j++)
		{
			auto tagSize = colParam_[i].collider_->GetTags().size();
			//i,jの値が最後まで行ったとき
			if (i == ParamSize - 1 && j == tagSize - 1)
			{
				//コライダのパンチがなかったら生成する
				if (tags[j] != Collider::TAG::PUNCH)
				{
					//プレイヤーの手(パンチの当たり判定)
					std::unique_ptr<Sphere>handSphereGeo = std::make_unique<Sphere>(action_->GetPunchPos(), PUNCH_RADIUS);
					MakeCollider({ tag_,Collider::TAG::PUNCH }, std::move(handSphereGeo));
					return;
				}
			}
		}
	}
}

void Player::KillPunchCol(void)
{
	//事前に配列のサイズを取得する
	auto ParamSize = colParam_.size();

	for (int i = 0; i < ParamSize; i++)
	{
		auto tags = colParam_[i].collider_->GetTags();
		auto tagSize = colParam_[i].collider_->GetTags().size();
		for (int j = 0; j < tagSize; j++)
		{
			if (tags[j] == Collider::TAG::PUNCH)
			{
				colParam_[i].collider_->Kill();

				//colParamの末尾を消去する
				colParam_.pop_back();
				return;
			}
		}
	}
}
