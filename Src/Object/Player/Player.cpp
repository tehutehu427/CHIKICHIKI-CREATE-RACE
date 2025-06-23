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
#include"../Item/Installation/MoveHoriFloor.h"
#include"../Item/Installation/MoveVerFloor.h"

#include "../../Object/Editor/EditController.h"


#include "../../Manager/Game/ItemManager.h"
#include"./PlayerAction.h"
#include "./Process/PlayerInput.h"

#include<algorithm>


#include "Player.h"

Player::Player(int _playerNum,DateBank::TYPE _cntl, const Collider::TAG _tag)
	:playerNum_(_playerNum)
	, cntl_(_cntl)
	, tag_(_tag)
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
	//----------------------------------------------------
	//当たり判定
	//----------------------------------------------------
	collObjectTables_.emplace(Collider::TAG::START, [this](const std::weak_ptr<Collider> _hitCol)
		{
			colUpdate_ = [this, _hitCol]() {CollFloor(_hitCol); };
		});
	collObjectTables_.emplace(Collider::TAG::MOVE_HORI_FLOOR, [this](const std::weak_ptr<Collider> _hitCol)
		{
			colUpdate_ = [this, _hitCol]() {CollMoveFloor(_hitCol); };
		});
	collObjectTables_.emplace(Collider::TAG::MOVE_VER_FLOOR, [this](const std::weak_ptr<Collider> _hitCol)
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
	collObjectTables_.emplace(Collider::TAG::GOAL, [this](const std::weak_ptr<Collider> _hitCol)
		{
			colUpdate_ = [this, _hitCol]() {CollFloor(_hitCol); };
		});
	isCol_ = false;



	//コライダ作成
	//*****************************************************
	//球コライダ作成
	//プレイヤーの体
	std::unique_ptr<Sphere>bodySphereGeo = std::make_unique<Sphere>(trans_.pos, RADIUS);
	MakeCollider(tag_, std::move(bodySphereGeo));



	//接地しているときのライン(床上にとどまっているとき)
	//Lineを引くための上と下の座標をとる
	std::unique_ptr<Line>lineGeo = std::make_unique<Line>(trans_.pos,trans_.quaRot, LOCAL_DOWN_POS, LOCAL_UP_POS);
	MakeCollider(tag_, std::move(lineGeo));


	//現在の座標と移動後座標を結んだ線のコライダ(落下時の当たり判定)
	std::unique_ptr<Line>moveLineGeo = std::make_unique<Line>(trans_.pos,trans_.quaRot, Utility::VECTOR_ZERO,movedPos_);
	MakeCollider(tag_, std::move(moveLineGeo));

	moveDiff_ = Utility::VECTOR_ZERO;

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
	animationController_->Add(static_cast<int>(ANIM_TYPE::IDLE), DEFAULT_SPD);
	animationController_->Add(static_cast<int>(ANIM_TYPE::WALK), DEFAULT_SPD);
	animationController_->Add(static_cast<int>(ANIM_TYPE::FALL), DEFAULT_SPD);
	animationController_->Add(static_cast<int>(ANIM_TYPE::JUMP), DEFAULT_SPD);
	animationController_->Add(static_cast<int>(ANIM_TYPE::LAND), DEFAULT_SPD);
	animationController_->Add(static_cast<int>(ANIM_TYPE::PUNCH), DEFAULT_SPD / PUNCH_TIME_MAX);


	action_ = std::make_unique<PlayerAction>(*this, scnMng_, *animationController_);

	////プレイヤーの手(パンチの当たり判定)
	std::unique_ptr<Sphere>handSphereGeo = std::make_unique<Sphere>(action_->GetPunchPos(), PUNCH_RADIUS);
	MakeCollider(tag_, std::move(handSphereGeo));

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


	ChangeState(PLAYER_STATE::ALIVE);
	action_->Init();

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
	trans_.quaRot = action_->GetPlayerRotY();
	

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

inline const bool Player::GetIsPunch(void)
{
	return action_->GetIsHitPunch();
}

inline const VECTOR Player::GetPunchPos(void)
{
	return action_->GetPunchPos();
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
	colParam_[MOVE_LINE_COL_NO].geometry_->Draw();
	colParam_[UP_AND_DOWN_LINE_COL_NO].geometry_->Draw();
	colParam_[HAND_SPHERE_COL_NO].geometry_->Draw();
	//DrawSphere3D(trans_.pos, RADIUS, 10, color, color, false);

	VECTOR pow = action_->GetMovePow();


	DrawFormatString(0, 16*(playerNum_*9), 0x000000
		, "角度(%.2f,%.2f,%.2f)\njumpDecel(%f)\nstepJump_(%f)\njumpPow(%f,%f,%f)\nmovedPos(%f,%f,%f)\nmovePow(%f,%f,%f)\nMoveDiff(%f,%f,%f)"
		, trans_.rot.x, trans_.rot.y, trans_.rot.z
		,action_->GetJumpDecel()
		,action_->GetStepJump()
		,action_->GetJumpPow().x, action_->GetJumpPow().y, action_->GetJumpPow().z
		,movedPos_.x,movedPos_.y,movedPos_.z
		, pow.x, pow.y, pow.z
		,moveDiff_.x,moveDiff_.y,moveDiff_.z
	);



	if (IsDeath())
	{
		static int OFFSET = 32;
		//リトライするかEditシーンに戻るか選べるようにする。
		DrawFormatString(Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y+ OFFSET *playerNum_, 0xff0000, "(%d)GameOver", playerNum_);
	}
	

	//DrawSphere3D(punchPos_, PUNCH_RADIUS, 4, 0xff0000, 0xff0000, isPunchHitTime_);

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
	action_->Update();


	if (IsDeath())
	{
		//何もできないようにする
		action_->ChangeAction(PlayerAction::ATK_ACT::NONE);
	}
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
	Model& hitModel = dynamic_cast<Model&>(const_cast<Geometry&>(_hitCol.lock()->GetGeometry()));
	HitModelCommon(hitModel);
}

void Player::CollMoveFloor(const std::weak_ptr<Collider> _hitCol)
{
	if (_hitCol.lock()->GetTag() == Collider::TAG::MOVE_HORI_FLOOR|| _hitCol.lock()->GetTag() == Collider::TAG::MOVE_VER_FLOOR)
	{
		ItemBase& floor = dynamic_cast<ItemBase&>(const_cast<ObjectBase&>(_hitCol.lock()->GetParent()));
		VECTOR movePow = floor.GetMovePow();
		movedPos_ = VAdd(movedPos_, VScale(floor.GetMovePow(),0.5f));
	}
	Model& hitModel = dynamic_cast<Model&>(const_cast<Geometry&>(_hitCol.lock()->GetGeometry()));
	HitModelCommon(hitModel);



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



void Player::ColPunch(const std::weak_ptr<Collider> _hitCol)
{
	//パンチしたプレイヤーの向いてる方向をセットする
	VECTOR punchedPlayerPos = _hitCol.lock()->GetParent().GetTransform().pos;

	//パンチしたプレイヤーの位置と自分の位置を比較して、
	action_->SetDir(Utility::GetMoveVec(punchedPlayerPos, trans_.pos));

	//ノックバック状態遷移
	action_->ChangeAction(PlayerAction::ATK_ACT::KNOCKBACK);
}

void Player::Collision(void)
{
	VECTOR pow = action_->GetMovePow();
	movedPos_ = VAdd(trans_.pos, action_->GetMovePow());
	movedPos_ = VAdd(movedPos_, action_->GetJumpPow());

#ifdef DEBUG_ON

	//デバッグ用床の当たり判定
	if (CollCube())
	{
		movedPos_ = VAdd(movedPos_, cubeMovePos_);
		action_->SetJumpPow(Utility::VECTOR_ZERO);
		movedPos_.y = cube_.upPos.y + RADIUS;
		action_->SetStepJump(0.0f);
		action_->SetIsJump(false);
		action_->SetJumpDecel(POW_JUMP);
	}
	else
	{
		action_->SetIsJump(true);
		//if (jumpPow_.y <= LIMIT_GRAVITY)
		//{
		//	jumpPow_.y = LIMIT_GRAVITY;
		//}
	}

#endif // DEBUG_ON

	//移動量ラインの更新
	VECTOR moveVec = VSub(movedPos_, trans_.pos);
	if (!Utility::EqualsVZero(moveVec))
	{
		Line& moveLine = dynamic_cast<Line&>(colParam_[MOVE_LINE_COL_NO].collider_->GetGeometry());
		moveLine.SetLocalPosPoint1(Utility::VECTOR_ZERO);
		moveLine.SetLocalPosPoint2(moveVec);
	}

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

	//移動前の座標を格納する
	moveDiff_ = trans_.pos;
	//移動
	trans_.pos = movedPos_;
	// 現在座標を起点に移動後座標を決める
#ifdef DEBUG_ON

	//if (movedPos_.y < 0.0f/*||!CollCube()*/)
	//{
	//	movedPos_.y = 0.0f;
	//	isJump_ = false;
	//	stepJump_ = 0.0f;
	//}
#endif // DEBUG_ON

}

void Player::UpDownColl(const Transform _itemTrans)
{
	////移動後と移動前をとる
	//VECTOR prePos = trans_.pos;
	//VECTOR curPos = movedPos_;
	//
	//VECTOR vec = VSub(curPos, prePos);

	//auto hit = MV1CollCheck_Line(_itemTrans.modelId, -1, prePos, curPos);
	//MapEditer& mapEdit = MapEditer::GetInstance();
	//isLandHit_ = false;
	////当たったら
	//if (hit.HitFlag > 0)
	//{
	//	//Y座標のみ半径分上に移動させる
	//	movedPos_.y = hit.HitPosition.y + RADIUS + POSITION_OFFSET;
	//	action_->SetJumpPow(Utility::VECTOR_ZERO);
	//	action_->SetIsJump(false);
	//	itemLocalPos_ = VSub(movedPos_, _itemTrans.pos);
	//	isLandHit_ = true;
	//	return;
	//}
	////else
	////{
	////	//当たらなかったら初期化する
	////	itemLocalPos_ = Utility::VECTOR_ZERO;
	////}


	////Lineを引くための上と下の座標をとる
	//VECTOR upPos = movedPos_;
	//upPos.y += (RADIUS);
	//VECTOR downPos = movedPos_;
	//downPos.y -= (RADIUS+10.0f);

	//hit = MV1CollCheck_Line(_itemTrans.modelId, -1, upPos, downPos);

	////当たったら
	//if (hit.HitFlag > 0)
	//{
	//	//座標をワールド座標とアイテムローカル座標を足した分移動させる
	//	if (!Utility::EqualsVZero(itemLocalPos_))
	//	{
	//		VECTOR itemLocalPos = VSub(movedPos_, _itemTrans.pos);
	//		movedPos_ = VAdd(itemLocalPos_, _itemTrans.pos);
	//		movedPos_ = VAdd(movedPos_, vec);
	//		hitItemType_ =mapEdit.GetItemType(mapEdit.WorldToMapPos(hit.HitPosition));
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
	//	action_->SetJumpPow(Utility::VECTOR_ZERO);
	//	//isJump_ = false;
	//	itemLocalPos_ = VSub(movedPos_, _itemTrans.pos);
	//}
	//else
	//{
	//	//当たらなかったら初期化する
	//	itemLocalPos_ = Utility::VECTOR_ZERO;
	//	action_->SetIsJump(true);
	//	hitItemType_ = ItemBase::ITEM_TYPE::NONE;
	//}
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

void Player::HitModelCommon(Model& _hitModel)
{
	//Y座標のみ半径分上に移動させる
	VECTOR hitPos = _hitModel.GetHitLineInfo().HitPosition;
	auto& moveLineCol = colParam_[MOVE_LINE_COL_NO].collider_;
	auto& upDownLine = colParam_[UP_AND_DOWN_LINE_COL_NO].collider_;
	if (moveLineCol->IsHit() > 0)
	{
		//Y座標のみ半径分上に移動させる
		movedPos_.y = hitPos.y + RADIUS + POSITION_OFFSET;
		action_->SetJumpPow(Utility::VECTOR_ZERO);
		action_->SetIsJump(false);
		return;
	}
	//プレイヤーの接地
	if (upDownLine->IsHit() > 0)
	{
		Line& upDown = dynamic_cast<Line&>(upDownLine->GetGeometry());
		VECTOR hitLinePos = upDown.GetHitInfo().HitPosition;
		if (movedPos_.y >= hitLinePos.y)
		{
			movedPos_.y = hitLinePos.y + RADIUS + POSITION_OFFSET;
		}
		else
		{
			movedPos_.y = hitLinePos.y - RADIUS - POSITION_OFFSET;
		}
		action_->SetJumpPow(Utility::VECTOR_ZERO);
	}


	//球の当たり判定(プレイヤーの周囲)
	auto& bodyShere = colParam_[BODY_SPHERE_COL_NO].collider_;
	//移動後座標を一回格納し、移動前をとる
	Transform trans = Transform(trans_);
	trans.pos = movedPos_;
	trans.Update();
	if (bodyShere->IsHit())
	{
		auto& hitInfo = _hitModel.GetHitInfo();

		for (int i = 0; i < hitInfo.HitNum; i++)
		{
			auto hit = hitInfo.Dim[i];
			for (int tryCnt = 0; tryCnt < COL_TRY_CNT_MAX; tryCnt++)
			{
				int pHit = HitCheck_Sphere_Triangle(trans.pos, RADIUS
					, hit.Position[0], hit.Position[1], hit.Position[2]);
				if (pHit)
				{
					movedPos_ = VAdd(movedPos_, VScale(hit.Normal, 1.0f));
					//カプセルを移動させる
					trans.pos = movedPos_;
					trans.Update();
					continue;
				}
				break;
			}
		}
		//当たり判定情報の解放
		//MV1CollResultPolyDimTerminate(hitInfo);
	}
	//移動前の座標を格納する
	moveDiff_ = trans_.pos;
	//移動
	trans_.pos = movedPos_;
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


