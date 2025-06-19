#include "../../Utility/Utility.h"
#include "../Application.h"

#include "../../Manager/Game/GravityManager.h"
#include "../../Manager/Game/MapEditer.h"
#include "../../Manager/System/ResourceManager.h"
#include "../../Manager/System/SceneManager.h"
#include "../../Manager/System/Camera.h"

#include "../../Object/Common/Geometry/Sphere.h"
#include "../../Object/Common/Geometry/Line.h"
#include"../../Object/Common/Geometry/Model.h"

#include "../../Object/Common/AnimationController.h"

#include "../../Object/Editor/EditController.h"
#include "../../Object/Editor/EditController.h"
#include "../../Object/Editor/EditController.h"

#include "../../Manager/Game/ItemManager.h"
#include "./Process/PlayerInput.h"

#include<algorithm>


#include "Player.h"

Player::Player(int _playerNum,DateBank::TYPE _cntl, const Collider::TAG _tag):playerNum_(_playerNum), cntl_(_cntl)
{
#ifdef DEBUG_ON
	cubeMovePos_=Utility::VECTOR_ZERO;
	cubePos_=Utility::VECTOR_ZERO;
#endif // DEBUG_ON

	trans_ = Transform();
	movedPos_ = Utility::VECTOR_ZERO;
	//初めのJOYPADがkey_padなのでパッドの番号に合わせる
	if (cntl_ == DateBank::TYPE::CONTROLLER)
	{
		//パッド番号を設定
		padNum_ = static_cast<KeyConfig::JOYPAD_NO>(playerNum_ + 1);
	}
	else
	{
		padNum_ = KeyConfig::JOYPAD_NO::INPUT_KEY;
	}
	//プレイヤー状態
	changeStates_.emplace(PLAYER_STATE::ALIVE, [this]() {ChangeAlive();});
	changeStates_.emplace(PLAYER_STATE::DEATH, [this]() {ChangeDeath(); });
	//操作関連
	//----------------------------------------------------
	changeAction_.emplace(ATK_ACT::NONE, [this]() {ChangeNone(); });
	changeAction_.emplace(ATK_ACT::MOVE, [this]() {ChangeMove(); });
	changeAction_.emplace(ATK_ACT::INPUT, [this]() {ChangeInput(); });
	changeAction_.emplace(ATK_ACT::JUMP, [this]() {ChangeJump(); });
	changeAction_.emplace(ATK_ACT::PUNCH, [this]() {ChangePunch(); });
	changeAction_.emplace(ATK_ACT::KNOCKBACK, [this]() {ChangeKnockBack(); });
	//----------------------------------------------------
	//当たり判定
	//----------------------------------------------------
	collObjectTables_.emplace(Collider::TAG::START, [this](const std::weak_ptr<Collider> _hitCol)
		{
			colUpdate_ = [this, _hitCol]() {CollFloor(_hitCol); };
		});
	collObjectTables_.emplace(Collider::TAG::MOVE_FLOOR, [this](const std::weak_ptr<Collider> _hitCol)
		{
			colUpdate_ = [this, _hitCol]() {CollMoveFloor(_hitCol); };
		});
	collObjectTables_.emplace(Collider::TAG::KILLER_ITEM, [this](const std::weak_ptr<Collider> _hitCol) 
		{
			colUpdate_ = [this, _hitCol]() {CollKillerItem(_hitCol); };
		});
	collObjectTables_.emplace(Collider::TAG::SLIME_FLOOR, [this](const std::weak_ptr<Collider> _hitCol) 
		{
			colUpdate_ = [this, _hitCol]() {CollSlimeFloor(_hitCol); };
		});
	collObjectTables_.emplace(Collider::TAG::NORMAL_ITEM, [this](const std::weak_ptr<Collider> _hitCol) 
		{
			colUpdate_ = [this, _hitCol]() {CollFloor(_hitCol); };
		});

	isCol_ = false;
	//コライダ作成
	//*****************************************************
	//接地しているときのライン(床上にとどまっているとき)
	//Lineを引くための上と下の座標をとる
	std::unique_ptr<Line>lineGeo = std::make_unique<Line>(trans_.pos,trans_.quaRot, LOCAL_DOWN_POS, LOCAL_UP_POS);
	MakeCollider(_tag, std::move(lineGeo));
	//球コライダ作成
	//プレイヤーの体
	std::unique_ptr<Sphere>bodySphereGeo = std::make_unique<Sphere>(trans_.pos, RADIUS);
	MakeCollider(_tag, std::move(bodySphereGeo));

	////プレイヤーの手(パンチの当たり判定)
	std::unique_ptr<Sphere>handSphereGeo = std::make_unique<Sphere>(punchPos_, PUNCH_RADIUS);
	MakeCollider(_tag, std::move(handSphereGeo));

	//現在の座標と移動後座標を結んだ線のコライダ(落下時の当たり判定)
	std::unique_ptr<Line>moveLineGeo = std::make_unique<Line>(trans_.pos,trans_.quaRot, movedPos_,trans_.pos);
	MakeCollider(_tag, std::move(moveLineGeo));



	//*****************************************************
	

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
	
	itemLocalPos_ = Utility::VECTOR_ZERO;
	hitItemType_ = ItemBase::ITEM_TYPE::NONE;

	input_ = nullptr;
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
	animationController_->Add(static_cast<int>(ANIM_TYPE::IDLE), DEFAULT_SPD);
	animationController_->Add(static_cast<int>(ANIM_TYPE::WALK), DEFAULT_SPD);
	animationController_->Add(static_cast<int>(ANIM_TYPE::FALL), DEFAULT_SPD);
	animationController_->Add(static_cast<int>(ANIM_TYPE::JUMP), DEFAULT_SPD);
	animationController_->Add(static_cast<int>(ANIM_TYPE::LAND), DEFAULT_SPD);
	animationController_->Add(static_cast<int>(ANIM_TYPE::PUNCH), DEFAULT_SPD / PUNCH_TIME_MAX);


	//入力
	input_ = std::make_shared<PlayerInput>(padNum_, cntl_);
}

void Player::Init(void)
{
	//Transformの設定
	trans_.quaRot = Quaternion();
	trans_.scl = MODEL_SCL;
	trans_.quaRotLocal = 
		Quaternion::Euler({ 0.0f, Utility::Deg2RadF(MODEL_LOCAL_DEG), 0.0f });

	float posX = PLAYER_ONE_POS_X + DISTANCE_POS * playerNum_;

	trans_.pos={ posX,0.0f,0.0f };

	trans_.localPos = { 0.0f,-Player::RADIUS,0.0f };
	//操作関連
	//---------------------------------
	//当たり判定
	isCol_ = false;

	isJump_ = false;
	stepJump_ = 0.0f;
	jumpPow_ = Utility::VECTOR_ZERO;
	jumpDeceralation_ = POW_JUMP;

	//パンチ関係の初期化
	punchCnt_ = 0.0f;
	punchCoolCnt_ = 0.0f;
	punchPos_ = Utility::VECTOR_ZERO;
	punchedCnt_ = PUNCHED_TIME;

	ChangeAction(ATK_ACT::INPUT);
	ChangeState(PLAYER_STATE::ALIVE);

#ifdef DEBUG_ON
	cube_.centerPos = Utility::VECTOR_ZERO;
	cubeMovePos_ = Utility::VECTOR_ZERO;
#endif // DEBUG_ON


	itemLocalPos_ = Utility::VECTOR_ZERO;
	trans_.Update();
}

void Player::Update(void)
{
	animationController_->Update();
#ifdef DEBUG_ON
	CubeMove();
#endif // DEBUG_ON

	//プレイヤー状態更新
	stateUpdate_();

	//回転の同期
	trans_.quaRot = playerRotY_;
	

	trans_.Update();
}

void Player::Draw(void)
{
	MV1DrawModel(trans_.modelId);
#ifdef DEBUG_ON
	DrawDebug();
#endif // DEBUG_ON
}

void Player::OnHit(const std::weak_ptr<Collider> _hitCol)
{
	collObjectTables_[_hitCol.lock()->GetTag()](_hitCol);
	colUpdate_();
}


#ifdef DEBUG_ON
void Player::DrawDebug(void)
{
	unsigned int color = 0xffffff;
	const int HIGH = 10;
	const int WIDTH = 200;
	if (playerNum_ == 0) { color = 0xffffff; }
	else if (playerNum_ == 1) { color = 0x550000; }
	else if (playerNum_ == 2) { color = 0x00ff00; }
	else if (playerNum_ == 3) { color = 0x0000ff; }
	if (isCol_) { color = 0xff0000; }
	colParam_[BODY_SPHERE_COL_NO].geometry_->Draw();
	colParam_[UP_AND_DOWN_LINE_COL_NO].geometry_->Draw();
	//DrawSphere3D(trans_.pos, RADIUS, 10, color, color, false);
	DrawFormatString(0, 16*(playerNum_*9), 0x000000
		, "角度(%.2f,%.2f,%.2f)\njumpDecel(%f)\nstepJump_(%f)\njumpPow(%f,%f,%f)\nmovedPos(%f,%f,%f)\nmovePow(%d,%d)"
		, trans_.rot.x, trans_.rot.y, trans_.rot.z
		,jumpDeceralation_
		,stepJump_
		,jumpPow_.x,jumpPow_.y,jumpPow_.z
		,movedPos_.x,movedPos_.y,movedPos_.z
//		,InputManager::GetInstance().GetKnockLStickSize(padNum_).x, InputManager::GetInstance().GetKnockLStickSize(padNum_).y
	);
	if (IsDeath())
	{
		static int OFFSET = 32;
		//リトライするかEditシーンに戻るか選べるようにする。
		DrawFormatString(Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y+ OFFSET *playerNum_, 0xff0000, "(%d)GameOver", playerNum_);
	}
	

	DrawSphere3D(punchPos_, PUNCH_RADIUS, 4, 0xff0000, 0xff0000, isPunchHitTime_);

	DrawCube3D({ cube_.centerPos.x - CUBE_W,cube_.centerPos.y - CUBE_H,cube_.centerPos.z - CUBE_D }
	, { cube_.centerPos.x + CUBE_W,cube_.centerPos.y + CUBE_H,cube_.centerPos.z + CUBE_D }, 0xff0000, 0xff0000, true);
	//DrawFormatString(0, 80* (playerNum_ + 3), 0, "POS = %f,%f,%f", movedPos_.x, movedPos_.y, movedPos_.z);
	//DrawFormatString(0, 100* (playerNum_ + 3), 0, "jump = %d", static_cast<int>(isJump_));
}
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
	if (IsDeath())
	{
		ChangeState(PLAYER_STATE::DEATH);
		return;
	}

	//入力更新
	input_->Update();

	//プレイヤーの下を設定
	static VECTOR dirDown = trans_.GetDown();

	//重力(各アクションに重力を反映させたいので先に重力を先に書く)
	GravityManager::GetInstance().CalcGravity(dirDown, jumpPow_, 20.0f);

	//アクション関係
	Action();

	//衝突判定
	Collision();
}
void Player::ChangeDeath(void)
{
	stateUpdate_ = std::bind(&Player::DeathUpdate, this);
}
void Player::DeathUpdate(void)
{
	//死んだ時の処理
	//落ちているアニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::FALL), true);

	//アニメーションループ
	if (animationController_->GetAnimStep() >= FALL_ANIM_START)
	{
		animationController_->SetEndLoop(FALL_ANIM_START, FALL_ANIM_END, 60.0f);
	}

}
#endif // DEBUG_ON
void Player::Action(void)
{
	//各アクションの更新
	actionUpdate_();

	//プレイヤーの回転
	Rotate();

	//プレイヤーの方向とスピードの更新
	UpdateMoveDirAndPow();

	if (IsDeath())
	{
		//何もできないようにする
		ChangeAction(ATK_ACT::NONE);
	}
}

void Player::ChangeAction(ATK_ACT _act)
{
	act_ = _act;
	changeAction_[act_]();
}

void Player::NoneUpdate(void)
{

}

void Player::ActionInputUpdate(void)
{
	//入力に応じてアクションを変える
	using ACT_CNTL = PlayerInput::ACT_CNTL;
	if (input_->CheckAct(ACT_CNTL::MOVE))
	{
		ChangeAction(ATK_ACT::MOVE);
		return;
	}
	if (input_->CheckAct(ACT_CNTL::PUNCH))
	{
		ChangeAction(ATK_ACT::PUNCH);
		return;
	}
	if (input_->CheckAct(ACT_CNTL::JUMP))
	{
		ChangeAction(ATK_ACT::JUMP);
		return;
	}
}

void Player::ChangeInput(void)
{
	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE));
	actionUpdate_ = std::bind(&Player::ActionInputUpdate, this);
}

void Player::ChangeNone(void)
{
	actionUpdate_ = std::bind(&Player::NoneUpdate, this);
}

void Player::MoveUpdate(void)
{
	//移動中に入力が入った時の状態遷移
	if (input_->CheckAct(PlayerInput::ACT_CNTL::JUMP))
	{
		ChangeAction(ATK_ACT::JUMP);
		return;
	}
	else if (input_->CheckAct(PlayerInput::ACT_CNTL::PUNCH))
	{
		speed_ = 0.0f;
		ChangeAction(ATK_ACT::PUNCH);
		return;
	}
	else if (!input_->CheckAct(PlayerInput::ACT_CNTL::MOVE))
	{
		speed_ = 0.0f;
		ChangeAction(ATK_ACT::INPUT);
		return;
	}
	//入力方向の移動
	MoveDirFronInput();
}
void Player::MoveDirFronInput(void)
{
	//移動量を0にリセット
 	movePow_ = Utility::VECTOR_ZERO;

	//プレイヤー入力クラスから角度を取得
	VECTOR getDir = input_->GetDir();
	float deg = input_->GetMoveDeg();
	Quaternion cameraRot = scnMng_.GetCamera(playerNum_).lock()->GetQuaRotOutX();
	Quaternion angle = Quaternion::AngleAxis(Utility::Deg2RadF(deg), Utility::AXIS_Y);
	dir_ = cameraRot.PosAxis(getDir);
	dir_ = VNorm(dir_);

	if (!Utility::EqualsVZero(dir_))
	{
		//補完角度の設定(入力角度まで方向転換する)
		SetGoalRotate(deg);
	}
}
void Player::ChangeMove(void)
{
	animationController_->Play(static_cast<int>(ANIM_TYPE::WALK));
	speed_ = MOVE_SPEED;
	actionUpdate_ = std::bind(&Player::MoveUpdate, this);
}
void Player::UpdateMoveDirAndPow(void)
{
	//方向の更新
	moveDir_ = dir_;
	//移動量の更新
	movePow_ = VScale(moveDir_, speed_);
}
void Player::Rotate(void)
{
	stepRotTime_ -= PlayerInput::DELTA_TIME;
	// 回転の球面補間
	playerRotY_ = Quaternion::Slerp(
		playerRotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);
}
void Player::SetGoalRotate(double _deg)
{
	//カメラの角度を取得
	VECTOR cameraRot = scnMng_.GetCamera(playerNum_).lock()->GetAngles();
	Quaternion axis = Quaternion::AngleAxis(
		(double)cameraRot.y + Utility::Deg2RadF(_deg), Utility::AXIS_Y);
	// 現在設定されている回転との角度差を取る
	double angleDiff = Quaternion::Angle(axis, goalQuaRot_);
	// しきい値
	if (angleDiff > 0.1)
	{
		stepRotTime_ = TIME_ROT;
	}
	goalQuaRot_ = axis;
}
bool Player::IsDeath(void)
{
	if (trans_.pos.y <= DEATH_POS_Y)
	{
		return true;
	}
	return false;
}
void Player::ChangeModelColor(const COLOR_F _colorScale)
{
	MV1SetEmiColorScale(trans_.modelId, _colorScale);
}
void Player::JumpUpdate(void)
{
	//ジャンプ中移動キー押されてなかったらスピード0にする
	if (!input_->CheckAct(PlayerInput::ACT_CNTL::MOVE))speed_ = 0.0f;

	//ジャンプ処理
	Jump();

}
void Player::Jump(void)
{
	//ステップジャンプを基準にジャンプ減衰量を決める
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();
	stepJump_ += deltaTime;

	//空中アニメーションステップのループ設定
	animationController_->SetEndLoop(23.0f, 25.0f, 5.0f);

	//ジャンプ中も移動できるようにする
	MoveDirFronInput();

	//ジャンプカウントが0以上なら
	if (stepJump_ > 0.0f)
	{
		stepJump_ += deltaTime;
		//プレイヤーが落下していたら
		if (jumpDeceralation_ < 0.0f)
		{
			animationController_->Play(static_cast<int>(ANIM_TYPE::LAND));
		}
		//減衰量の計算
		float deceralation = stepJump_ * TIME_JUMP_SCALE;
		jumpDeceralation_ -= deceralation;

		//ジャンプ量に掛ける
		jumpPow_ = VScale(trans_.GetUp(), jumpDeceralation_);
	}

	//地面に着いたらジャンプ関係の変数リセット
	if (!isJump_)
	{
		jumpDeceralation_ = POW_JUMP;
		jumpPow_ = Utility::VECTOR_ZERO;
		stepJump_ = 0.0f;

		//動いていた場合の移動量リセット
		speed_ = 0.0f;
		ChangeAction(ATK_ACT::INPUT);
		return;
	}
}
void Player::ChangeJump(void)
{
	//ジャンプ関係
	isJump_ = true;
	stepJump_ = 0.0f;
	//アニメーションの再生
	animationController_->Play(
		(int)ANIM_TYPE::JUMP, false, 10.0f, 60.0f);
	//状態遷移
	actionUpdate_ = std::bind(&Player::JumpUpdate, this);
}
void Player::Punch(void)
{
	//プレイヤーの手の座標を設定する
	punchPos_ = MV1GetFramePosition(trans_.modelId, 10);

	//アニメーション
	animationController_->Play((int)ANIM_TYPE::PUNCH, false);

	//アニメステップを取得して一定のところで攻撃判定を発生させる
	float animStep = animationController_->GetAnimStep();
	if (animStep > PUNCH_HIT_END_ANIM_STEP)
	{
		isPunchHitTime_ = false;
	}
	else if (animStep > PUNCH_HIT_START_ANIM_STEP)
	{
		isPunchHitTime_ = true;
	}

	if (animationController_->IsEnd())
	{
		//パンチクールタイムセット
		punchCoolCnt_ = PUNCH_COOL_TIME;
		ChangeAction(ATK_ACT::INPUT);
	}
}

void Player::ChangePunch(void)
{
	punchCnt_ = 0.0f;
	punchCoolCnt_ = PUNCH_COOL_TIME;
	actionUpdate_ = std::bind(&Player::Punch, this);
}

void Player::KnockBack(void)
{
	//カウントの減算
	punchedCnt_ -= scnMng_.GetDeltaTime();
	if (punchedCnt_ < 0.0f)
	{
		punchedCnt_ = PUNCHED_TIME;
		ChangeAction(ATK_ACT::INPUT);
	}
}

void Player::ChangeKnockBack(void)
{
	//ダメージアニメーション
	//animationController_->Play((int)ANIM_TYPE::DAMAGE,true,)
	speed_ = FLY_AWAY_SPEED;
	actionUpdate_ = std::bind(&Player::KnockBack, this);
}




VECTOR Player::AddPosRotate(VECTOR _followPos, Quaternion _followRot, VECTOR _localPos)
{
	//座標回転
	VECTOR addPos = _followRot.PosAxis(_localPos);

	//足したものを返す
	return VAdd(_followPos, addPos);
}

void Player::HitItem(const IntVector3 _colPos)
{
	MapEditer& mapEdit = MapEditer::GetInstance();
	ItemManager& itemMng = ItemManager::GetInstance();
	if (mapEdit.IsObjectAtMapPos(_colPos))
	{
		IntVector3 lPos=mapEdit.GetLeaderMapPos(_colPos);
		for (auto& iLPos : itemLPos_)
		{
			if (iLPos == lPos)return;
		}
		
		
		//アイテムタイプ取得
		ItemBase::ITEM_TYPE type = mapEdit.GetItemType(_colPos);

		
		//アイテムのTransform取得
		Transform itemTrans = itemMng.GetItemTransform(lPos,type);

		UpDownColl(itemTrans);
		ArroundColl(itemTrans);

		itemLPos_.push_back(lPos);
	}
}

void Player::CollFloor(const std::weak_ptr<Collider> _hitCol)
{
	VECTOR itemPos = _hitCol.lock()->GetParent().GetTransform().pos;
	//移動後と移動前をとる
	VECTOR prePos = trans_.pos;
	VECTOR curPos = movedPos_;
	//移動前と移動後のベクトル
	VECTOR vec = VSub(curPos, prePos);

	//プレイヤーの上下ライン
	auto& bodyShere = colParam_[BODY_SPHERE_COL_NO].collider_;
	auto& moveLineCol= colParam_[MOVE_LINE_COL_NO].collider_;
	auto& upDownLineCol = colParam_[UP_AND_DOWN_LINE_COL_NO].collider_;


	if (upDownLineCol->IsHit())
	{
		Model& hitModel = dynamic_cast<Model&>(const_cast<Geometry&>(_hitCol.lock()->GetGeometry()));
		auto hitLineInfo = hitModel.GetHitLineInfo();
		//座標をワールド座標とアイテムローカル座標を足した分移動させる
		if (!Utility::EqualsVZero(itemLocalPos_))
		{

			VECTOR itemLocalPos = VSub(movedPos_, _hitCol.lock()->GetParent().GetTransform().pos);
			movedPos_ = VAdd(itemLocalPos_, itemPos);
			movedPos_ = VAdd(movedPos_, vec);
			//hitItemType_ = mapEdit.GetItemType(mapEdit.WorldToMapPos(hit.HitPosition));
		}
		//Y座標のみ半径分上に移動させる

		if (movedPos_.y > hitLineInfo.HitPosition.y)
		{
			movedPos_.y = hitLineInfo.HitPosition.y + RADIUS + POSITION_OFFSET;
		}
		else
		{
			movedPos_.y = hitLineInfo.HitPosition.y - RADIUS - POSITION_OFFSET;
		}
		jumpPow_ = Utility::VECTOR_ZERO;
		//isJump_ = false;
		//itemLocalPos_ = VSub(movedPos_, itemPos);
	}

	if (bodyShere->IsHit())
	{
		//移動後座標を一回格納し、移動前をとる
		Transform trans = Transform(trans_);
		trans.pos = movedPos_;
		trans.Update();

		//movedPos_ = VAdd(movedPos_, VScale(hit.Normal, 1.0f));
		// カプセルを移動させる
		trans.pos = movedPos_;
		trans.Update();

		//for (int i = 0; i < hitInfo.HitNum; i++)
		//{
		//	auto hit = hits.Dim[i];
		//	for (int tryCnt = 0; tryCnt < COL_TRY_CNT_MAX; tryCnt++)
		//	{
		//		int pHit = HitCheck_Sphere_Triangle(trans.pos, RADIUS
		//			, hit.Position[0], hit.Position[1], hit.Position[2]);
		//		if (pHit)
		//		{

		//			continue;
		//		}

		//		break;
		//	}

		//}
	}


	////Y座標のみ半径分上に移動させる
	//movedPos_.y = hit.HitPosition.y + RADIUS + POSITION_OFFSET;
	//jumpPow_ = Utility::VECTOR_ZERO;
	//isJump_ = false;
	//itemLocalPos_ = VSub(movedPos_, itemPos);
	//isLandHit_ = true;
	//return;
	//
	////else
	////{
	////	//当たらなかったら初期化する
	////	itemLocalPos_ = Utility::VECTOR_ZERO;
	////}


	//	//座標をワールド座標とアイテムローカル座標を足した分移動させる
	//	if (!Utility::EqualsVZero(itemLocalPos_))
	//	{
	//		VECTOR itemLocalPos = VSub(movedPos_, _itemTrans.pos);
	//		movedPos_ = VAdd(itemLocalPos_, _itemTrans.pos);
	//		movedPos_ = VAdd(movedPos_, vec);
	//		//hitItemType_ = mapEdit.GetItemType(mapEdit.WorldToMapPos(hit.HitPosition));
	//	}
	//	//Y座標のみ半径分上に移動させる

	//	if (movedPos_.y > hit.HitPosition.y)
	//	{
	//		movedPos_.y = hit.HitPosition.y + RADIUS + POSITION_OFFSET;
	//	}
	//	else
	//	{
	//		movedPos_.y = hit.HitPosition.y - RADIUS - POSITION_OFFSET;
	//	}
	//	jumpPow_ = Utility::VECTOR_ZERO;
	//	//isJump_ = false;
	//	itemLocalPos_ = VSub(movedPos_, _itemTrans.pos);
	//}
	//else
	//{
	//	//当たらなかったら初期化する
	//	itemLocalPos_ = Utility::VECTOR_ZERO;
	//	isJump_ = true;
	//	hitItemType_ = ItemBase::ITEM_TYPE::NONE;
	//}
}

void Player::CollMoveFloor(const std::weak_ptr<Collider> _hitCol)
{
	

}

void Player::CollSlimeFloor(const std::weak_ptr<Collider> _hitCol)
{

}

void Player::CollCannon(const std::weak_ptr<Collider> _hitCol)
{

}

void Player::CollKillerItem(const std::weak_ptr<Collider> _hitCol)
{

}



void Player::Collision(void)
{
	movedPos_ = VAdd(trans_.pos, movePow_);
	movedPos_ = VAdd(movedPos_, jumpPow_);

#ifdef DEBUG_ON

	//デバッグ用床の当たり判定
	if (CollCube())
	{
		movedPos_ = VAdd(movedPos_, cubeMovePos_);
		jumpPow_ = Utility::VECTOR_ZERO;
		movedPos_.y = cube_.upPos.y + RADIUS;
		stepJump_ = 0.0f;
		isJump_ = false;
		jumpDeceralation_ = POW_JUMP;
	}
	else
	{
		isJump_ = true;
		//if (jumpPow_.y <= LIMIT_GRAVITY)
		//{
		//	jumpPow_.y = LIMIT_GRAVITY;
		//}
	}

#endif // DEBUG_ON



	//MapEditer& mapEdit = MapEditer::GetInstance();
	//IntVector3 mapPos = mapEdit.WorldToMapPos(movedPos_);
	//for (int x = -COL_RANGE; x <= COL_RANGE; x++)
	//{
	//	for (int y = -COL_RANGE; y <= COL_RANGE; y++)
	//	{
	//		for (int z = -COL_RANGE; z <= COL_RANGE; z++)
	//		{
	//			colPos_ = mapPos + IntVector3{x, y, z};
	//			if (colPos_.x < 0 || colPos_.y < 0 || colPos_.z < 0)continue;
	//			HitItem(colPos_);
	//		}
	//	}
	//}
	itemLPos_.clear();

#ifdef DEBUG_ON

	//if (movedPos_.y < 0.0f/*||!CollCube()*/)
	//{
	//	movedPos_.y = 0.0f;
	//	isJump_ = false;
	//	stepJump_ = 0.0f;
	//}
#endif // DEBUG_ON
	//移動前の座標を格納する
	moveDiff_ = VSub(movedPos_, trans_.pos);
	// 移動
	trans_.pos = movedPos_;
	// 現在座標を起点に移動後座標を決める
}

void Player::UpDownColl(const Transform _itemTrans)
{
	//移動後と移動前をとる
	VECTOR prePos = trans_.pos;
	VECTOR curPos = movedPos_;
	
	VECTOR vec = VSub(curPos, prePos);

	auto hit = MV1CollCheck_Line(_itemTrans.modelId, -1, prePos, curPos);
	MapEditer& mapEdit = MapEditer::GetInstance();
	isLandHit_ = false;
	//当たったら
	if (hit.HitFlag > 0)
	{
		//Y座標のみ半径分上に移動させる
		movedPos_.y = hit.HitPosition.y + RADIUS + POSITION_OFFSET;
		jumpPow_ = Utility::VECTOR_ZERO;
		isJump_ = false;
		itemLocalPos_ = VSub(movedPos_, _itemTrans.pos);
		isLandHit_ = true;
		return;
	}
	//else
	//{
	//	//当たらなかったら初期化する
	//	itemLocalPos_ = Utility::VECTOR_ZERO;
	//}


	//Lineを引くための上と下の座標をとる
	VECTOR upPos = movedPos_;
	upPos.y += (RADIUS);
	VECTOR downPos = movedPos_;
	downPos.y -= (RADIUS);

	hit = MV1CollCheck_Line(_itemTrans.modelId, -1, upPos, downPos);
	int i=std::min(1, 2);

	//当たったら
	if (hit.HitFlag > 0)
	{
		//座標をワールド座標とアイテムローカル座標を足した分移動させる
		if (!Utility::EqualsVZero(itemLocalPos_))
		{
			VECTOR itemLocalPos = VSub(movedPos_, _itemTrans.pos);
			movedPos_ = VAdd(itemLocalPos_, _itemTrans.pos);
			movedPos_ = VAdd(movedPos_, vec);
			hitItemType_ =mapEdit.GetItemType(mapEdit.WorldToMapPos(hit.HitPosition));
		}
		//Y座標のみ半径分上に移動させる

		if (movedPos_.y > hit.HitPosition.y)
		{
			movedPos_.y = hit.HitPosition.y + RADIUS + POSITION_OFFSET;
		}
		else
		{
			movedPos_.y = hit.HitPosition.y - RADIUS - POSITION_OFFSET;
		}
		jumpPow_ = Utility::VECTOR_ZERO;
		//isJump_ = false;
		itemLocalPos_ = VSub(movedPos_, _itemTrans.pos);
	}
	else
	{
		//当たらなかったら初期化する
		itemLocalPos_ = Utility::VECTOR_ZERO;
		isJump_ = true;
		hitItemType_ = ItemBase::ITEM_TYPE::NONE;
	}
}

void Player::ArroundColl(Transform _itemTrans)
{
	////移動後座標を一回格納し、移動前をとる
	//Transform trans = Transform(trans_);
	//trans.pos = movedPos_;
	//trans.Update();

	//auto hits = MV1CollCheck_Sphere(_itemTrans.modelId, -1, trans.pos
	//	, RADIUS);
	//for (int i = 0; i < hits.HitNum; i++)
	//{
	//	auto hit = hits.Dim[i];
	//	for (int tryCnt = 0; tryCnt < COL_TRY_CNT_MAX; tryCnt++)
	//	{
	//		int pHit = HitCheck_Sphere_Triangle(trans.pos, RADIUS
	//			, hit.Position[0], hit.Position[1], hit.Position[2]);
	//		if (pHit)
	//		{
	//			movedPos_ = VAdd(movedPos_, VScale(hit.Normal, 1.0f));
	//			// カプセルを移動させる
	//			trans.pos = movedPos_;
	//			trans.Update();
	//			continue;
	//		}

	//		break;
	//	}
	//	
	//}
	//MV1CollResultPolyDimTerminate(hits);
}

#ifdef DEBUG_ON
void Player::CubeMove(void)
{
	auto& input = KeyConfig::GetInstance();
	const float SPD = 8.0f;
	cubeMovePos_ = Utility::VECTOR_ZERO;
	cube_.upPos = VAdd(cube_.centerPos, { 0.0f,CUBE_H,0.0f });
	if (CheckHitKey(KEY_INPUT_UP))cubeMovePos_.z += SPD;
	if (CheckHitKey(KEY_INPUT_DOWN))cubeMovePos_.z -= SPD;
	if (CheckHitKey(KEY_INPUT_RIGHT))cubeMovePos_.x += SPD;
	if (CheckHitKey(KEY_INPUT_LEFT))cubeMovePos_.x -= SPD;
	if (CheckHitKey(KEY_INPUT_T))cubeMovePos_.y -= SPD;
	if (CheckHitKey(KEY_INPUT_Y))cubeMovePos_.y += SPD;
	cube_.centerPos = VAdd(cube_.centerPos, cubeMovePos_);
}

bool Player::CollCube(void)
{
	VECTOR jumpLine = VSub(movedPos_, trans_.pos);
	VECTOR diff = VSub(cube_.centerPos, movedPos_);
	if (fabsf(diff.x) > CUBE_W + RADIUS
		|| fabsf(diff.y) > CUBE_H + RADIUS
		|| fabsf(diff.z) > CUBE_D + RADIUS)
	{
		return false;
	}
	return true;
}
#endif // DEBUG_ON


